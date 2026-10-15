//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) : pin1(APPS1_pin), pin2(APPS2_pin) {}

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    if (value >= 1.0) {
        value = 1.0;
    } else if (value <= 0) {
        value = 0.0;
    }

    return value;
}

/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
bool ETCController::in_range(float value, float low, float high) {
    if (low <= value && value <= high) return true;
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

   // Read Voltage from the pins on both APPS sensors
   pin1_voltage = pin1.read() * 3.3f;
   pin2_voltage = pin2.read() * 3.3f;

   // Calculating Pedal Positions with Deadzones Percentages
   pos1 = clamp((pin1_voltage - (APPS1_MIN_VOLTAGE + PEDAL_DEADZONE_PERCENTAGE)) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE - (2 * PEDAL_DEADZONE_PERCENTAGE)));
   pos2 = clamp((pin2_voltage - (APPS2_MIN_VOLTAGE + PEDAL_DEADZONE_PERCENTAGE)) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE - (2 * PEDAL_DEADZONE_PERCENTAGE)));

   // Calculating Average Voltages from both Pedal Positions
   float avg_volt = (pos1 + pos2) / 2.0;

   // Calculating Torque on Motor based on Average Measured Voltage
   calculated_torque = avg_volt * MAX_TORQUE;
   
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

    int64_t deviation_ms;
    int64_t out_of_range_ms;

    // Checking for Rule T.4.2.4
    if ((abs(pos1 - pos2)) > 0.1) {
        deviation_timer.start();
        deviation_ms = deviation_timer.elapsed_time().count() / 1000;

        if (deviation_ms > 100) {
            printf("Deviation Implaus detected\n");
            deviation_implaus = true;
            READY_TO_DRIVE = false;
        }
    } else {
        printf("Devation Implaus not detected\n");
        // deviation_implaus = false;
        deviation_timer.stop();
        deviation_timer.reset();
    }
    
    // Checking for Rule T.4.2.9
    if (!in_range(pin1_voltage, APPS1_MIN_VOLTAGE, APPS1_MAX_VOLTAGE) || !in_range(pin2_voltage, APPS2_MIN_VOLTAGE, APPS2_MAX_VOLTAGE)) {
        out_of_range_timer.start();
        out_of_range_ms = out_of_range_timer.elapsed_time().count() / 1000;

        if (out_of_range_ms > 100) {
            printf("Out of Range Implaus detected\n");
            out_of_range_implaus = true;
            READY_TO_DRIVE = false;
        }
    } else {
        printf("Out of Range Implaus not detected\n");
        // out_of_range_implaus = false;
        out_of_range_timer.stop();
        out_of_range_timer.reset();
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
