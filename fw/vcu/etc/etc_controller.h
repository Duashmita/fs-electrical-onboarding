//
// Created by Jackson Pinsonneault on 3/24/26.
//

#ifndef ETC_CONTROLLER_H
#define ETC_CONTROLLER_H

#include "mbed.h"
#include <cstdint>

class ETCController {
public:

    /*
        Start of public variables\
    */

    
    bool get_range_implaus(){
        return APPS_Range_Implausibility_Confirmed;
    }

    bool get_percent_implaus(){
        return APPS_Percent_Implausibility_Confirmed;
    }

    int16_t get_Motor_Torque_Demand(){
        return Motor_Torque_Demand;
    }



    
    /*
        End of public variables
    */

    /*
        Public method definitions, don't edit
    */
    ETCController(PinName APPS1_pin, PinName APPS2_pin); // <------ Constructor

    void update_state();

private:
    /*
        Start of private variables
    */

    AnalogIn APPS_sensor_1; // Initialize AnalogIn Pin so we can read their voltages later when we need them
    AnalogIn APPS_sensor_2; // Initialize AnalogIn Pin so we can read their voltages later when we need them
    
    float APPS1_Voltage = 0; // Voltage from APPS1
    float APPS2_Voltage = 0;// Voltage from APPS2

    float APPS1_Pedal_Position = 0;
    float APPS2_Pedal_Position = 0;

    float Clamped_APPS1_Pedal_Position = 0;
    float Clamped_APPS2_Pedal_Position = 0;

    int16_t Motor_Torque_Demand = 0;



    // bool APPS1_Sensor_Range_Valid;
    // bool APPS2_Sensors_Range_Valid;
    // bool APPS_Sensors_Range_Valid;

    bool APPS_Range_Implausibility_Confirmed; //Real value for Implausibilities, True if there is an Implausibility
    bool APPS_Percent_Implausibility_Confirmed; //Real Value for Implausibilities

    

    Timer Timer_Range_Implaus;
    Timer Timer_Percent_Implaus;
    

    /*
        End of private variables
    */

    /*  
        !! IMPORTANT: All global voltages are based off     !!
        !! an older testing time. Still, voltages           !!
        !! should always be checked and updated if wrong.   !!
    */

    static constexpr float APPS1_MIN_VOLTAGE = 0.396; //Min percentage Voltage kind of
    static constexpr float APPS1_MAX_VOLTAGE = 1.086f; //Max percentage voltage kind of

    static constexpr float APPS2_MIN_VOLTAGE = 0.439f;
    static constexpr float APPS2_MAX_VOLTAGE = 1.133f;

    static constexpr float PEDAL_DEADZONE_PERCENTAGE = 0.03;

    static constexpr int16_t MAX_TORQUE = 32767 * 0.1; // Multiplied by 0.1 for controlled 
                                                       // motor torque during onboarding.

    float clamp(float value);

    bool in_range(float value, float low, float high);

    void update_implausibilities();
};

#endif
