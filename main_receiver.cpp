/**
 * ENGR 029 LoRa Project
 * Copyright (c) 2024 Thomas Makin, Xezel Peshlakai
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "lorastack.h"
#include "trace_helper.h"

int main(void) {
    setup_trace();

    printf("\ncreating new lorastack!\n");

    LoRaStack *stack = new LoRaStack();

    LowPowerTimer t;
    long current_time = 0, last_time = 0;

    stack->setup_rx();

    t.start();
    while (1) {
        current_time = std::chrono::duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count();
        if((current_time - last_time) >= 500) {
            stack->receive();
            last_time = current_time;
        }
    }

    return 0;
}
