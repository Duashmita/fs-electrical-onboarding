#include <cstdio>
#include <string.h>
#include "analogin_api.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_uart.h"
#include "mbed.h"
#include "objects.h"

extern UART_HandleTypeDef huart2;
AnalogIn APPS1_adc{PC_1};
AnalogIn APPS2_adc{PC_2};
CAN can{PB_8, PB_9, 500000};

ADC_HandleTypeDef *APPS1_hadc = &(((analogin_t*)&APPS1_adc)->handle);
ADC_HandleTypeDef *APPS2_hadc = &(((analogin_t*)&APPS2_adc)->handle);
CAN_HandleTypeDef *hcan = &(((can_t*)&can)->CanHandle);

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

    while (true) {
        float apps1_volts = read_voltage(APPS1_hadc);
        float apps2_volts = read_voltage(APPS2_hadc);
    }

    return 0;
}
