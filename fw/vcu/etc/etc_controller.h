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

//torque request and two safety flags that other code can read (public--main can read) -y

int16_t torque_demand = 0; //final torque request, whole num, starts 0 -y
bool apps_deviation_implaus = false; //true when the two sensors disagree. start false but stays true until restart -y
bool apps_range_implaus = false; // is true when sensor reads outside range, lanches until restart like above -y
 
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

// one reader for each pedal sensor, constructur gives their pins. (private --only this class) -y

    AnalogIn apps1;
    AnalogIn apps2;

// stopwatches: how long each problem has lasted (needs >100ms to trip) -y

    Timer apps_deviation_timer;
    Timer apps_range_timer;

   

// latest voltage and pedal position for each sensor, kept here so every fn can see them -y

    float apps1_voltage = 0.0f;
    float apps2_voltage = 0.0f;
    float apps1_pedal_position = 0.0f;
    float apps2_pedal_position = 0.0f;

    /*
        End of private variables
    */

    /*  
        !! IMPORTANT: All global voltages are based off     !!
        !! an older testing time. Still, voltages           !!
        !! should always be checked and updated if wrong.   !!
    */

    static constexpr float APPS1_MIN_VOLTAGE = 0.396f;
    static constexpr float APPS1_MAX_VOLTAGE = 1.086f;

    static constexpr float APPS2_MIN_VOLTAGE = 0.439f;
    static constexpr float APPS2_MAX_VOLTAGE = 1.133f;

    static constexpr float PEDAL_DEADZONE_PERCENTAGE = 0.03f;

    static constexpr int16_t MAX_TORQUE = 32767 * 0.1; // Multiplied by 0.1 for controlled 
                                                       // motor torque during onboarding.

    static constexpr float APPS_VOLTAGE_TOLERANCE = 0.1f;
    static constexpr float APPS_DEVIATION_LIMIT = 0.10f;

    static constexpr std::chrono::milliseconds APPS_TIME_LIMIT{100};

    float clamp(float value);

    bool in_range(float value, float low, float high);

    void update_implausibilities();
};

#endif
