//
// Created by Jackson Pinsonneault on 3/24/26.
//
// Adding something to Test Git Committ :)
// Adding something else to ensure everything is fine!


#include "etc_controller.h"
#include <cmath>


// Assign appropriate GPIO objects depending on pin parameters
ETCController::ETCController(PinName APPS1_pin, PinName APPS2_pin)

    : APPS_sensor_1(APPS1_pin), //Initialize AnalogIn Pin so we can read their voltages later when we need them
    APPS_sensor_2(APPS2_pin)
{

}  







/* 
    Nice function to write which returns a wrapped version of the
    value given between 0 and 1 (-0.1 => 0, 10 => 1, 0.3 => 0.3).
*/
float ETCController::clamp(float value) {
    if( value < 0.0){
        return 0.0;
    }else if( value > 1.0){
        return 1.0;
    }else{
        return value;
    }
}





/*
    Simple function to write which returns a bool of true or false
    depending on whether low <= value <= high is true.
*/
bool ETCController::in_range(float value, float low, float high) {
    return( value >= low && value <= high );
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
    // //printf("Reached(etc_controller.cpp), Line 69, Problem with reading voltages");
    APPS1_Voltage = APPS_sensor_1.read() * 3.3; //Read the percentage from 0.0 to 1.0 -> convert it to a voltage
    // //printf("Reached(etc_controller.cpp), Line 71");
    APPS2_Voltage = APPS_sensor_2.read() * 3.3;

    //Now determine the pedal postions

    

    // //printf("Reached(etc_controller.cpp), Line 78, Problem with Pedal Position");
    //Now determine the pedal postions
    APPS1_Pedal_Position = ( APPS1_Voltage - APPS1_MIN_VOLTAGE) / (APPS1_MAX_VOLTAGE - APPS1_MIN_VOLTAGE  ); //Determine Pedal Position Percentage
    // //printf("Reached(etc_controller.cpp), Line 81");

    APPS2_Pedal_Position = ( APPS2_Voltage - APPS2_MIN_VOLTAGE) / (APPS2_MAX_VOLTAGE - APPS2_MIN_VOLTAGE  ); //Determine Pedal Position Percentage

    //printf("Reached(etc_controller.cpp), Line 85, Problems with CLaming Pedal Position");
    //Clamp Pedal position percentages to be from 0% to 100%, i.e. 0.0 to 1.0
    Clamped_APPS1_Pedal_Position = clamp( APPS1_Pedal_Position - PEDAL_DEADZONE_PERCENTAGE/(1.0 - 2.0 * PEDAL_DEADZONE_PERCENTAGE));// Subtract by DeadZone percentage to adjust to our "new" 0% 
    //printf("Reached(etc_controller.cpp), Line 88");

    Clamped_APPS2_Pedal_Position = clamp( APPS2_Pedal_Position - PEDAL_DEADZONE_PERCENTAGE/(1.0 - 2.0 * PEDAL_DEADZONE_PERCENTAGE)); // Then divide by 1-PEDAL_DEADZONE_PERCENTAGE to get our "new" 100% pedal position

    //printf("Reached(etc_controller.cpp), Line 92, Torque Demand");
    //Calculate a motor torque demanad
    Motor_Torque_Demand = ((Clamped_APPS1_Pedal_Position + Clamped_APPS2_Pedal_Position) / 2.0f) * (MAX_TORQUE);


    
        

    /*
        Finish the step listed above.
    */

    //printf("Reached(etc_controller.cpp), Line 104");

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
    
    

    //Check if Volatages corresponding to APPS1 and APPS2 are valid!!!!!


    //printf("Reached(etc_controller.cpp), Line 124");
    int64_t duration_Range_Implaus = 0; //Timer For Range Implaus
    //printf("Reached(etc_controller.cpp), Line 126");
    int64_t duration_Percent_Implaus = 0; //Timer for Percent Implaus

    //printf("Reached(etc_controller.cpp), Line 129");
    bool APPS_Sensors_Range_Implaus = !(in_range(APPS1_Voltage, APPS1_MIN_VOLTAGE, APPS1_MAX_VOLTAGE) && in_range(APPS2_Voltage, APPS2_MIN_VOLTAGE, APPS2_MAX_VOLTAGE));
    //Determines weather the APPS Sensors are in Valid Voltage Range, if in range sets it to false, 
    
    //printf("Reached(etc_controller.cpp), Line 133, Logic for starting timer for implaus");
    if (APPS_Sensors_Range_Implaus) { //If there is a Range Implaus,start the timer to see how long it is occuring
        //printf("Reached(etc_controller.cpp), Line 135");
        Timer_Range_Implaus.start(); //Start Timer
        
        //printf("Reached(etc_controller.cpp), Line 138");    
        duration_Range_Implaus = Timer_Range_Implaus.elapsed_time().count() / 1000;  
        //Record Time that has passed since timer was initially started 
        //printf("Reached(etc_controller.cpp), Line 141");
        

    }
    //printf("Reached(etc_controller.cpp), Line 145, Before stopping and reseting timer");
    if(!APPS_Sensors_Range_Implaus){
        //printf("Reached(etc_controller.cpp), Line 147");
        Timer_Range_Implaus.stop();
        //printf("Reached(etc_controller.cpp), Line 149");
        Timer_Range_Implaus.reset();
    }
    
    //printf("Reached(etc_controller.cpp), Line 153, before conforming APPS_RANGE_Implaus");
    if( duration_Range_Implaus > 100){
        //printf("Reached(etc_controller.cpp), Line 155");
        APPS_Range_Implausibility_Confirmed = true;
    }



    //Check if two pedal postions are within 10%
    //Then time for 100msec and if problem persists, stop power

    
    //printf("Reached(etc_controller.cpp), Line 165");
    float Percentage_Difference = std::abs(APPS1_Pedal_Position - APPS2_Pedal_Position); 
    //printf("Reached(etc_controller.cpp), Line 167");
    if( Percentage_Difference <= 0.10){ //There is no Implaus
        //printf("Reached(etc_controller.cpp), Line 169");

        Timer_Percent_Implaus.stop();
        //printf("Reached(etc_controller.cpp), Line 172");

        Timer_Percent_Implaus.reset();
    }else{
        //printf("Reached(etc_controller.cpp), Line 176");

        Timer_Percent_Implaus.start(); //There is an implaus so start timer

        //printf("Reached(etc_controller.cpp), Line 180");

        duration_Percent_Implaus = Timer_Percent_Implaus.elapsed_time().count() / 1000;
        //printf("Reached(etc_controller.cpp), Line 183");
        if( duration_Percent_Implaus > 100){
            APPS_Percent_Implausibility_Confirmed = true;
            //printf("Reached(etc_controller.cpp), Line 1");
        }
        
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
