#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

ETCController etc{PC_1, PC_2};

CAN can{PB_8, PB_9, 500000};

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

void send_messages() {
    send_etc_CAN_messages();
    send_sme_CAN_messages();
}

Ticker myTicker;

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
   
   myTicker.attach(&send_messages, 50ms);
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
    uint8_t vcu_tpdo_status[8] = {0};

    // sets bit 0 and bit 1 to 1
    vcu_tpdo_status[0] |= etc.READY_TO_DRIVE << 0;
    vcu_tpdo_status[0] |= etc.READY_TO_DRIVE << 1;


    vcu_tpdo_status[0] |= etc.out_of_range_implaus << 4;
    vcu_tpdo_status[0] |= etc.deviation_implaus << 6;


    // set bits 4 and 6 to 0 if no implaus detected.
    /*
    if (etc.deviation_implaus || etc.out_of_range_implaus) {
        vcu_tpdo_status[0] &= ~(1 << 4);
        vcu_tpdo_status[0] &= ~(1 << 6);

    // sets bits 4 and 6 to 1 if implaus detected.
    } else {
        vcu_tpdo_status[0] |= 1 << 4;
        vcu_tpdo_status[0] |= 1 << 6;
    }
    */

    // create and write CAN message
    CANMessage status_msg{403, vcu_tpdo_status, 8};
    can.write(status_msg);
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    uint8_t sme_rdpo_throttle_demand[8] = {0};

    // first byte of demand message is set to the Least Significant Byte of 16-bit calculation
    sme_rdpo_throttle_demand[0] = etc.calculated_torque & 0xFF;

    // second byte of demand message is set to Most Significant Byte of 16-bit calculation
    sme_rdpo_throttle_demand[1] = (etc.calculated_torque >> 8) & 0xFF;

    // create and write CAN message
    CANMessage status_msg{390, sme_rdpo_throttle_demand, 8};
    can.write(status_msg);
}

/*
    CAN NODE - has CAN tranceiver that feeds its output into the MCU.
                - tranceiver converts from "CAN logic" into Digital Logic.

    CAN Bus - it's a twisted pair of wires for CAN High & CAN Low.

    When a digital 0 is sent, both wires remain at the same recessive voltage.

    When a digital 1 is sent, both wires go to their dominant voltage (CAN High and CAN low).

    The difference between the two voltages is the information that we are trying to transmit.

    CAN Arbitration - allows for packet-prioritazation with no overhead

*/
