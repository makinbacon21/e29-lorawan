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
