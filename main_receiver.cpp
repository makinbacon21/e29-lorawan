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

    stack->setup_rx();

    t.start();
    while (1) {
        if((std::chrono::duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count() % 1000) == 0) {
            stack->receive();
        }
    }

    return 0;
}
