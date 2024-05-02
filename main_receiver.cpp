/**
 * ENGR 029 LoRa Project
 * Copyright (c) 2024 Thomas Makin, Xezel Peshlakai
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "lcd_i2c.h"
#include "lorastack.h"
#include "trace_helper.h"

void rx_interrupt_handler(const uint8_t *payload, uint16_t size,
                                     int16_t rssi, int8_t snr) {
    std::string text[LCD_ROWS];
    char text_scan[LCD_ROWS][LCD_COLS];
    float temp = 0.0, humidity = 0.0, pressure = 0.0;

    printf("RECEIVED PKT\n");

    char _rx_payload[48];
    if (size > sizeof _rx_payload) {
        printf("ERROR! Packet too big.\n");
        return;
    }

    memcpy(_rx_payload, payload, size);

    sscanf(_rx_payload, "wthrstn1: <%f> <%f> <%f>\r\n", &temp, &humidity, &pressure);
    printf("%f %f %f\n", temp, humidity, pressure);

    snprintf(text_scan[0], LCD_COLS, "T:%.1fF H:%.1f%%", temp, humidity);
    snprintf(text_scan[1], LCD_COLS, "P:%.1fhPa", pressure);

    text[0] = std::string(text_scan[0]);
    text[1] = std::string(text_scan[1]);

    clearDisplay();
    writeLCD(text);
}

int main(void) {
    setup_trace();

    initDisplay();

    printf("\ncreating new lorastack!\n");

    radio_events_t radio_events;
    radio_events.rx_done =
        mbed::callback(&rx_interrupt_handler);

    LoRaStack *stack = new LoRaStack(radio_events);

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
