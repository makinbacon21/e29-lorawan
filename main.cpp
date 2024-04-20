/**
 * ENGR 029 LoRa Project
 * Copyright (c) 2024 Thomas Makin
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "lorastack.h"
#include "trace_helper.h"

int main(void) {
    setup_trace();

    printf("\ncreating new lorastack!\n");

    LoRaStack *stack = new LoRaStack();

    while (1) {
        // printf("STATUS: ");
        // switch (stack->get_radio_status()) {
        // case 0:
        //     printf("IDLE\n");
        //     break;
        // case 1:
        //     printf("RX_RUNNING\n");
        //     break;
        // case 2:
        //     printf("TX_RUNNING\n");
        //     break;
        // }

        stack->send_cont_wave(); //0.5s

        ThisThread::sleep_for(1s);
    }
    return 0;
}
