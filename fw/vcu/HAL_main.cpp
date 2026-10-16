#include <cstdio>
#include <string.h>
#include "analogin_api.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_uart.h"
#include "mbed.h"
#include "objects.h"

#include <cmath>

extern UART_HandleTypeDef huart2;
AnalogIn APPS1_adc{PC_1};
AnalogIn APPS2_adc{PC_2};
CAN can{PB_8, PB_9, 500000};

ADC_HandleTypeDef *APPS1_hadc = &(((analogin_t*)&APPS1_adc)->handle);
ADC_HandleTypeDef *APPS2_hadc = &(((analogin_t*)&APPS2_adc)->handle);
CAN_HandleTypeDef *hcan = &(((can_t*)&can)->CanHandle);

float clamp(float v) {
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

float read_voltage(ADC_HandleTypeDef *hadc) {
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, 10);
    uint32_t val = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return (val / 4095.0f) * 3.3f;
}

int main(void) {

    char msg[] = "Hello world!\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY); // Sending "Hello world!\n" over Serial

    const float APPS1_MIN = 0.396f;
    const float APPS1_MAX = 1.086f;
    const float APPS2_MIN = 0.439f;
    const float APPS2_MAX = 1.133f;
    const float DEADZONE = 0.03f;
    const int16_t MAX_TORQUE = 32767 * 0.1f;

    uint32_t range_timer_start = 0;
    bool range_timer_active = false;
    bool implaus_out_of_range = false;

    uint32_t dev_timer_start = 0;
    bool dev_timer_active = false;
    bool implaus_deviation = false;

    int16_t torque_demand = 0;

    while (true) {
        float apps1_volts = read_voltage(APPS1_hadc);
        float apps2_volts = read_voltage(APPS2_hadc);

        float raw_pos1 = clamp((apps1_volts - APPS1_MIN) / (APPS1_MAX - APPS1_MIN));
        float raw_pos2 = clamp((apps2_volts - APPS2_MIN) / (APPS2_MAX - APPS2_MIN));

        float pos1 = (raw_pos1 < DEADZONE) ? 0.0f : clamp((raw_pos1 - DEADZONE) / (1.0f - DEADZONE));
        float pos2 = (raw_pos2 < DEADZONE) ? 0.0f : clamp((raw_pos2 - DEADZONE) / (1.0f - DEADZONE));

        uint32_t now = HAL_GetTick();

        bool out_of_range = (apps1_volts < APPS1_MIN || apps1_volts > APPS1_MAX ||
                             apps2_volts < APPS2_MIN || apps2_volts > APPS2_MAX);
        if (out_of_range) {
            if (!range_timer_active) {
                range_timer_start = now;
                range_timer_active = true;
            } else if (now - range_timer_start >= 100) {
                implaus_out_of_range = true;
            }
        } else {
            range_timer_active = false;
            implaus_out_of_range = false;
        }

        bool dev = (std::fabs(pos1 - pos2) > 0.10f);
        if (dev) {
            if (!dev_timer_active) {
                dev_timer_start = now;
                dev_timer_active = true;
            } else if (now - dev_timer_start >= 100) {
                implaus_deviation = true;
            }
        } else {
            dev_timer_active = false;
            implaus_deviation = false;
        }

        if (implaus_out_of_range || implaus_deviation) {
            torque_demand = 0;
        } else {
            float avg = (pos1 + pos2) / 2.0f;
            float mapped = avg * avg;
            torque_demand = (int16_t)(mapped * MAX_TORQUE);
        }

        HAL_Delay(10);
    }

    return 0;
}
