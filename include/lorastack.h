/**
 * \file      lorastack.h
 *
 * \brief     Pure LoRa stack layer implementation
 *
 * \copyright Revised BSD License, see LICENSE.TXT file include in the project
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Thomas Makin
 * 
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \defgroup  Pure LoRa stack layer
 *
 * License: Revised BSD License, see LICENSE.TXT file include in the project
 *
 * Copyright (c) 2024, Thomas Makin
 * Copyright (c) 2017, Arm Limited and affiliates.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef LORASTACK_H_
#define LORASTACK_H_

#include <stdint.h>
#include "events/EventQueue.h"
#include "platform/mbed_atomic.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "platform/ScopedLock.h"

#include "lorastack/mac/LoRaMac.h"
#include "system/LoRaWANTimer.h"
#include "system/lorawan_data_structures.h"
#include "LoRaRadio.h"

class LoRaPHY;

/** LoRaStack Class
 * A controller layer for a Non-LoRaWan protocol impl
 */
class LoRaStack: private mbed::NonCopyable<LoRaStack> {

public:
    LoRaStack();

    uint8_t get_radio_status(void);
    void setup_tx(void);
    void setup_rx(void);
    void send(uint8_t *buf);
    void send_bs(void);
    void receive(void);
    void send_cont_wave(void);

    void lock(void)
    {
        _loramac.lock();
    }
    void unlock(void)
    {
        _loramac.unlock();
    }

private:
    typedef mbed::ScopedLock<LoRaStack> Lock;

    /**
     * TX interrupt handlers and corresponding processors
     */
    void tx_interrupt_handler(void);
    void tx_timeout_interrupt_handler(void);
    void process_transmission(void);
    void process_transmission_timeout(void);

    /**
     * RX interrupt handlers and corresponding processors
     */
    void rx_interrupt_handler(const uint8_t *payload, uint16_t size, int16_t rssi,
                              int8_t snr);
    void rx_timeout_interrupt_handler(void);
    void rx_error_interrupt_handler(void);
    void process_reception(const uint8_t *payload, uint16_t size, int16_t rssi,
                           int8_t snr);
    void process_reception_timeout(bool is_timeout);

private:
    LoRaMac _loramac;
    radio_events_t radio_events;
    device_states_t _device_current_state;
    LoRaRadio *_radio;
    LoRaPHY *_default_phy;
};

#endif /* LORASTACK_H_ */
