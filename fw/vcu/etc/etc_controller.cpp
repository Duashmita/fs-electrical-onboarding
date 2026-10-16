//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"
#include "AnalogIn.h"

#include <cmath>

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) : apps1(APPS1_pin), apps2(APPS2_pin) {}

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    if (value < 0) {
        return 0;
    } else if (value > 1) {
        return 1;
    } else {
        return value;
    }
}

/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
bool ETCController::in_range(float value, float low, float high) {
    if (low <= value && value <= high) {
        return true;
    }
    return false;
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

    apps1_voltage = apps1.read() * 3.3f; 
    apps2_voltage = apps2.read() * 3.3f;

    // determining pedal position from 0 to 1 by normalizing.

    apps1_position = clamp((
        (apps1_voltage - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE)
    ));

    apps2_position = clamp((
        (apps2_voltage - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE)
    ));

    float average_of_both_positions = (0.5f * (apps1_position + apps2_position));

    float deadzone_applied_average = clamp(
        (average_of_both_positions -  PEDAL_DEADZONE_PERCENTAGE) / (1.0f -(2 * PEDAL_DEADZONE_PERCENTAGE))
    );

    torque_demand = static_cast<int16_t>(deadzone_applied_average *  MAX_TORQUE);

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


    // see if either sensor's raw voltage is outside its calibrated range + margin, to take action after 100ms

    bool out_range_now = 
        !in_range(apps1_voltage, APPS1_MIN_VOLTAGE - APPS_RANGE_MARGIN, APPS1_MAX_VOLTAGE + APPS_RANGE_MARGIN) ||
        !in_range(apps2_voltage, APPS2_MIN_VOLTAGE - APPS_RANGE_MARGIN, APPS2_MAX_VOLTAGE + APPS_RANGE_MARGIN);

    bool deviation_now = fabsf(apps1_position - apps2_position) > APPS_DEVIATION_LIMIT;

    /*
        End of implaus logic checking.
    */

    if (out_range_now) {
        out_of_range.start(); //If the voltages are indeed out of their defined range, measure to see if it persists for 100ms.
    } else {
        out_of_range.stop();
        out_of_range.reset();
    }

    apps_out_of_range_implausible = chrono::duration_cast<chrono::milliseconds>(out_of_range.elapsed_time()) > IMPLAUS_TIME_LIMIT;

    if (deviation_now) {
        deviation_timer.start();
    } else {
        deviation_timer.stop();
        deviation_timer.reset();
    }
    apps_deviation_implausible = chrono::duration_cast<chrono::milliseconds>(deviation_timer.elapsed_time()) > IMPLAUS_TIME_LIMIT; //latch these so it stays manually true and has to be reset manually, deadzone is 2 times goes to 2 places

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */

    //if there are really implausibilities, shut off the motor.

    if (apps_out_of_range_implausible || apps_deviation_implausible) {
        torque_demand = 0;
    }

    
}
