/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include <cstddef>

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
