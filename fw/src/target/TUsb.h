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
        usbd_poll(Device);
    }

    static void DataCallback(uint8_t ep);

private:
#ifndef HOST
    static usbd_device* Device;
#endif
    static void SetConfig(uint16_t value);
};

extern TUsb Usb;

#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifndef HOST
void UsbDataCallback(usbd_device *usbd_dev, uint8_t ep);
#endif
#ifdef __cplusplus
};
#endif
