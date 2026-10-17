//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"
#include <cmath>

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin)
    : apps1(APPS1_pin), apps2(APPS2_pin) {}

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    if (value < 0.0f) {
        return 0.0f;
    } else if (value > 1.0f) {
        return 1.0f;
    } else {
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
    apps1_volts = apps1.read() * 3.3f;
    apps2_volts = apps2.read() * 3.3f;

    float raw_pos1 = clamp((apps1_volts - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE));
    float raw_pos2 = clamp((apps2_volts - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE));

    if (raw_pos1 < PEDAL_DEADZONE_PERCENTAGE) {
        apps1_pos = 0.0f;
    } else {
        apps1_pos = clamp((raw_pos1 - PEDAL_DEADZONE_PERCENTAGE) / (1.0f - 2*PEDAL_DEADZONE_PERCENTAGE));
    }

    if (raw_pos2 < PEDAL_DEADZONE_PERCENTAGE) {
        apps2_pos = 0.0f;
    } else {
        apps2_pos = clamp((raw_pos2 - PEDAL_DEADZONE_PERCENTAGE) / (1.0f - PEDAL_DEADZONE_PERCENTAGE));
    }

    update_implausibilities();

    if (implause_apps_out_of_range || implaus_apps_deviation) {
        torque_demand = 0;
    } else {
        float avg_pos = (apps1_pos + apps2_pos) / 2.0f;
        // float mapped_pos = avg_pos * avg_pos;
        torque_demand = (int16_t)(avg_pos * MAX_TORQUE);
    }
}

/*
    Timer based logic for the ETC to be rules compliant. MUST
    include rules T.4.2.4, T.4.2.9 (out of range).
*/
void ETCController::update_implausibilities() {
    bool out_of_range = !in_range(apps1_volts, APPS1_MIN_VOLTAGE, APPS1_MAX_VOLTAGE) ||
                        !in_range(apps2_volts, APPS2_MIN_VOLTAGE, APPS2_MAX_VOLTAGE);

    if (out_of_range) {
        timer_range.start();
        if (timer_range.elapsed_time().count() >= 100000) {
            implause_apps_out_of_range = true;
        }
    } else {
        timer_range.stop();
        timer_range.reset();
    }

    bool deviating = (std::fabs(apps1_pos - apps2_pos) > 0.10f);

    if (deviating) {
        timer_dev.start();
        if (timer_dev.elapsed_time().count() >= 100000) {
            implaus_apps_deviation = true;
        }
    } else {
        timer_dev.stop();
        timer_dev.reset();
        implaus_apps_deviation = false;
    }
}
