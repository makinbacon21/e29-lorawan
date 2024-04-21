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

    stack->setup_rx();

    //stack->setup_tx();

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

        // stack->send_cont_wave(); //0.5s
        //stack->send_bs();
        stack->receive();

        ThisThread::sleep_for(3s);
    }
    return 0;
}
