#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>

// Not ADC1_IN because Mbed OS maps by physical GPIO and pin
ETCController etc{PC_1, PC_2};

CAN can{PB_8, PB_9, 500000};

// Predefined for use in main()
void send_etc_CAN_messages();
void send_sme_CAN_messages();

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */

    // init tickers
    Ticker etc_can_ticker;
    Ticker sme_can_ticker;

    float frequency = 20.0f;
    float period = 1.0f/frequency;

    auto period_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::duration<float>(period)
    );

    // attach CAN send functions using period
    etc_can_ticker.attach(&send_etc_CAN_messages, period_duration);
    sme_can_ticker.attach(&send_sme_CAN_messages, period_duration);


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
    // create 8-byte buffer
    uint8_t payload[8] = {0};

    // packing the bits -> buffer
    payload[0] = (1 << 0) | (1 << 1);

    if (etc.implaus_apps_out_of_range) {
        payload[0] |= (1 << 4);
    }
    if (etc.implaus_apps_deviation) {
        payload[0] |= (1 << 6);
    }

    // construct & transmit the CAN message
    can.write(CANMessage(0x193, reinterpret_cast<const char*>(payload), 8));
}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages() {
    // 8-byte buffer
    uint8_t payload[8] = {0};


    // pack the bits...
    // using little-endian here (lower bytes first)
    payload[0] = static_cast<uint8_t>(etc.motor_torque_demand & 0xFF);
    payload[1] = static_cast<uint8_t>((etc.motor_torque_demand >> 8) & 0xFF);

    // transmit the CAN message
    can.write(CANMessage(0x186, reinterpret_cast<const char*>(payload), 8));
}
