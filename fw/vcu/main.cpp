#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

ETCController etc{PC_1, PC_2}; // APPS_1 = PC1 (pin 9), APPS_2 = PC2 (pin 10), from vcu schematic -y

CAN can{PB_8, PB_9, 500000};

// debug...ticker flags every 200ms, while loop prints -y
// print f isnt save in ticker because it runs an interupt -y

Ticker print_ticker;
volatile bool time_to_print = false;

void set_print_flag() {
    time_to_print = true; // only raise flag here no printing -y
}

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */


// debug... call set_print_flag every 200ms -y
print_ticker.attach(&set_print_flag, 200ms);

// added debiug print below for testing -y

    // No need to edit this while loop
    while (true) {
        // Must implement in etc/ subfolder
        etc.update_state();

    // debug.. if ticker flags, print once and lower it -y
    if (time_to_print) {
        time_to_print = false;
        printf("torque=%d range=%d dev=%d\n",
             etc.torque_demand, etc.apps_range_implaus, etc.apps_deviation_implaus);
    }
    }

    return 0;
}

/* 
    Must send the VCU_TPDO_STATUS CAN message with
    VCU_ETC_READY_TO_DRIVE & VCU_ETC_MOTOR_ENABLED
    set to 1, and VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE
    & VCU_ETC_IMPLAUS_APPS_DEVIATION properly added.
*/
void send_etc_CAN_messages() {}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {}
