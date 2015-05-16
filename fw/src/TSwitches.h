/* -*- c++ -*- */
#pragma once

#include <cstdint>

class TSwitches
{
public:
    TSwitches();
    void Poll();

private:
    void PollTacts();
    void PollEncoder();

    static const int LONG_PRESS_DELAY_MS = 1000;
    static const int SWITCH_COUNT = 5;

    uint8_t LastSwitchState;
    uint8_t LastEncoderState;
    uint32_t PressTime[SWITCH_COUNT];

};

extern TSwitches Switches;
