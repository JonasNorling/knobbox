#include "TSwitches.h"
#include "device.h"
#include "TGui.h"

TSwitches::TSwitches()
:  LastSwitchState(0), LastEncoderState(0)
{
}

void TSwitches::Poll()
{
    PollTacts();
    PollEncoder();
}

void TSwitches::PollTacts()
{
    const uint16_t port = gpio_port_read(Pin_sw_1.Port);
    const uint8_t state =
            ((port & Pin_sw_5.Pin) ? 0x01 : 0x00) |
            ((port & Pin_sw_4.Pin) ? 0x02 : 0x00) |
            ((port & Pin_sw_3.Pin) ? 0x04 : 0x00) |
            ((port & Pin_sw_2.Pin) ? 0x08 : 0x00) |
            ((port & Pin_sw_1.Pin) ? 0x10 : 0x00);

    if (state ^ LastSwitchState) {
        // At least one button has changed state

        const uint16_t pressed = state & ~LastSwitchState;
        const uint16_t released = LastSwitchState & ~state;

        for (int i = 0; i < SWITCH_COUNT; i++) {
            if (pressed & (1 << i)) {
                PressTime[i] = SystemTime;
                Gui.Event(0x10 + 0x10 * i); // Must match key ids in TEvent.h
            }
            if (released & (1 << i)) {
                PressTime[i] = 0;
            }
        }
    }
    LastSwitchState = state;

    const int LONG_PRESS_DELAY_MS = 1000;
    for (int i = 0; i < SWITCH_COUNT; i++) {
        if (PressTime[i] != 0 && SystemTime > (PressTime[i] + LONG_PRESS_DELAY_MS)) {
            Gui.Event(0x60 + 0x10 * i); // Must match key ids in TEvent.h
            PressTime[i] = 0;
        }
    }
}

void TSwitches::PollEncoder()
{
    const uint8_t state =
            (Pin_enc_btn.Read() ? 0x00 : 0x01) |
            (Pin_enc_1.Read() ? 0x02 : 0x00) |
            (Pin_enc_2.Read() ? 0x04 : 0x00);

    const uint16_t edges = state ^ LastEncoderState;

    if ((edges & state) & 0x01) {
        // Button changed and went high
        Gui.Event(KEY_OK);
    }

    if (!(state & 0x04) && (edges & 0x02)) {
        if (state & 0x02) {
            Gui.Event(KEY_UP);
        }
        else {
            Gui.Event(KEY_DOWN);
        }
    }

    LastEncoderState = state;
}
