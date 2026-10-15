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

int main() {
    printf("Hello World!!\n");
    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
   send_etc_CAN_messages_ticker.attach(&send_etc_CAN_messages, 50ms);
   send_sme_CAN_messages_ticker.attach(&send_sme_CAN_messages, 50ms);


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
    uint8_t vcu_tpdo_status_data[8] = {0};

    vcu_tpdo_status_data[0] = (
        (1u << 0) | // ready to drive //should change to be based on implausabilities occuring
        (1u << 1) | // etc motor enabled
        (etc.getImplaus_1() << 4) | // implaus_apps_out_of_range
        (etc.getImplaus2() << 6)
    );

    CANMessage vcu_tpdo_status_msg(403, vcu_tpdo_status_data, 8);
    can.write(vcu_tpdo_status_msg);
    printf("Sending ETC CAN messages\n");
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    uint8_t sme_rpdo_throttle_demand_data[8] = {0};
    
    sme_rpdo_throttle_demand_data[0] = (
        etc.getCalculatedTorque() & 0xFF
    );

    sme_rpdo_throttle_demand_data[1] = (
        (etc.getCalculatedTorque() >> 8) & 0xFF
    );
// shift and get first 8 bits of 16 bit mssg as first element
//2nd element is the last 8 bits of mssg
    // add data
    CANMessage sme_rpdo_throttle_demand_msg(390, sme_rpdo_throttle_demand_data, 8);
    can.write(sme_rpdo_throttle_demand_msg);
    printf("Sending SME CAN messages\n");
}
