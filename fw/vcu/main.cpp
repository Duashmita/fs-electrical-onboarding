#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

ETCController etc{PC_1, PC_2 };

CAN can{PB_8, PB_9, 500000};

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();
void send_BOTH_CAN_messages();

Ticker my_ticker;


int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
   my_ticker.attach(&send_BOTH_CAN_messages, 50ms); //Sends both CAN message at 20Hz



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

    bool range_status = etc.get_range_implaus();
    bool percent_status = etc.get_percent_implaus();


    bool VCU_READY_TO_DRIVE = range_status && percent_status; //Contains 1 or 0 indicating weather VCU is ready to drive
    CAN_Message_1[0] = CAN_Message_1[0] | VCU_READY_TO_DRIVE; // Byte 1 = 0000 0001 or 0000 0000


}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {}



void send_BOTH_CAN_messages() {
    send_etc_CAN_messages();
    send_sme_CAN_messages();
}