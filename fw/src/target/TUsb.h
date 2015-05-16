/* -*- c++ -*- */
#pragma once

#define MIDI_EP_MAX_SIZE 32

#ifdef __cplusplus
#include "device.h"

/**
 * USB MIDI implementation. C code (usbmidi.c) is used to set up the
 * descriptors because struct initializations are more robust in
 * C. That adds a bit of unnecessary complexity -- it's bad enough already.
 */
class TUsb
{
public:
    TUsb() { }
    void Init();

    void Poll() {
        usbd_poll();
    }

    static void DataCallback(uint8_t ep);

private:
    static void SetConfig(uint16_t value);
};

#endif

#ifdef __cplusplus
extern "C" {
#endif
void UsbDataCallback(uint8_t ep);
#ifdef __cplusplus
};
#endif
