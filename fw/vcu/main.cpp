#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

ETCController etc(PC_1, PC_2);

CAN can{PB_8, PB_9, 500000};

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

int main() {
    Ticker etc_CAN, sme_CAN;

    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */

    sme_CAN.attach(&send_etc_CAN_messages, 50ms);
    etc_CAN.attach(&send_etc_CAN_messages, 50ms);


    // No need to edit this while loop
    while (true) {
        // Must implement in etc/ subfolder
        etc.update_state();
    }

    return 0;
}

//target board is little endian

/* 
    Must send the VCU_TPDO_STATUS CAN message with
    VCU_ETC_READY_TO_DRIVE & VCU_ETC_MOTOR_ENABLED
    set to 1, and VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE
    & VCU_ETC_IMPLAUS_APPS_DEVIATION properly added.
*/
void send_etc_CAN_messages() {
    uint8_t msg_buff[8] = {1, 0, 0, 0};

    /*
        Bit packing:
        targ board is little endian, so is message:
        bit 0 (msg_buff[0] & 1 ) = VCU_ETC_READY_TO_DRIVE            = 1 (in msg_buff def)
        bit 1 (msg_buff[0] & 2 ) = VCU_ETC_MOTOR_ENABLED             = !etc.implausability_timeout
        bit 4 (msg_buff[0] & 8 ) = VCU_ETC_IMPLAUS_APPS_OUT_OF_RANGE = etc.implausability_timeout & etc.range_violation
        bit 6 (msg_buff[0] & 32) = VCU_ETC_IMPLAUS_APPS_DEVIATION    = etc.implausability_timeout & etc.alignment_violation
        
        everything lse is zeroed as it is out of scope of this project
    */
    msg_buff[0] |= !etc.implausability_timeout                            << 1;
    msg_buff[0] |= (etc.implausability_timeout & etc.range_violation    ) << 4;
    msg_buff[0] |= (etc.implausability_timeout & etc.alignment_violation) << 6;
    
    can.write(CANMessage(403, msg_buff, 8U));

}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the 
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    union {
        uint8_t u8[8];
        int16_t i16[4];
    } msg_buff = {0};
    
    /*
        Bit packing:
        targ board is little endian, so is message:
        bits 0-15 = SME_THROTL_TorqueDemand = etc.torque;

        everything lse is zeroed as it is out of scope of this project
            actually im assuming this ont work bc like SME_THROTL_MaxSpeed
            so lets do SME_THROTL_MaxSpeed = 32767
    */

    msg_buff.i16[0] = etc.torque;
    msg_buff.i16[1] = 32767;

    can.write(CANMessage(390, msg_buff.u8, 8U));
}
