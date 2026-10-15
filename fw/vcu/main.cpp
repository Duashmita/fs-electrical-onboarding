#include "iostream"
#include "etc_controller.h"
#include "etc_controller.cpp"
#include "mbed.h"
#include <cstdio>

ETCController etc{PC_1 , PC_2};

// can{read_pin, transmit_pin, bus_frequency(hz)} 
CAN can{PB_8, PB_9, 500000};

// do the sme and etc send messages with the same can bus?

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages(); //just makes them exist to compiler

Ticker send_etc_CAN_messages_ticker;
Ticker send_sme_CAN_messages_ticker;

uint8_t vcu_tpdo_status_data[8] = {0};
//need to check if this is good spot to place

int main() {
    printf("Hello World!!\n");
    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
   send_etc_CAN_messages_ticker.attach(&send_etc_CAN_messages, 20ms);
   send_sme_CAN_messages_ticker.attach(&send_sme_CAN_messages, 20ms);


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
    vcu_tpdo_status_data[0] = (
        (1u << 0) | // ready to drive
        (1u << 1) | // etc motor enabled
        (static_cast<uint8_t>(etc.getImplaus_1()) << 4) | // implaus_apps_out_of_range
        (static_cast<uint8_t>(etc.getImplaus2())<< 6)
    );

    CANMessage vcu_tpdo_status_msg(403, vcu_tpdo_status_data, 8);
    if (can.write(vcu_tpdo_status_msg)) {
        printf("Sending ETC CAN messages\n");
    }
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    
    printf("Sending SME CAN messages\n");
}
