#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>
#include <cmath>

ETCController etc{PC_1, PC_2 };

CAN can{PB_8, PB_9, 500000};

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

Ticker my_ticker_1;
Ticker my_ticker_2;

bool send_etc = false;
bool send_sme = false;

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
//    //printf("Reached(main.cpp), Line 26");
    my_ticker_1.attach([]{send_etc = true;}, 50ms); //Sends 1st CAN message at 20Hz
    my_ticker_2.attach([]{send_sme = true;}, 50ms); //Sends 1st CAN message at 20Hz
    
//     //printf("Reached(main.cpp), Line 29");
//    my_ticker_2.attach(&send_sme_CAN_messages, 50ms); //Sends 2nd CAN message at 20Hz
 
    //printf("Reached(main.cpp), Line 33");

    // No need to edit this while loop
    while (true) {
        // Must implement in etc/ subfolder
        //printf("Reached(main.cpp), Line 38");
        etc.update_state();

        if (send_etc) {
            send_etc_CAN_messages();
            send_etc = false;
        }
        if (send_sme) {
            send_sme_CAN_messages();
            send_sme = false;
        }

        printf("PEDAL POSITION(1) = %.2f, PEDAL POSITION(2) = %.2f, MOTOR TORQUE DEMAND = %d\n", etc.get_Clamped_Pedal_Postion_1() * 100, etc.get_Clamped_Pedal_Postion_2() * 100,etc.get_Motor_Torque_Demand());
        // printf("VOLTAGE 1 = %fV, VOLTAGE 2 = %fV\n", etc.get_Voltage_1(), etc.get_Voltage_2());
        // printf("IMPLAUS =====  %d\n", etc.get_percent_implaus() && etc.get_range_implaus());
    }
    // //printf("Reached(main.cpp), Line 41");



    return 0;
}

/* 
    Must send the VCU_TPDO_STATUS CAN message with
    VCU_ETC_READY_TO_DRIVE & VCU_ETC_MOTOR_ENABLED
    set to 1, and VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE
    & VCU_ETC_IMPLAUS_APPS_DEVIATION properly added.
*/
void send_etc_CAN_messages() {
    //printf("Fail Happned, Line 52");

    uint8_t CAN_Message_1[8] = {0}; //8 byte Can Message Initialized to have 64 total bits, that are all 0's for now

    
    

    //printf("Reached(main.cpp), Line 59");
    bool Range_Implaus = etc.get_range_implaus(); //True if there is an implaus
    
    //printf("Fail Happneed, Line 62");
    bool Deviation_Implaus = etc.get_percent_implaus(); //True if there is an implaus
    
    //printf("Reached(main.cpp), Line 65");
    bool VCU_READY_TO_DRIVE = !Range_Implaus && !Deviation_Implaus; //Contains 1 or 0 indicating weather VCU is ready to drive, 1 if no implaus
    
    //printf("Reached(main.cpp), Line 68");
    CAN_Message_1[0] |= VCU_READY_TO_DRIVE; // Packng VCU_ETC_READY_TO_DRIVE into bit 0, which is set to 1
    
    //printf("Reached(main.cpp), Line 71");
    CAN_Message_1[0] |= (VCU_READY_TO_DRIVE << 1); // Packing VCU_ETC_MOTOR_ENABLED into bit 1, 0 or 1 if there is Implaus
    
    //printf("Reached(main.cpp), Line 74");
    CAN_Message_1[0] |= (Range_Implaus << 4); //Packing VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE into bit 4
    
    //printf("Reached(main.cpp), Line 77");
    CAN_Message_1[0] |= (Deviation_Implaus << 6); // Packing VCU_ETC_IMPLAUS_APPS_DEVIATION into bit 6
    
    //printf("Reached(main.cpp), Line 80");
    CANMessage msg(403, CAN_Message_1, 8); //Create can message
    

    //printf("Reached(main.cpp), Line 84");
    can.write(msg); //Write can message
    
    //printf("Reached(main.cpp), Line 87");
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    //printf("Reached(main.cpp), Line 96");
    uint8_t CAN_Message_2[8] = {0}; //8 byte Can Message Initialized to have 64 total bits, that are all 0's for now

    //printf("Reached(main.cpp), Line 99");
    int16_t Motor_Torque_Demand = etc.get_Motor_Torque_Demand();

    //printf("Reached(main.cpp), Line 102");
    CAN_Message_2[0] = Motor_Torque_Demand & 0xFF;
    //printf("Reached(main.cpp), Line 104");
    CAN_Message_2[1] = (Motor_Torque_Demand >> 8) & 0xFF;

    //printf("Reached(main.cpp), Line 107");
    CANMessage msg(390, CAN_Message_2, 8); //Create can message

    //printf("Reached(main.cpp), Line 110");
    can.write(msg); //Write can message

    //printf("Reached(main.cpp), Line 113");
}


