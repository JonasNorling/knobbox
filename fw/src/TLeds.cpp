#include "device.h"
#include "TLeds.h"

const TPin* const TLeds::Leds[] = {
        &Pin_led_1,
        &Pin_led_2,
        &Pin_led_3,
        &Pin_led_4,
        &Pin_led_tp9,
        &Pin_led_tp16,
        &Pin_lcd_backlight,
        &Pin_led_enc1,
        &Pin_led_enc2
};

const uint8_t TLeds::ActiveLow[] = {
        1,
        1,
        1,
        1,
        0,
        0,
        0,
        0,
        0
};
