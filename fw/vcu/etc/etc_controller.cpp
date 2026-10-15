//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"
#include <cmath>

// Assign appropriate GPIO objects depending on pin parameters

// connects each pedal sensor to its pin when the controller is created -y

ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) :
apps1(APPS1_pin), apps2(APPS2_pin) {}

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/

// stops a value from going below 0 or above 1 -y

float ETCController::clamp(float value) {
    if (value < 0.0f) {
        return 0.0f;
    } 
    if (value > 1.0f){
        return 1.0f;
    }
    return value;
}
 
/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/

// true if value is between low and high, otherwise false -y

bool ETCController::in_range(float value, float low, float high) {
        return (low <= value) && (value <= high);
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

    // reads each sensor as a 0 to 1 fractions, then multplies by 3.3 to get volts -y

apps1_voltage = apps1.read() * 3.3f;
apps2_voltage = apps2.read() * 3.3f;

// turns each voltage into a 0 to 1 pedal position using its min and max, then clamps it (no higher/lower) -y
 
apps1_pedal_position = clamp((apps1_voltage - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE));
apps2_pedal_position = clamp((apps2_voltage - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE));

// deadzone both 3% of pedal travel counts as 0 (resting foot means no torque)
// then the rest is scaled linearly back to 0-1 so full pedal is still 100% -y -- UPDATED TO BOTH AS PER WHITEBOARD FORMULA

apps1_pedal_position = clamp((apps1_pedal_position - PEDAL_DEADZONE_PERCENTAGE) / (1.0f -2.0f * PEDAL_DEADZONE_PERCENTAGE));
apps2_pedal_position = clamp((apps2_pedal_position - PEDAL_DEADZONE_PERCENTAGE) / (1.0f -2.0f * PEDAL_DEADZONE_PERCENTAGE)); 
   
torque_demand = static_cast<int16_t>(((apps1_pedal_position + apps2_pedal_position) / 2.0f) * MAX_TORQUE);

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

// range check (t.4.2.9/10): broken or shorted sensor = voltage outside its window -y
bool apps1_in_range = in_range(apps1_voltage, APPS1_MIN_VOLTAGE - APPS_VOLTAGE_TOLERANCE, APPS1_MAX_VOLTAGE + APPS_VOLTAGE_TOLERANCE); 
bool apps2_in_range = in_range(apps2_voltage, APPS2_MIN_VOLTAGE - APPS_VOLTAGE_TOLERANCE, APPS2_MAX_VOLTAGE + APPS_VOLTAGE_TOLERANCE);

bool out_of_range = !apps1_in_range || !apps2_in_range;

// deviation check (T.4.2.4): sensors disagree by more than 10% of pedal travel -y
bool deviating = fabsf(apps1_pedal_position - apps2_pedal_position) > APPS_DEVIATION_LIMIT;

    /*
        End of implaus logic checking.
    */

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */

    // deviation timer: keep counting while sensors disagree, reset when they agree -y

    if (deviating) {
        apps_deviation_timer.start();
    } else {
        apps_deviation_timer.stop();
        apps_deviation_timer.reset();
    }

    // range timer: keep counting while a sensor is out of range, reset when both are fine -y
    if (out_of_range) {
        apps_range_timer.start();
    } else {
        apps_range_timer.stop();
        apps_range_timer.reset();
    }

// flags turn on after the problem has lasted more than 100 ms (T.4.2.5), then latch (stay true until restart) -y
    if (chrono::duration_cast<chrono::milliseconds>(apps_deviation_timer.elapsed_time()) > APPS_TIME_LIMIT) {
        apps_deviation_implaus = true;
    }
    if (chrono::duration_cast<chrono::milliseconds>(apps_range_timer.elapsed_time())> APPS_TIME_LIMIT) {
        apps_range_implaus = true;
    }

// stop motor power while either implausibility is active -y
    if (apps_range_implaus || apps_deviation_implaus) {
        torque_demand = 0;
}
}
