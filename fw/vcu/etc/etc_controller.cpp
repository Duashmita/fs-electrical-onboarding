//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"
#include <cmath>
#include <chrono>

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) 
    : apps1(APPS1_pin), apps2(APPS2_pin) {
    
}

int16_t ETCController::get_torque_demand() {
    return torque_demand;
}

bool ETCController::get_apps_out_of_range() {
    return apps_out_of_range;
}

bool ETCController::get_apps_deviation() {
    return apps_deviation;
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

    if (pedal_position <= PEDAL_DEADZONE_PERCENTAGE) {
        pedal_position = 0.0f;
    } else {
        pedal_position = clamp(
            (pedal_position - PEDAL_DEADZONE_PERCENTAGE) /
            (1.0f - PEDAL_DEADZONE_PERCENTAGE)
        );
    }

    torque_demand = static_cast<int16_t>(
        pedal_position * MAX_TORQUE
    );

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

    apps_out_of_range =
        !in_range(apps1_voltage,
                  APPS1_MIN_VOLTAGE,
                  APPS1_MAX_VOLTAGE) ||
        !in_range(apps2_voltage,
                  APPS2_MIN_VOLTAGE,
                  APPS2_MAX_VOLTAGE);

    apps_deviation =
        std::fabs(apps1_position - apps2_position) > 0.1f;


    /*
        End of implaus logic checking.
    */

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */

    if (apps_deviation) {
        if (!deviation_timer_active) {
            deviation_timer.reset();
            deviation_timer.start();
            deviation_timer_active = true;
        }

        deviation_fault_persistent =
            deviation_timer.elapsed_time() > std::chrono::milliseconds(100);
    } else {
        deviation_timer.stop();
        deviation_timer.reset();
        deviation_timer_active = false;
        deviation_fault_persistent = false;
    }

    if (apps_out_of_range || apps_deviation ||
        range_fault_persistent || deviation_fault_persistent) {
        torque_demand = 0;
    }
    
}
