/**
 * ENGR 029 LoRa Project
 * Copyright (c) 2024 Thomas Makin, Xezel Peshlakai
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "BMP280.h"
#include "DHT.h"
#include "lorastack.h"
#include "trace_helper.h"

#define DHTPIN D3
#define DHTTYPE DHT11

Mutex write_mutex;

DHT temp_sensor(DHTPIN, DHT::DHT11);
BMP280 baro_sensor(D14, D15);

Thread thread;

volatile float temp = 0.0f, humidity = 0.0f, pressure = 0.0f;

void polling_thread()
{
    int val;
    while (true) {
        int err = temp_sensor.read();
        write_mutex.lock();
        if (err == DHT::SUCCESS) {
            temp = temp_sensor.getTemperature(DHT::FARENHEIT);
            humidity = temp_sensor.getHumidity();
        } else {
            printf("Error code : %d\r\n", err);
        }

        pressure = baro_sensor.getPressure();

        write_mutex.unlock();
        ThisThread::sleep_for(1min);
    }
}

int main(void) {
    setup_trace();

    baro_sensor.initialize();

    printf("\ncreating new lorastack!\n");

    LoRaStack *stack = new LoRaStack();

    // stack->setup_rx();

    stack->setup_tx();

    char sendBuf[128];

    while (1) {
        thread.start(polling_thread);

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
        printf("T: %.1f\r\n", temp);
        printf("H: %.1f\r\n", humidity);
        printf("P: %.1f\r\n", pressure);

        snprintf(sendBuf, 128, "%.1f,%.1f,%.1f", temp, humidity, pressure);
        stack->send(sendBuf);
        // stack->receive();

        ThisThread::sleep_for(10s);
    }
    return 0;
}
