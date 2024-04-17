#include "lorastack.h"

#include "lorawan/lorastack/phy/LoRaPHYUS915.h"

#include "lora_radio_helper.h"

LoRaStack::LoRaStack() {
    _radio = &radio;

    printf("\nsetting rx config\n");
    radio.set_rx_config(MODEM_LORA, 1 /* 256 kHz */, 8 /* 256 bps */,
                        2 /* 4/6 */, 0 /* FSK only */, 0 /* 0 sym preamble */,
                        1024 /* 1024 symbol timeout */,
                        0 /* variable len packets */, 0 /* variable */,
                        0 /* CRC off */, 0 /* freq hopping off */,
                        0 /* 0 syms between hops */, 0 /* IQ not inverted */,
                        1 /* continuous rx */);

    printf("\nsetting tx config\n");
    radio.set_tx_config(MODEM_LORA, 30 /* 30 dBm/1 W power */, 0 /* fsk only */,
                        1 /* 256 kHz */, 8 /* 256 bps */, 2 /* 4/6 */,
                        0 /* 0 sym preamble */, 0 /* variable len packets */,
                        0 /* CRC off */, 0 /* freq hopping off */,
                        0 /* 0 syms between hops */, 0 /* IQ not inverted */,
                        100 /* 100 ms timeout */);

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

    _default_phy = new LoRaPHYUS915();

    cw_mode_params_t params = {
        /*!
        * The current channel index.
        */
        0,
        /*!
        * The datarate. Used to limit the TX power.
        */
        8,
        /*!
        * The TX power to set up.
        */
        30,
        /*!
        * The max EIRP, if applicable.
        */
        0,
        /*!
        * The antenna gain, if applicable.
        */
        0,
        /*!
        * Specifies the time the radio will stay in CW mode.
        */
        10000,
    };

    _default_phy->set_radio_instance(radio);
    _loramac.bind_phy(*_default_phy);

    radio.lock();
    // actual initialization of the radio driver with the radio_events_t
    radio.init_radio(&radio_events);
    radio.unlock();

    printf("\n radio initialized! \n");
    _default_phy->set_tx_cont_mode(&params, 902300000);
}

/*****************************************************************************
 * Interrupt handlers                                                        *
 ****************************************************************************/
void LoRaStack::tx_interrupt_handler(void) {
    // _tx_timestamp = _loramac.get_current_time();
    // const int ret = _queue->call(this, &LoRaStack::process_transmission);
    // MBED_ASSERT(ret != 0);
    // (void)ret;
}

void LoRaStack::rx_interrupt_handler(const uint8_t *payload, uint16_t size,
                                     int16_t rssi, int8_t snr) {
    // if (size > sizeof _rx_payload ||
    //     core_util_atomic_flag_test_and_set(&_rx_payload_in_use)) {
    //     return;
    // }

    // memcpy(_rx_payload, payload, size);

    // const uint8_t *ptr = _rx_payload;
    // const int ret =
    //     _queue->call(this, &LoRaStack::process_reception, ptr, size, rssi,
    //     snr);
    // MBED_ASSERT(ret != 0);
    // (void)ret;

    // TODO: check and fix payload formatter--maybe pad to int?
    printf("\n PAYLOAD RECEIVED: payload <0x%02x> size<%d> rssi <%d> snr<%d>\n",
           *payload, size, rssi, snr);
}

void LoRaStack::rx_error_interrupt_handler(void) {
    // const int ret =
    //     _queue->call(this, &LoRaStack::process_reception_timeout, false);
    // MBED_ASSERT(ret != 0);
    // (void)ret;
}

void LoRaStack::tx_timeout_interrupt_handler(void) {
    // const int ret =
    //     _queue->call(this, &LoRaStack::process_transmission_timeout);
    // MBED_ASSERT(ret != 0);
    // (void)ret;
}

void LoRaStack::rx_timeout_interrupt_handler(void) {
    // const int ret =
    //     _queue->call(this, &LoRaStack::process_reception_timeout, true);
    // MBED_ASSERT(ret != 0);
    // (void)ret;
}

uint8_t LoRaStack::get_radio_status(void) { return _radio->get_status(); }
