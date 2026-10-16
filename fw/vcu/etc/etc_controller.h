//
// Created by Jackson Pinsonneault on 3/24/26.
//

#ifndef ETC_CONTROLLER_H
#define ETC_CONTROLLER_H

#include "mbed.h"

Timer implaus1_timer;
Timer implaus2_timer;
// Timer implaus3_timer;
// Timer implaus4_timer;

class ETCController {
public:

    /*
        Start of public variables
    */
   // list all variables used in etc_controller.cpp
   // make everything private and use getters and setters to be safe
    /*
        End of public variables
    */
   //getters
    bool getImplaus_1();
    bool getImplaus2();

    bool getGenFaulting();

    int16_t getCalculatedTorque();

    /*
        Public method definitions, don't edit
    */
    ETCController(PinName APPS1_pin, PinName APPS2_pin) {}; //constructor

    void update_state();

private:
    /*
        Start of private variables
    */
   
    //can message
    //each ellement of the array can hold 8 bits
        //does that mean each item has to be 8 bits or can it be less?
            //ie SG_ VCU_ETC_READY_TO_DRIVE : 0|1@1+ (1,0) [0|1] "" Vector__XXX
            //should this ^^^ be 1 item or should  it be grouped with other items?
            //SG_ VCU_BRAKE_PRESSSURE_FRONT : 16|16@1+ (1,0) [0|65535] "psi" Vector__XXX
            //^^ how do i break this up into seperate items?
    // uint8_t VCU_TPDO_STATUS[8] = {
    //     00000001, //ready to drive
    //     00000001, //    motor enabled
    //     00000000, //make void -  i think this skips this message
    //     00000000,//skip
    //     00000001, //apps out of range
    //     00000000, //skip
    //     00000001, //apps deviation
    //     00000000
    // }; //kinda correct but not really
    // also dont put in header file private bc it cant be accessed by send_can_mssgs



   //input variables
    float apps1_voltage = 0.0f;
    float apps2_voltage = 0.0f;

    //implaus variables
        //fault types
    bool apps1_out_of_range = false;
    bool apps2_out_of_range = false;
    bool pre_offset_apps_is_matching = false;
    bool post_offset_apps_is_matching = false;
    bool implaus_1 = false;
    bool implaus_2 = false;
    // bool implaus_3 = false;
    // bool implaus_4 = false;


    bool gen_faulting = false;
    bool gen_implaus = false;

    //pedal position variables
    float apps_1_volt_increment = 0.0069;
    int apps_1_pedal_position = 0;
    float apps_2_volt_increment = 0.00694;
    int apps_2_pedal_position = 0;
    float avg_pedal_position = 0;

    //torque variables
    int16_t torque_apps_1 = 0;
    int16_t torque_apps_2 = 0;
    int16_t calculated_torque = 0;


    void calc_torque(int apps_1_pedal_position, int apps_2_pedal_position);

    void apps_1_pedal_position_calc(int apps1_voltage , int apps_1_volt_increment);

    void apps_2_pedal_position_calc(int apps2_voltage , int apps_2_volt_increment);
    
    /*
        End of private variables
    */

    /*  
        !! IMPORTANT: All global voltages are based off     !!
        !! an older testing time. Still, voltages           !!
        !! should always be checked and updated if wrong.   !!
    */

    static constexpr float APPS1_MIN_VOLTAGE = 0.396; //constexpr makes evaluate at compile time not run time      
    static constexpr float APPS1_MAX_VOLTAGE = 1.086f;

    static constexpr float APPS2_MIN_VOLTAGE = 0.439f;
    static constexpr float APPS2_MAX_VOLTAGE = 1.133f;

    static constexpr float PEDAL_DEADZONE_PERCENTAGE = 3.0f;

    static constexpr int16_t MAX_TORQUE = 32767 * 0.1; // Multiplied by 0.1 for controlled 
                                                       // motor torque during onboarding.

    float clamp(float value , float min = 0, float max = 1);
//im confused on clamp and in_range. if the value is outside the range, should we clamp it?
    bool in_range(float value, float low, float high);

    void update_implausibilities();
};

#endif
