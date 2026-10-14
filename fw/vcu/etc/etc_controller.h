//
// Created by Jackson Pinsonneault on 3/24/26.
//

#ifndef ETC_CONTROLLER_H
#define ETC_CONTROLLER_H

#include "mbed.h"
#include <chrono>

class ETCController {
public:

    /*
        Start of public variables
    */

    int16_t torque_demand = 0;
    bool implaus_apps_out_of_range = false;
    bool implaus_apps_deviation = false;

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

    AnalogIn apps1_adc;
    AnalogIn apps2_adc;

    float apps1_voltage = 0.0f;
    float apps2_voltage = 0.0f;

    float apps1_raw_position = 0.0f;
    float apps2_raw_position = 0.0f;

    Timer out_of_range_timer;
    Timer deviation_timer;
    bool out_of_range_timing = false;
    bool deviation_timing = false;

    static constexpr std::chrono::milliseconds IMPLAUS_TIMEOUT{100};
    static constexpr float APPS_DEVIATION_LIMIT = 0.10f;
    static constexpr float APPS_RANGE_MARGIN = 0.10f;

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