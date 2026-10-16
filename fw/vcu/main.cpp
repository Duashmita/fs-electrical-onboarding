#include "PinNames.h"
#include "cmsis_os2.h"
#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

ETCController etc{PC_1, PC_2}; //APPS1 pin: 9, APPS2 pin: 10. Corresponds to PC_1 and PC_2.

CAN can{PB_8, PB_9, 500000};

constexpr auto CAN_SENDING_PERIOD = 50ms;

Ticker can_ticker;
volatile bool can_send_due = false;

void mark_can_send_due() {
    can_send_due = true;
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

    can_ticker.attach(&mark_can_send_due, CAN_SENDING_PERIOD);

    // No need to edit this while loop
    while (true) {
        // Must implement in etc/ subfolder
        etc.update_state();

        if (can_send_due) {
            can_send_due = false;
            send_etc_CAN_messages();
            send_sme_CAN_messages();
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
void send_etc_CAN_messages() {
    uint8_t data[8] = {0};

    data[0] |= (1 << 0); 
    data[0] |= (1 << 1);  
    data[0] |= (etc.apps_out_of_range_implausible ? 1 : 0) << 4; 
    data[0] |= (etc.apps_deviation_implausible ? 1 : 0) << 6;    

    can.write(CANMessage(403, data, 8));
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    uint16_t torque = static_cast<uint16_t>(etc.torque_demand);

    uint8_t data[8] = {0};

    data[0] = torque & 0xFF;         
    data[1] = (torque >> 8) & 0xFF;

    can.write(CANMessage(390, data, 8));

}