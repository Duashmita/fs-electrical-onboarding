#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

ETCController etc{PC_1, PC_2};

CAN can{PB_8, PB_9, 500000};

Thread can_thread{osPriorityAboveNormal};

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

void can_loop() {
    while (true) {
        send_etc_CAN_messages();
        send_sme_CAN_messages();

        ThisThread::sleep_for(50ms);
    }
}

Ticker print_ticker;
volatile bool print_flag = false;

void set_print_flag() {
    print_flag = true;
}

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
   can_thread.start(&can_loop);
   print_ticker.attach(&set_print_flag, 100ms);


    // No need to edit this while loop
    while (true) {
        etc.update_state();

        if (print_flag) {
            print_flag = false;
            printf("torque=%d  v1=%.3f  v2=%.3f\n",
                   etc.torque_demand, etc.apps1_voltage, etc.apps2_voltage);
        }

        // torque=0  v1=0.353  v2=0.395
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
    bool any_implausibility = etc.apps_implausibility || etc.out_of_range_implausibility;

    data[0] = (!any_implausibility << 0)
            | (!any_implausibility << 1)
            | (etc.out_of_range_implausibility << 4)
            | (etc.apps_implausibility << 6);

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