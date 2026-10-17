#include <cstdio>
#include <string.h>
#include "analogin_api.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_uart.h"
#include "mbed.h"
#include "objects.h"
#include <algorithm>
#include <inttypes.h>

constexpr auto TICKER_CHANNEL = TIM_CHANNEL_2;
constexpr uint32_t TICKER_DELAY = 20000;

struct APPS {
    //constructor
    [[gnu::cold]]
    APPS (ADC_HandleTypeDef* handle, const float V_MIN, const float V_MAX) ;

    [[gnu::cold]]
    void init () ;

    //return if reading is good and in range.
    explicit operator bool() const ;

    //read voltage and update corrispondig values
    void update () ;

    static constexpr float PEDAL_DEADZONE_PERCENTAGE = 0.03f;
    static constexpr int16_t MAX_TORQUE = 32767 * 0.1;

    const float V_MIN, V_MAX;
    const float DEADZONE_SIZE; //effective v_min and v_max. Calculated ahead of time bc it is faster and 8 bytes

    ADC_HandleTypeDef* handle;

    //state
    float raw_voltage;
    float position;
    bool goodRead = false;
};

struct ImplausabilityTimer {
    [[gnu::cold]]
    ImplausabilityTimer (const TIM_HandleTypeDef* time_handle) ;

    [[gnu::const]]
    explicit operator bool() const ;
    bool timedout () ;

    void start () ;
    void stop  () ;

    static constexpr uint32_t IMPLAUSABILITY_TIMEOUT_DURATION = 100000;

    const TIM_HandleTypeDef* time_handle;

    uint32_t stTime;
    bool active = false;
};

extern UART_HandleTypeDef huart2;
AnalogIn APPS1_adc{PC_1};
AnalogIn APPS2_adc{PC_2};
CAN can{PB_8, PB_9, 500000}; //Assuming this does all the setup for can.

void send_sme_CAN_messages() ;
void send_sme_CAN_messages() ;

extern "C" void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim->Instance == TIM1) {
        send_etc_CAN_messages();
        send_sme_CAN_messages();
        uint32_t ccr = __HAL_TIM_GET_COMPARE(htim, TICKER_CHANNEL);
        __HAL_TIM_SET_COMPARE(htim, , ccr + TICKER_DELAY);
    }
}

ADC_HandleTypeDef *APPS1_hadc = &(((analogin_t*)&APPS1_adc)->handle);
ADC_HandleTypeDef *APPS2_hadc = &(((analogin_t*)&APPS2_adc)->handle);
CAN_HandleTypeDef *hcan = &(((can_t*)&can)->CanHandle);
TIM_HandleTypeDef* htim1;

APPS apps1 (APPS1_hadc, 0.396f, 1.086f);
APPS apps2 (APPS2_hadc, 0.439f, 1.133f);

ImplausabilityTimer range_timer(htim1), alignment_timer(htim1);

bool range_violation, alignment_violation, implausability_timeout = false;

int16_t torque = 0;

int main(void) {
    char msg[] = "Hello world!\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY); // Sending "Hello world!\n" over Serial

    htim1->Instance               = TIM1;
    htim1->Init.Prescaler         = 84 - 1;       // 84 MHz / 84 = 1 MHz
    htim1->Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim1->Init.Period            = 0xFFFFFFFF;   // Full 32-bit range
    htim1->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim1->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Init(htim1);
    HAL_TIM_Base_Start(htim1);

    apps1.init();
    apps2.init();

    __HAL_TIM_SET_COMPARE(htim1, TICKER_CHANNEL, us_now() + TICKER_DELAY);
    __HAL_TIM_CLEAR_FLAG(htim2, TIM_FLAG_CC2);   // Drop any stale match
    __HAL_TIM_ENABLE_IT(htim2, TIM_IT_CC2);

    while (!range_timer.timedout() && !alignment_timer.timedout()) {
        /*
            Add lines where you start the ADC, poll from it, then
            convert it to voltage from a 12 bit number.

            Next, you should add two TIM_HandleTypeDef objects for
            creating timers and using them for implaus checks.

            After confirming your approach works, try to research
            a way to get CAN communication going aswell.
        */

        apps1.update();
        apps2.update();

        if(apps1.goodRead && apps2.goodRead) torque = (apps1.position * APPS::MAX_TORQUE + apps2.position * APPS::MAX_TORQUE) / 2;

        //treat bad reads as out-of-range reads.
        range_violation     = !(bool)apps1 || !(bool)apps2;
        alignment_violation = std::abs(apps1.position - apps2.position) > 0.1f;

        if(range_violation) range_timer.start();
        else                range_timer.stop();

        if(alignment_violation) alignment_timer.start();
        else                    alignment_timer.stop();
    }

    implausability_timeout = true;

    while(true) { ThisThread::sleep_for(1s); }

    return 0;
}

void send_etc_CAN_messages() {
    uint8_t msg_buff[8] = {1, 0, 0, 0};

    /*
        Bit packing:
        targ board is little endian, so is message:
        bit 0 (msg_buff[0] & 1 ) = VCU_ETC_READY_TO_DRIVE            = 1 (in msg_buff def)
        bit 1 (msg_buff[0] & 2 ) = VCU_ETC_MOTOR_ENABLED             = !etc.implausability_timeout
        bit 4 (msg_buff[0] & 8 ) = VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE = etc.implausability_timeout & etc.range_violation
        bit 6 (msg_buff[0] & 32) = VCU_ETC_IMPLAUS_APPS_DEVIATION    = etc.implausability_timeout & etc.alignment_violation
        
        everything lse is zeroed as it is out of scope of this project
    */
    msg_buff[0] |= !implausability_timeout                        << 1;
    msg_buff[0] |= (implausability_timeout & range_violation    ) << 4;
    msg_buff[0] |= (implausability_timeout & alignment_violation) << 6;
    
    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = 0x123;
    tx.ExtId = 0;
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 8;
    tx.TransmitGlobalTime = DISABLE;

    uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint32_t mailbox;

    if ((mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) > 0) HAL_CAN_AddTxMessage(&hcan, &tx, msg_buff, &mailbox);
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the 
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    union {
        uint8_t u8[8];
        int16_t i16[4];
    } msg_buff = {0};
    
    /*
        Bit packing:
        targ board is little endian, so is message:
        bits 0-15 = SME_THROTL_TorqueDemand = etc.torque;

        everything lse is zeroed as it is out of scope of this project
            actually im assuming this ont work bc like SME_THROTL_MaxSpeed
            so lets do SME_THROTL_MaxSpeed = 32767
    */

    msg_buff.i16[0] = torque;
    msg_buff.i16[1] = 32767;

    CAN_TxHeaderTypeDef tx = {0};
    tx.StdId = 0x123;
    tx.ExtId = 0;
    tx.IDE   = CAN_ID_STD;
    tx.RTR   = CAN_RTR_DATA;
    tx.DLC   = 8;
    tx.TransmitGlobalTime = DISABLE;

    uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint32_t mailbox;

    if ((mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) > 0) HAL_CAN_AddTxMessage(&hcan, &tx, msg_buff, &mailbox);
}

// APPS
[[gnu::cold]]
APPS::APPS(ADC_HandleTypeDef* handle, const float V_MIN, const float V_MAX)
    : handle(handle), V_MIN(V_MIN), V_MAX(V_MAX), DEADZONE_SIZE((V_MAX - V_MIN) * PEDAL_DEADZONE_PERCENTAGE) {}

[[gnu::cold]]
void APPS::init() {
    HAL_ADC_Init(handle);
    HAL_ADC_Start(handle);
}

[[gnu::const]]
APPS::operator bool() const { return goodRead && V_MIN <= raw_voltage && raw_voltage <= V_MAX; }

void APPS::update() {
    goodRead = HAL_ADC_PollForConversion(handle, HAL_MAX_DELAY) == HAL_OK;
    if(!goodRead) [[unlikely]] {
        raw_voltage = position = NAN;
        return;
    }

    raw_voltage = HAL_ADC_GetValue(handle);
    
    if(raw_voltage <= V_MIN) [[unlikely]] { position = 0.0f; return; }
    if(raw_voltage >= V_MAX) [[unlikely]] { position = 1.0f; return; }
    position = std::clamp((raw_voltage - V_MIN) / (V_MAX - V_MIN), -1.0f, 1.0f);
}

//ImplausabilityTimer

[[gnu::cold]]
ImplausabilityTimer::ImplausabilityTimer(const TIM_HandleTypeDef* time_handle) : time_handle(time_handle) {}

[[gnu::const]]
ImplausabilityTimer::operator bool() const { return active; }

bool ImplausabilityTimer::timedout() {
    if(!active) return false;
    //considering the clock would overflow in about an hour, I do not feel the need to handle overflow.
    uint32_t reading = __HAL_TIM_GET_COUNTER(time_handle);
    return reading >= stTime + IMPLAUSABILITY_TIMEOUT_DURATION;
}

void ImplausabilityTimer::start () {
    if(active) return;
    stTime = HAL_TIM_ReadCapturedValue(time_handle, HAL_TIM_ACTIVE_CHANNEL_1);
    active = true;
}

void ImplausabilityTimer::stop() {
    if(active) active = false;
}