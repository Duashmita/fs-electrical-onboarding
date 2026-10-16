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

int main(void) {

    char msg[] = "Hello world!\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY); // Sending "Hello world!\n" over Serial

    while (true) {
        /*
            Add lines where you start the ADC, poll from it, then
            convert it to voltage from a 12 bit number.

            Next, you should add two TIM_HandleTypeDef objects for
            creating timers and using them for implaus checks.

            After confirming your approach works, try to research
            a way to get CAN communication going aswell.
        */



    }

    return 0;
}
