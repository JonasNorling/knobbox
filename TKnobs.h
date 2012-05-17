/* -*- c++ -*- */
#pragma once

#include <cstdint>

/**
 * Handle shift registers that output red/green LED current and read
 * encoders and push buttons.
 */
class TKnobs
{
public:
  static const uint8_t Knobs = 4;
  static const uint8_t ShiftLength = 2;

  TKnobs();
  void StartShifting();
  void Poll();

  /// Wanted intensity for each LED, 0..255
  uint8_t LedIntensity[2][Knobs];

private:
  uint8_t CurrentPwmStep;

  /// Data to shift out and in
  uint8_t LedControl[ShiftLength];
  uint8_t SwitchData[ShiftLength];

  struct TEncoderBits {
    uint16_t EncoderA;
    uint16_t EncoderB;
    uint16_t Button;
  };
  TEncoderBits LastEncoderData;
};

extern TKnobs Knobs;
