//
// Created by Jackson Pinsonneault on 3/24/26.
//

#ifndef ETC_CONTROLLER_H
#define ETC_CONTROLLER_H

#include "mbed.h"

class ETCController {
public:

    /*
        Start of public variables
    */

    bool implausability_timeout = false;
    bool range_violation = false;
    bool alignment_violation = false;

    int16_t torque;

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

    Timer range_timer, alignment_timer;

    AnalogIn  apps1_in , apps2_in ;
    // AnalogOut apps1_out, apps2_out;

    float v1, v2;
    float p1, p2;

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

    static constexpr float PEDAL_DEADZONE_PERCENTAGE = 0.03f;

    static constexpr int16_t MAX_TORQUE = 32767 * 0.1; // Multiplied by 0.1 for controlled 
                                                       // motor torque during onboarding.

    //mine own constants
    static constexpr float APPS1_DEADZONE_VOLTAGE = (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE) * PEDAL_DEADZONE_PERCENTAGE;
    static constexpr float APPS2_DEADZONE_VOLTAGE = (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE) * PEDAL_DEADZONE_PERCENTAGE;


    float clamp(float value) const;

    bool in_range(float value, float low, float high) const;

    void update_implausibilities();
};

#endif
