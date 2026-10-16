//
// Created by Jackson Pinsonneault on 3/24/26.
//

#include "etc_controller.h"

// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin) 
    : apps1(APPS1_pin), apps2(APPS2_pin){}

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

        APPS1_MIN_VOLTAGE = 0.396; 
        APPS1_MAX_VOLTAGE = 1.086f;

        pedal position = (given - 0.396)/(1.086-0.396) = btwn 0-1

        APPS2_MIN_VOLTAGE = 0.439f;
        APPS2_MAX_VOLTAGE = 1.133f; 

        how different can the readings be before its implausible

        percent  = (value-left shift)/range

        so the range will be min + d to max - d
        percent = (v-(min+d))/(max - min - 2d)

        (v - min -d)/max-min-2d
        
        throttle is based on adjusted percent

    */



    /*
        Finish the step listed above.
    */

    float apps1_reading = apps1.read();
    float apps2_reading = apps2.read();

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

//10% 

    /*
        End of implaus logic checking.
    */

    /*
        Check how long each implaus has been active for and
        update the actual implaus result if it's been active 
        for too long.
    */

    
}
