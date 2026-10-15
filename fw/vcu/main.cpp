#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>
#include <cmath>

ETCController etc{PC_1, PC_2 };

CAN can{PB_8, PB_9, 500000};

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();
void send_BOTH_CAN_messages();

Ticker my_ticker_1;
Ticker my_ticker_2;



int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
   my_ticker_1.attach(&send_etc_CAN_messages, 50ms); //Sends 1st CAN message at 20Hz
   my_ticker_2.attach(&send_sme_CAN_messages, 50ms); //Sends 2nd CAN message at 20Hz



    // No need to edit this while loop
    while (true) {
        // Must implement in etc/ subfolder
        etc.update_state();
    }

    return 0;
}

/* 
    Must send the VCU_TPDO_STATUS CAN message with
    VCU_ETC_READY_TO_DRIVE & VCU_ETC_MOTOR_ENABLED
    set to 1, and VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE
    & VCU_ETC_IMPLAUS_APPS_DEVIATION properly added.
*/
void send_etc_CAN_messages() {
    uint8_t CAN_Message_1[8] = {0}; //8 byte Can Message Initialized to have 64 total bits, that are all 0's for now

    
    

    
    bool Range_Implaus = etc.get_range_implaus();
    bool Deviation_Implaus = etc.get_percent_implaus();
    bool VCU_READY_TO_DRIVE = Range_Implaus && Deviation_Implaus; //Contains 1 or 0 indicating weather VCU is ready to drive

    CAN_Message_1[0] |= VCU_READY_TO_DRIVE; // Packng VCU_ETC_READY_TO_DRIVE into bit 0, which is set to 1

    
    CAN_Message_1[0] |= (VCU_READY_TO_DRIVE << 1); // Packing VCU_ETC_MOTOR_ENABLED into bit 1, 0 or 1 if there is Implaus

    CAN_Message_1[0] |= (Range_Implaus << 4); //Packing VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE into bit 3

    CAN_Message_1[0] |= (Deviation_Implaus << 6); // Packing VCU_ETC_IMPLAUS_APPS_DEVIATION into bit 6

    CANMessage msg(403, CAN_Message_1, 8); //Create can message

    can.write(msg); //Write can message

}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    uint8_t CAN_Message_2[8] = {0}; //8 byte Can Message Initialized to have 64 total bits, that are all 0's for now

    int16_t Motor_Torque_Demand = etc.get_Motor_Torque_Demand();

    CAN_Message_2[0] = Motor_Torque_Demand & 0xFF;
    CAN_Message_2[1] = (Motor_Torque_Demand >> 8) & 0xFF;

    CANMessage msg(390, CAN_Message_2, 8); //Create can message

    can.write(msg); //Write can message


}


