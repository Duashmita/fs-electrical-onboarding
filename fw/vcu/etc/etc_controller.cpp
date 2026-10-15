//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"
#include <cmath>

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) 
    : apps1(APPS1_pin), apps2(APPS2_pin) {
    
}

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    if (value < 0.0f) {
        return 0.0f;
    }

    if (value > 1.0f) {
        return 1.0f;
    }

    return value;
}

/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
bool ETCController::in_range(float value, float low, float high) {
    return value >= low && value <= high;
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

    constexpr float ADC_REFERENCE_VOLTAGE = 3.3f;
    
    apps1_voltage = apps1.read() * ADC_REFERENCE_VOLTAGE;
    apps2_voltage = apps2.read() * ADC_REFERENCE_VOLTAGE;

    apps1_position = clamp(
    (apps1_voltage - APPS1_MIN_VOLTAGE) /
    (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE)
    );

    apps2_position = clamp(
        (apps2_voltage - APPS2_MIN_VOLTAGE) /
        (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE)
    );

    pedal_position = (apps1_position + apps2_position) / 2.0f;

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
void ETCController::update_implausibilities() {
    /*
        Start of implaus logic checking.
    */

    bool apps1_invalid = !in_range(
        apps1_voltage,
        APPS1_MIN_VOLTAGE,
        APPS1_MAX_VOLTAGE
    );

    bool apps2_invalid = !in_range(
        apps2_voltage,
        APPS2_MIN_VOLTAGE,
        APPS2_MAX_VOLTAGE
    );

    apps_out_of_range = apps1_invalid || apps2_invalid;

    /*
        End of implaus logic checking.
    */

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */

    
}
