/* -*- c++ -*- */
#pragma once

#include "TMidi.h"

enum EEventCode {
    NO_EVENT,
    KEY_BACK      = 1, // Note that value is hard-coded in TSwitches.cpp
    KEY_UP        = 2, // Note that value is hard-coded in TSwitches.cpp
    KEY_XXX       = 3, // Note that value is hard-coded in TSwitches.cpp
    KEY_DOWN      = 4, // Note that value is hard-coded in TSwitches.cpp
    KEY_OK        = 5, // Note that value is hard-coded in TSwitches.cpp
    KEY_LONGPRESS_BACK = 6, // Note that value is hard-coded in TSwitches.cpp
    KEY_LONGPRESS_UP   = 7, // Note that value is hard-coded in TSwitches.cpp
    KEY_LONGPRESS_XXX  = 8, // Note that value is hard-coded in TSwitches.cpp
    KEY_LONGPRESS_DOWN = 9, // Note that value is hard-coded in TSwitches.cpp
    KEY_LONGPRESS_OK   = 10, // Note that value is hard-coded in TSwitches.cpp
    BLINK_TIMER,
    KNOB_RIGHT, // Value: knob number
    KNOB_LEFT, // Value: knob number
    KNOB_PUSH, // Value: knob number
    RENDER_LINE, // Value: line number
    MIDI_EVENT
};

class TEvent
{
public:
    TEvent(EEventCode code) : Code(code) {}

    EEventCode Code;
    union {
        uint8_t byte;
        TMidiEvent midi;
    } Value;
};

static inline TEvent construct_event(EEventCode code, uint8_t value) {
  TEvent e(code);
  e.Value.byte = value;
  return e;
}

static inline uint8_t event_code(const TEvent& event) {
    return event.Code;
}

static inline uint8_t event_value(const TEvent& event) {
    return event.Value.byte;
}
