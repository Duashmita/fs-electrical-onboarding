//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) : apps1(APPS1_pin), apps2(APPS2_pin) {
    motor_torque_demand  = {0};
    implaus_apps_out_of_range = false;
    implaus_apps_deviation = false;

    apps1_voltage = 0.0f;
    apps2_voltage = 0.0f;

    apps1_travel = 0.0f;
    apps2_travel = 0.0f;
}

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    if (value < 0.0f) {
        return 0.0f;
    }
    else if (value > 1.0f) {
        return 1.0f;
    }
    else {
        return value;
    }
}

/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
bool ETCController::in_range(float value, float low, float high) {
    return (value >= low && value <= high);
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

    // get both sensor voltages
    apps1_voltage = apps1.read_voltage();
    apps2_voltage = apps2.read_voltage();

    // calculate pedal position based off voltage
    apps1_travel = clamp((apps1_voltage - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE));
    apps2_travel = clamp((apps2_voltage - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE));

    // average travel
    float avg_travel = (apps1_travel + apps2_travel) / 2.0f;

    // deadzone (cut-off method)
    if (avg_travel < PEDAL_DEADZONE_PERCENTAGE) {
        avg_travel = 0.0f;
    }

    // calculate torque
    motor_torque_demand = static_cast<int16_t>(avg_travel * MAX_TORQUE);

    /*
        Finish the step listed above.
    */

    // Here to simplify this function and readability.
    update_implausibilities();
    if (implaus_apps_out_of_range || implaus_apps_deviation) {
        motor_torque_demand = 0;
    }
}

/*
    Timer based logic for the ETC to be rules compliant. MUST
    include rules T.4.2.4, T.4.2.9 (out of range).
*/
void ETCController::update_implausibilities() {
    /*
        Start of implaus logic checking.
    */

    // see if either voltages are in range
    bool out_of_range_detected = !in_range(apps1_voltage, APPS1_MIN_VOLTAGE, APPS1_MAX_VOLTAGE) ||
                                !in_range(apps2_voltage, APPS2_MIN_VOLTAGE, APPS2_MAX_VOLTAGE);

    // update our boolean values
    if (out_of_range_detected) {
        out_of_range_timer.start();
        if (out_of_range_timer.read_ms() >= 100) {
        implaus_apps_out_of_range = true;
        }
    } else {
        out_of_range_timer.stop();
        out_of_range_timer.reset();
        implaus_apps_out_of_range = false;
    }

    // see if there is deviation
    bool deviation_detected = fabs(apps1_travel - apps2_travel) > 0.10f;

    // update boolean values
    if (deviation_detected) {
        deviation_timer.start();
        if (deviation_timer.read_ms() >= 100) {
            implaus_apps_deviation = true;
        }
    } else {
        deviation_timer.stop();

        deviation_timer.reset();
        implaus_apps_deviation = false;
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
