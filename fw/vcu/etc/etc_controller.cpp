//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"

//TODO: make a hotpath/coldpath

//should this be on coldpath
//depends on if this is a truly rare exeption or if momentary violations are common

//would mark with [[gnu::cold]] but not a function and idk if we can add them to header file (it needs var access) (now not rlly bc ##)
#define VIOLATION(type)                     \
do {                                        \
    int t = type##_timer.read_ms();         \
    if(!t) type##_timer.start();            \
    else if(t > 100) [[unlikely]] {         \
        implausability_timeout = true;      \
        type##_timer.stop();                \
    }                                       \
} while (0);

[[gnu::const]]
static constexpr float to_position(const float v_low, const float v_high, const float v_in) {
    if(v_in <= v_low ) [[unlikely]] return 0.0f; //TODO: verify it is rare that the readings are out of range
    if(v_in >= v_high) [[unlikely]] return 1.0f;
    return (v_in - v_low) / (v_high - v_low);
}

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) : apps1_in(APPS1_pin), apps2_in(APPS2_pin) /* , apps1_out(APPS1_pin), apps2_out(APPS2_pin) */ {} //auto zeros everything else

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
[[gnu::const]]
float ETCController::clamp(float value) const {
    if(value < 0.0f) return 0.0f;
    if(value > 1.0f) return 1.0f;
    return value;
}

/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
[[gnu::const]]
bool ETCController::in_range(float value, float low, float high) const {
    return low <= value && value <= high;
}

/*
    Complicated method with the goal of refreshing voltages,
    pedal positions, implausibilities, and motor torque demand.
*/
void ETCController::update_state() {
    /*
        Retrieve both APPS sensor voltages, determine pedal
        positions from globally set variables in header file, 
        then calculate a motor torque demand based off the 
        average if the two positions. 

        If you want an extra challenge, include the pedal
        deadzone percentage when determining pedal positions. 
        If you want to get really difficult, maybe even 
        include your own pedal mapping to make it non linear!
    */

    v1 = apps1_in.read_voltage();
    v2 = apps2_in.read_voltage();

    //error case always gets [[unlikely]]
    if(range_violation || alignment_violation) [[unlikely]] {
        p1 = p2 = 0.0f;
        torque = 0;
        return;
    }
    
    //not clamped bc an out of range reading should be a fault of the sensor
    p1 = clamp(to_position(APPS1_MIN_VOLTAGE + APPS1_DEADZONE_VOLTAGE, APPS1_MAX_VOLTAGE - APPS1_DEADZONE_VOLTAGE, v1));
    p2 = clamp(to_position(APPS2_MIN_VOLTAGE + APPS1_DEADZONE_VOLTAGE, APPS2_MAX_VOLTAGE - APPS1_DEADZONE_VOLTAGE, v2));

    torque = (p1 * MAX_TORQUE + p2 * MAX_TORQUE) / 2;
    /*
        Finish the step listed above.
    */

    // Here to simplify this function and readability.
    update_implausibilities();
}

/*
    Timer based logic for the ETC to be rules compliant. MUST
    include rules T.4.2.4, T.4.2.9 (out of range).
*/
[[gnu::always_inline]]
void ETCController::update_implausibilities() {
    /*
        Start of implaus logic checking.
    */

    //TODO: need to see if the momentary implausablilties are not unlikly enough to warrent miss penalty
    //  accrued by [[unlikely]] when one occurs 
    
    range_violation = !in_range(v1, APPS1_MIN_VOLTAGE, APPS1_MAX_VOLTAGE) 
                   || !in_range(v2, APPS2_MIN_VOLTAGE, APPS2_MAX_VOLTAGE)
                   ;
    alignment_violation = std::abs(p1 - p2) > 0.1f;
    
    if(range_violation) [[unlikely]] { VIOLATION(range); }
    else if(range_timer.read() != 0) {
        range_timer.stop();
        range_timer.reset();
    }

    if(alignment_violation) [[unlikely]] { VIOLATION(alignment); }
    else if(alignment_timer.read() != 0) {
        alignment_timer.stop();
        alignment_timer.reset();
    }



    /*
        End of implaus logic checking.
    */

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */    
}