/* -*- c++ -*- */
#pragma once

#include <cstdint>

struct TMidiEvent;

class IMidi
{
public:
    virtual void SendClockTick() = 0;
    virtual void SendEvent(const TMidiEvent& event) = 0;
};
