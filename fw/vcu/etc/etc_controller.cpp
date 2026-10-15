// 
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"
#include <cmath>

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin)   
   :    myread_1(APPS1_pin),
        myread_2(APPS2_pin),
        motor_torque_demand(0.0f) // defaultly setting this to 0 for now
{

}

/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    if (value < 0) {
        return 0.0;
    } else if (value > 1) {
        return 1.0;
    }

    return value;
}

/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
bool ETCController::in_range(float value, float low, float high) {
    if (value <= high && value >= low) {
        return true;
    } else {
        return false;
    }
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

    // retrieve the APPS sensor voltages..
    float voltage_1 = myread_1.read() * 3.3;
    float voltage_2 = myread_2.read() * 3.3;
    // ^^ both multiplied by microcontroller reference voltage                                   

    // convert to float \in [0.0, 1.0]
    float pedal_pos_1 = (voltage_1 - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE);
    float pedal_pos_2 = (voltage_2 - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE);

    float avg_pedal_pos = (pedal_pos_1 + pedal_pos_2) / 2.0;

    motor_torque_demand = avg_pedal_pos * MAX_TORQUE;
    // ^^update member variable

    // Here to simplify this function and readability.
    update_implausibilities();
}

/*
    Timer based logic for the ETC to be rules compliant. MUST
    include rules T.4.2.4, T.4.2.9 (out of range).
*/
void ETCController::update_implausibilities() {
    // implaus logic checking:

    // retrieve the APPS sensor voltages..
    float APPS1_reading = myread_1.read();
    float APPS2_reading = myread_2.read();

    // T.4.2.9
    if (!in_range(APPS1_reading, 0.0, 1.0) || !in_range(APPS2_reading, 0.0, 1.0)) {
        motor_torque_demand = 0;
        implaus_T429 = true;
    } else {
        implaus_T429 = false;
    }

    Timer duration_implaus_ms; // in milliseconds

    // T.4.2.4
    if (std::abs(APPS1_reading - APPS2_reading) > 0.1) {
        
        if (!implaus_T424) { // implaus currently not active
            implaus_T424 = true;
            duration_implaus_ms.start();
        } else { // implaus currently active
            if ((duration_implaus_ms.elapsed_time().count() / 1000) > 100) {
                motor_torque_demand = 0;
            }
        }

    } else {
        implaus_T424 = false;
        // ^^ no currently active implaus
    }

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */

}

int main() {
    printf("Hello world!");

    return 0;

}
