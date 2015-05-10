#pragma once

class TLeds
{
public:
    enum Led {
        LED_1,
        LED_2,
        LED_3,
        LED_4,
        LED_TP9,
        LED_TP16,
        LED_LCD_BL,
        LED_ENC_RED,
        LED_ENC_GREEN,
    };

    static void Set(Led led, bool state) {
        if (static_cast<uint8_t>(state) ^ ActiveLow[led]) {
            Leds[led]->Set();
        }
        else {
            Leds[led]->Clear();
        }
    }

private:
    static const TPin* const Leds[];
    static const uint8_t ActiveLow[];
};
