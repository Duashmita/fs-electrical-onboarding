#include <cstdio>
#include <string.h>
#include "analogin_api.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_uart.h"
#include "mbed.h"
#include "objects.h"

/* temp disabled HAL UART debugging as huart2 not initialized or defined in the project */
// extern UART_HandleTypeDef huart2;
AnalogIn APPS1_adc{PC_1};
AnalogIn APPS2_adc{PC_2};
CAN can{PB_8, PB_9, 500000};

ADC_HandleTypeDef *APPS1_hadc = &(((analogin_t*)&APPS1_adc)->handle);
ADC_HandleTypeDef *APPS2_hadc = &(((analogin_t*)&APPS2_adc)->handle);
CAN_HandleTypeDef *hcan = &(((can_t*)&can)->CanHandle);

constexpr float APPS1_MIN_VOLTAGE = 0.396;
constexpr float APPS1_MAX_VOLTAGE = 1.086f;

constexpr float APPS2_MIN_VOLTAGE = 0.439f;
constexpr float APPS2_MAX_VOLTAGE = 1.133f;

constexpr float PEDAL_DEADZONE_PERCENTAGE = 0.03;

constexpr int16_t MAX_TORQUE = 32767 * 0.1; // Multiplied by 0.1 for controlled 
                                                    // motor torque during onboarding.

TIM_HandleTypeDef range_timer{};
TIM_HandleTypeDef deviation_timer{};

bool range_timer_active = false;
bool deviation_timer_active = false;

bool range_fault_persistent = false;
bool deviation_fault_persistent = false;

float clamp(float value) {
    if (value < 0.0f) {
        return 0.0f;
    }

    if (value > 1.0f) {
        return 1.0f;
    }

    return value;
}

bool read_voltage(ADC_HandleTypeDef *hadc, float &voltage) {
    // start the adc
    if (HAL_ADC_Start(hadc) != HAL_OK) {
        return false;
    }

    // wait for conversion to complete, with a timeout of 10ms
    if (HAL_ADC_PollForConversion(hadc, 10) != HAL_OK) {
        HAL_ADC_Stop(hadc);
        return false;
    }

    //retrieve value
    float adc_value = static_cast<float>(HAL_ADC_GetValue(hadc));
    HAL_ADC_Stop(hadc);

    voltage = adc_value / 4095.0f * 3.3f; //12 bit adc, 3.3v reference

    return true;
}

int16_t calculate_torque(float apps1_voltage, float apps2_voltage) {
    float apps1_position = clamp(
        (apps1_voltage - APPS1_MIN_VOLTAGE) /
        (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE)
    );

    float apps2_position = clamp(
        (apps2_voltage - APPS2_MIN_VOLTAGE) /
        (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE)
    );

    float pedal_position = (apps1_position + apps2_position) / 2.0f;

    pedal_position = clamp(
        (pedal_position - PEDAL_DEADZONE_PERCENTAGE) /
        (1.0f - 2 * PEDAL_DEADZONE_PERCENTAGE)
    );

    return static_cast<int16_t>(pedal_position * MAX_TORQUE);
}


int main(void) {

    // char msg[] = "Hello world!\n";
    // HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY); // Sending "Hello world!\n" over Serial

    while (true) {
        /*
            Add lines where you start the ADC, poll from it, then
            convert it to voltage from a 12 bit number.

            Next, you should add two TIM_HandleTypeDef objects for
            creating timers and using them for implaus checks.

            After confirming your approach works, try to research
            a way to get CAN communication going aswell.
        */

        float apps1_voltage = 0.0f;
        float apps2_voltage = 0.0f;

        int16_t torque_demand = 0;

        // make sure to check if the ADC read was successful before printing the voltages
        bool apps1_ok = read_voltage(APPS1_hadc, apps1_voltage);
        bool apps2_ok = read_voltage(APPS2_hadc, apps2_voltage);

        if (apps1_ok && apps2_ok) {
            torque_demand = calculate_torque(apps1_voltage, apps2_voltage);
            printf("APPS1: %.3f V, APPS2: %.3f V\n, Torque Demand: %d\n",
                apps1_voltage, apps2_voltage, torque_demand);
        } else {
            torque_demand = 0;
            printf("ADC read failed\n");
        }

        ThisThread::sleep_for(std::chrono::milliseconds(100));

    }

    return 0;
}
