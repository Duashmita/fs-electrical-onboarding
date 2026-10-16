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
    auto next = Kernel::Clock::now();
    while (true) {
        send_etc_CAN_messages();
        send_sme_CAN_messages();
        next += 50ms;
        ThisThread::sleep_until(next);
    }
}

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */
   can_thread.start(can_loop);


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
    uint8_t data[8] = {0};
    data[0] |= (1 << 0);
    data[0] |= (1 << 1);
    if (etc.out_of_range_implausibility) {
        data[0] |= (1 << 4);
    }
    if (etc.apps_implausibility) {
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
