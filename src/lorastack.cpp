/**
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech
 ___ _____ _   ___ _  _____ ___  ___  ___ ___
/ __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
\__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
|___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
embedded.connectivity.solutions===============

Description: Pure LoRa stack layer based loosely off mbed-os/connectivity/lorawan

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Thomas Makin


Copyright (c) 2017, Arm Limited and affiliates.
Copyright (c) 2024, Thomas Makin

SPDX-License-Identifier: BSD-3-Clause
*/


#include "lorastack.h"

#include "lora_radio_helper.h"

LoRaStack::LoRaStack() {
    _radio = &radio;

    printf("\nsetting up radio events\n");
    // setting up callbakcs in radio_events_t
    radio_events.tx_done =
        mbed::callback(this, &LoRaStack::tx_interrupt_handler);
    radio_events.rx_done =
        mbed::callback(this, &LoRaStack::rx_interrupt_handler);
    radio_events.rx_error =
        mbed::callback(this, &LoRaStack::rx_error_interrupt_handler);
    radio_events.tx_timeout =
        mbed::callback(this, &LoRaStack::tx_timeout_interrupt_handler);
    radio_events.rx_timeout =
        mbed::callback(this, &LoRaStack::rx_timeout_interrupt_handler);

    radio.lock();
    // actual initialization of the radio driver with the radio_events_t
    radio.init_radio(&radio_events);
    radio.unlock();

    printf("\n radio initialized! \n");
}

LoRaStack::LoRaStack(radio_events_t radio_events) {
    _radio = &radio;

    this->radio_events = radio_events;

    printf("\nsetting up radio events\n");

    radio.lock();
    // actual initialization of the radio driver with the radio_events_t
    radio.init_radio(&(this->radio_events));
    radio.unlock();

    printf("\n radio initialized! \n");
}

/*****************************************************************************
 * Interrupt handlers                                                        *
 ****************************************************************************/
void LoRaStack::tx_interrupt_handler(void) {
    printf("TX DONE\n");
}

void LoRaStack::rx_interrupt_handler(const uint8_t *payload, uint16_t size,
                                     int16_t rssi, int8_t snr) {
    printf("RX_RECEIVED: rssi <%d> snr <%d>\n", rssi, snr);
    char _rx_payload[48];
    if (size > sizeof _rx_payload) {
        printf("ERROR! Packet too big.\n");
        return;
    }

    memcpy(_rx_payload, payload, size);

    printf("%s\n", _rx_payload);
}

void LoRaStack::rx_error_interrupt_handler(void) {
    printf("RX ERROR\n");
}

void LoRaStack::tx_timeout_interrupt_handler(void) {
    printf("TX TIMED OUT\n");
}

void LoRaStack::rx_timeout_interrupt_handler(void) {
    // printf("RX TIMED OUT\n");
}

uint8_t LoRaStack::get_radio_status(void) { return _radio->get_status(); }

void LoRaStack::setup_tx(void) {
    printf("\nsetting tx config\n");
    _radio->set_channel(915000000);
    _radio->set_tx_config(MODEM_LORA, 20 /* 20 dBm/1 W power */, 0 /* fsk only */,
                        0 /* 125 kHz */, 9 /* 512 chirps */, 4 /* 4/8 */,
                        8 /* 8 sym preamble */, false /* var len packets */,
                        true /* CRC on */, 0 /* freq hopping off */,
                        0 /* 0 syms between hops */, false /* IQ not inverted */,
                        3000 /* 3000 ms timeout */);
    _radio->set_max_payload_length(MODEM_LORA, 48);
}

void LoRaStack::setup_rx(void) {
    printf("\nsetting rx config\n");
    _radio->set_channel(915000000);
    _radio->set_rx_config(MODEM_LORA, 0 /* 125 kHz */, 9 /* 512 chirps */,
                        4 /* 4/8 */, 0 /* FSK only */, 8 /* 8 sym preamble */,
                        1024 /* 1024 symbol timeout */,
                        false /* var len packets */, 0 /* n/a */,
                        true /* CRC on */, 0 /* freq hopping off */,
                        0 /* 0 syms between hops */, false /* IQ not inverted */,
                        1 /* continuous rx */);
    _radio->set_max_payload_length(MODEM_LORA, 48);
}

void LoRaStack::send(char *buf) {
    uint8_t buffer[48];
    int i;
    for(i=0; i<48; i++) {
        buffer[i] = buf[i];
        if(buf[i] == '\0')
            break;
    }
    _radio->send(buffer, i);
}

void LoRaStack::send_bs(void) { 
    uint8_t buf[8] = "urmom!!";
    _radio->send(buf, 8);
}

void LoRaStack::receive(void) {
    _radio->receive();
}

void LoRaStack::send_cont_wave(void) {
    _radio->set_tx_continuous_wave(915000000, TX_POWER_5, 0.5);
}
