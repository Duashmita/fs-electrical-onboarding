//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"
#include <cmath>

namespace {

bool persists_for(bool active, Timer &timer, bool &timing,
                  std::chrono::milliseconds timeout) {
    if (!active) {
        timer.stop();
        timer.reset();
        timing = false;
        return false;
    }

    if (!timing) {
        timer.reset();
        timer.start();
        timing = true;
    }

    return timer.elapsed_time() > timeout;
}

} // namespace

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin)
    : apps1_adc(APPS1_pin), apps2_adc(APPS2_pin) {}

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

    apps1_voltage = apps1_adc.read() * 3.3f;
    apps2_voltage = apps2_adc.read() * 3.3f;

    apps1_raw_position = (apps1_voltage - APPS1_MIN_VOLTAGE) /
                         (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE);
    apps2_raw_position = (apps2_voltage - APPS2_MIN_VOLTAGE) /
                         (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE);

    /*
        Finish the step listed above.
    */

    float apps1_position = clamp((apps1_raw_position - PEDAL_DEADZONE_PERCENTAGE) /
                                 (1.0f - PEDAL_DEADZONE_PERCENTAGE));
    float apps2_position = clamp((apps2_raw_position - PEDAL_DEADZONE_PERCENTAGE) /
                                 (1.0f - PEDAL_DEADZONE_PERCENTAGE));

    float average_position = (apps1_position + apps2_position) / 2.0f;
    torque_demand = static_cast<int16_t>(average_position * MAX_TORQUE);

    // Here to simplify this function and readability.
    update_implausibilities();

    if (implaus_apps_out_of_range || implaus_apps_deviation) {
        torque_demand = 0;
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

    bool out_of_range_now =
        !in_range(apps1_raw_position, -APPS_RANGE_MARGIN, 1.0f + APPS_RANGE_MARGIN) ||
        !in_range(apps2_raw_position, -APPS_RANGE_MARGIN, 1.0f + APPS_RANGE_MARGIN);

    bool deviation_now =
        std::fabs(apps1_raw_position - apps2_raw_position) > APPS_DEVIATION_LIMIT;

    /*
        End of implaus logic checking.
    */

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */

    implaus_apps_out_of_range =
        persists_for(out_of_range_now, out_of_range_timer, out_of_range_timing, IMPLAUS_TIMEOUT);
    implaus_apps_deviation =
        persists_for(deviation_now, deviation_timer, deviation_timing, IMPLAUS_TIMEOUT);
}