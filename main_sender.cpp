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

#define PRELUDE "wthrstn1"

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

    LowPowerTimer t;
    long current_time = 0, last_time = 0;

    stack->setup_tx();

    char sendBuf[48];

    thread.start(polling_thread);
    t.start();
    while (1) {
        // allow sleeping by timing with lowpowertimer
        current_time = std::chrono::duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count();
        if((current_time - last_time) >= 10000) {
            snprintf(sendBuf, 48, "%s: <%.1f> <%.1f> <%.1f>\r\n", PRELUDE, temp, humidity, pressure);
            stack->send(sendBuf);
            last_time = current_time;
        }
    }
    return 0;
}
