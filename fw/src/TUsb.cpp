#include "TUsb.h"
#include "TMidi.h"
#include "device.h"

extern "C" {
void UsbMidiInit();
};

void TUsb::Init()
{
#ifndef HOST
    UsbMidiInit();
#endif
}

#ifndef HOST

struct TUsbMidiEvent {
    uint8_t Code : 4;
    uint8_t Cable : 4;
    uint8_t Data[3];
} __attribute__((packed));

void TUsb::DataCallback(uint8_t ep __attribute__((unused)))
{
    TUsbMidiEvent buf[MIDI_EP_MAX_SIZE / sizeof(TUsbMidiEvent)];
    static_assert(sizeof(TUsbMidiEvent) == 4, "Strange");
    static_assert(sizeof(buf) == MIDI_EP_MAX_SIZE, "Buffer size is screwed");

    const int len = usbd_ep_read_packet(1, buf, MIDI_EP_MAX_SIZE);
    const int events = len / sizeof(TUsbMidiEvent);

    for (int i = 0; i < events; i++) {
        switch (buf[i].Code) {
        case 0x08: // note off
        case 0x09: // note on
        case 0x0e: // pitch bend
            Midi.EnqueueByte(buf[i].Data[0]);
            Midi.EnqueueByte(buf[i].Data[1]);
            Midi.EnqueueByte(buf[i].Data[2]);
            break;
        case 0x0b: // controller change
            Midi.EnqueueByte(buf[i].Data[0]);
            Midi.EnqueueByte(buf[i].Data[1]);
            Midi.EnqueueByte(buf[i].Data[2]);
            break;
        }
    }
}

void UsbDataCallback(uint8_t ep)
{
    TUsb::DataCallback(ep);
}

#endif
