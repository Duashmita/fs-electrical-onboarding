#include "etc_controller.h"
#include "mbed.h"
#include <cstdio>
#include <chrono>

ETCController etc{PC_1, PC_2};

CAN can{PB_8, PB_9, 500000};

constexpr int STATUS_ID = 403;
constexpr int STATUS_DLC = 8; 
constexpr int THROTTLE_ID = 390; 
constexpr int THROTTLE_DLC = 8; 

Ticker can_ticker;
EventFlags can_flags;
Thread can_thread;

// Predefined for use in main()
void send_etc_CAN_messages(const ETCController::State &state);
void send_sme_CAN_messages(const ETCController::State &state);

void trigger_can() {
    can_flags.set(1);
}

void can_worker() {
    while (true) {
        can_flags.wait_any(1);

        const auto state = etc.get_state();
        send_etc_CAN_messages(state);
        send_sme_CAN_messages(state);
    }
}

int main() {
    printf("Hello World!!\n");

    /*
        Make send_CAN messages loop consistently
        at 20hz (20 messages / second)
    */

    etc.update_state();
    can_thread.start(can_worker);
    can_ticker.attach(
        &trigger_can,
        std::chrono::milliseconds(50)
    );


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
void send_etc_CAN_messages(const ETCController::State &state) {
    uint8_t data[STATUS_DLC] = {0};

    uint8_t ready_to_drive = 1;
    uint8_t motor_enabled = 1;

    uint8_t out_of_range = state.apps_out_of_range;
    uint8_t deviation = state.apps_deviation;

    // Pack each signal into the correct bit
    data[0] = (ready_to_drive)
            | (motor_enabled) << 1
            | (out_of_range) << 4
            | (deviation) << 6;

    CANMessage msg(
        STATUS_ID,
        data,
        STATUS_DLC
    );

    can.write(msg);

}

/*
    Must send the SME_RPDO_Throttle_Demand CAN
    message with SME_THROTL_TorqueDemand set to the
    torque demand determined by the ETC.
*/
void send_sme_CAN_messages(const ETCController::State &state) {
    uint8_t data[THROTTLE_DLC] = {0};

    int16_t torque = state.torque_demand;

    data[0] = torque & 0xFF;
    data[1] = (torque >> 8) & 0xFF;

    CANMessage msg(
        THROTTLE_ID,
        data,
        THROTTLE_DLC
    );

    can.write(msg);

}
