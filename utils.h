/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include <cstddef>
#include <algorithm>

/**
 * \file utils.h
 * Some cheap libc alternatives.
 */

/**
 * Copy string until \0.
 * \todo Consider not making inline.
 */
inline static size_t cheap_strcpy(char* out, const char* in)
{
  size_t i = 0;
  for (; in[i] != '\0'; i++) out[i] = in[i];
  out[i] = '\0';
  return i;
}

/**
 * Render a number to a string.
 * \todo Consider not making inline.
 */
inline static void render_uint(char* pt, uint32_t n, size_t fieldsize)
{
  uint32_t value = 1;
  for (; fieldsize > 0; fieldsize--, value*=10) {
    pt[fieldsize-1] = '0' + (n % (10*value)) / value;
  }
}

inline static void render_hexbyte(char* pt, uint8_t n)
{
  pt[0] = (n >> 4) < 0xa ? (n >> 4) + '0' : (n >> 4 ) - 0xa + 'a';
  pt[1] = (n & 0xf) < 0xa ? (n & 0xf) + '0' : (n & 0xf) - 0xa + 'a';
}

/**
 * Note duration, in 48ths, as 3 chars.
 */
inline static void render_note_time(char* pt, uint8_t n)
{
  if ((n / 48) > 0) {
    pt[0] = '\022' + n / 48;
  }
  else {
    pt[0] = '\'';
  }

  render_uint(pt+1, n % 48, 2);
}

inline static void render_signed_note_time(char* pt, int8_t n)
{
  pt[0] = n < 0 ? '-' : ' ';
  render_note_time(pt+1, abs(n));
}

inline static int clamp(int v, int min, int max)
{
  return std::min(std::max(v, min), max);
}
