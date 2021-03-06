/* -*- c++ -*- */
#pragma once

#include <cstdint>

/**
 * The encoder data lines, push buttons, and LEDs are connected to
 * input and output shift registers. The shift registers are connected
 * to USART1, which is run in synchronous mode. The LEDs are pulse
 * width modulated to give 256 intensity steps.
 *
 * This class handles shift registers that output red/green LED
 * current and read encoders and push buttons.
 */
class TKnobs
{
public:
    /// Number of knobs really on this board
    static const uint8_t Knobs = 16;
    /// Number of bytes we need to shift for LEDs and encoders
    static const uint8_t ShiftLength = 6;
    enum Color {
        COLOR_RED = 0,
        COLOR_GREEN = 1
    };

    TKnobs();
    /// Called from an ISR when the DMA job has finished.
    void StartShifting();
    void Poll();

    /// Intensity for each LED, 0..255
    volatile uint8_t LedIntensity[2][Knobs];

    //private:
    uint8_t CurrentPwmStep;

    uint8_t LedControl[ShiftLength]; ///< Data to shift out
    uint8_t SwitchData[ShiftLength]; ///< Data to shift in

    /// Input shift data transformed for simpler computations.
    struct TEncoderBits {
        uint16_t EncoderA;
        uint16_t EncoderB;
        uint16_t Button;
    };
    TEncoderBits LastEncoderData;
};

extern TKnobs Knobs;
