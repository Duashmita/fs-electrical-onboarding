#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

ETCController etc{PC_1, PC_2};

CAN can{PB_8, PB_9, 500000};
Ticker can_ticker;
volatile bool send_can_flag = false;

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

void on_can_timer() {
    send_can_flag = true;
}

void send_can_messages() {
    send_etc_CAN_messages();
    send_sme_CAN_messages();
}

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
    can_ticker.attach(&on_can_timer, 50ms);

    // No need to edit this while loop
    while (true) {
        // Must implement in etc/ subfolder
        etc.update_state();

        if (send_can_flag) {
            send_can_flag = false;
            send_can_messages();
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

    bool has_implaus = etc.implause_apps_out_of_range || etc.implaus_apps_deviation;

    if (!has_implaus) {
        data[0] |= (1 << 0);
        data[0] |= (1 << 1);
    }

    if (etc.implause_apps_out_of_range) {
        data[0] |= (1 << 4);
    }
    if (etc.implaus_apps_deviation) {
        data[0] |= (1 << 6);
    }

    CANMessage msg(403, data, 8);
    can.write(msg);
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    uint8_t data[8] = {0};
    int16_t torque = etc.torque_demand;

    data[0] = torque & 0xFF;
    data[1] = (torque >> 8) & 0xFF;

    CANMessage msg(390, data, 8);
    can.write(msg);
}
