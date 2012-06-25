/* -*- c++ -*- */
#pragma once

const uint8_t KEY_UP        = 0x10;
const uint8_t KEY_DOWN      = 0x20;
const uint8_t KEY_OK        = 0x30;
const uint8_t KEY_BACK      = 0x40;
const uint8_t RECEIVE_FOCUS = 0x50;
const uint8_t BLINK_TIMER   = 0x60;
const uint8_t KNOB_RIGHT    = 0x70;
const uint8_t KNOB_LEFT     = 0x80;

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
