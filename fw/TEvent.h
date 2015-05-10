/* -*- c++ -*- */
#pragma once

const uint8_t NO_EVENT      = 0;
const uint8_t KEY_BACK      = 1; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_UP        = 2; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_XXX       = 3; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_DOWN      = 4; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_OK        = 5; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_LONGPRESS_BACK = 6; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_LONGPRESS_UP   = 7; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_LONGPRESS_XXX  = 8; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_LONGPRESS_DOWN = 9; // Note that value is hard-coded in TSwitches.cpp
const uint8_t KEY_LONGPRESS_OK   = 10; // Note that value is hard-coded in TSwitches.cpp
const uint8_t BLINK_TIMER   = 11;
const uint8_t KNOB_RIGHT    = 12; // Value: knob number
const uint8_t KNOB_LEFT     = 13; // Value: knob number
const uint8_t KNOB_PUSH     = 14; // Value: knob number
const uint8_t RENDER_LINE   = 15; // Value: line number
const uint8_t MIDI_EVENT    = 16;

typedef uint32_t TEvent;

static inline TEvent construct_event(uint8_t code, uint32_t value) {
  return ((uint32_t)code << 24) | (value & 0xffffff);
}

static inline uint8_t event_code(TEvent event) {
  return event >> 24;
}

static inline uint8_t event_value(TEvent event) {
  return event & 0xffffff;
}
