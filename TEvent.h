/* -*- c++ -*- */
#pragma once

const uint8_t NO_EVENT      = 0x00;
const uint8_t KEY_OK        = 0x10; // Note that value is hard-coded in main.cpp
const uint8_t KEY_UP        = 0x20; // Note that value is hard-coded in main.cpp
const uint8_t KEY_DOWN      = 0x30; // Note that value is hard-coded in main.cpp
const uint8_t KEY_BACK      = 0x40; // Note that value is hard-coded in main.cpp
const uint8_t KEY_LONGPRESS_OK   = 0x50; // Note that value is hard-coded in main.cpp
const uint8_t KEY_LONGPRESS_UP   = 0x60; // Note that value is hard-coded in main.cpp
const uint8_t KEY_LONGPRESS_DOWN = 0x70; // Note that value is hard-coded in main.cpp
const uint8_t KEY_LONGPRESS_BACK = 0x80; // Note that value is hard-coded in main.cpp
const uint8_t BLINK_TIMER   = 0x90;
const uint8_t KNOB_RIGHT    = 0xa0; // Value: knob number
const uint8_t KNOB_LEFT     = 0xb0; // Value: knob number
const uint8_t KNOB_PUSH     = 0xc0; // Value: knob number
const uint8_t RENDER_LINE   = 0xd0; // Value: line number

typedef uint8_t TEvent;

static inline TEvent construct_event(uint8_t code, uint8_t value) {
  return code | value;
}

static inline uint8_t event_code(TEvent event) {
  return event & 0xf0;
}

static inline uint8_t event_value(TEvent event) {
  return event & 0x0f;
}
