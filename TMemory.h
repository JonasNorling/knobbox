/* -*- c++ -*- */
#pragma once

/*
 * Thoughts about a memory map:
 * 512B pages
 *
 * page 0: production data
 * page 1: global settings
 *
 * pointer block?
 *
 * instrument setup
 *   Name[16],
 *   CCs: define names for all 0..127,
 *   NRPNs,
 *   Sysexes
 *
 * knob parameter map scenes
 *   Name[16]
 *   Instrument
 *   params[16]
 *
 * sequencer scenes
 *   Name[16]
 *   Instrument
 *   Key that this sequence is in
 *   Trigger key
 *
 */
