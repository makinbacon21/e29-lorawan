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

    stack->setup_rx();

    stack->receive();

    return 0;
}
