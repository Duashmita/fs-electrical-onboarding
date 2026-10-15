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
        Start of public variables
    */

    struct State {
        int16_t torque_demand;
        bool apps_out_of_range;
        bool apps_deviation;
    };

    State get_state();
    int16_t get_torque_demand();
    bool get_apps_out_of_range();
    bool get_apps_deviation();

    /*
        End of public variables
    */

    /*
        Public method definitions, don't edit
    */
    ETCController(PinName APPS1_pin, PinName APPS2_pin);

    void update_state();

private:
    /*
        Start of private variables
    */
    Mutex state_mutex;
    AnalogIn apps1;
    AnalogIn apps2;
    
    float apps1_voltage = 0.0f;
    float apps2_voltage = 0.0f;

    float apps1_position = 0.0f;
    float apps2_position = 0.0f;

    float pedal_position = 0.0f;

    bool apps_out_of_range = false;
    bool apps_deviation = false;    
 
    bool range_timer_active = false;
    bool deviation_timer_active = false;

    Timer range_timer;
    Timer deviation_timer;
 
    bool range_fault_persistent = false;
    bool deviation_fault_persistent = false;
  
    int16_t torque_demand = 0;


    /*
        End of private variables
    */

    /*  
        !! IMPORTANT: All global voltages are based off     !!
        !! an older testing time. Still, voltages           !!
        !! should always be checked and updated if wrong.   !!
    */

    static constexpr float APPS1_MIN_VOLTAGE = 0.396;
    static constexpr float APPS1_MAX_VOLTAGE = 1.086f;

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
