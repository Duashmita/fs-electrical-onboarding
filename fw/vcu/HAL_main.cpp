#include <cstdio>
#include <string.h>
#include "analogin_api.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_uart.h"
#include "mbed.h"
#include "objects.h"

extern UART_HandleTypeDef huart2;
AnalogIn APPS1_adc{PC_1/* PIN FOR APPS1 */};
AnalogIn APPS2_adc{PC_2/* PIN FOR APPS2 */};
CAN can{PB_8, PB_9, 500000};

ADC_HandleTypeDef *APPS1_hadc = &(((analogin_t*)&APPS1_adc)->handle);
ADC_HandleTypeDef *APPS2_hadc = &(((analogin_t*)&APPS2_adc)->handle);
CAN_HandleTypeDef *hcan = &(((can_t*)&can)->CanHandle);

int main(void) {

    char msg[] = "Hello world!\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY); // Sending "Hello world!\n" over Serial
    
    bool adc_error_1; //Tells us if soemthing went wrong with conversion
    bool adc_error_2;

    uint32_t  ADC_Value_1;
    uint32_t  ADC_Value_2;
    while (true) {
        /*
            Add lines where you start the ADC, poll from it, then
            convert it to voltage from a 12 bit number.
           

            Next, you should add two TIM_HandleTypeDef objects for
            creating timers and using them for implaus checks.

            After confirming your approach works, try to research
            a way to get CAN communication going aswell.
        */
       
        HAL_ADC_Start(APPS1_hadc); //Enables ADC and starts conversion of the regular channels( Turns on the ADC)

        if( HAL_ADC_PollForConversion(APPS1_hadc, 10) != HAL_OK){ //Check if Converison is complete
             adc_error_1 = true;//If conversion is not complete, report error
        }else{
            ADC_Value_1 =  HAL_ADC_GetValue(APPS1_hadc); //If coversion is complete store value
        }
        

        HAL_ADC_Start(APPS2_adc); 
        if( HAL_ADC_PollForConversion(APPS2_hadc, 10) != HAL_OK){ //Check if Converison is complete
             adc_error_2 = true;//If conversion is not complete, report error
        }else{
            ADC_Value_2 =  HAL_ADC_GetValue(APPS2_hadc); //If coversion is complete store value
        }


        //Now need to conver to a voltage
        //Voltage = (ADC VALUE/ 4096) * 3.3
        float Voltage_1 = (ADC_Value_1 / 4096.0) * 3.3f; //Calculate Voltages
        float Voltage_2 = (ADC_Value_2 / 4096.0) * 3.3f;
        
        
    return 0;
}
}