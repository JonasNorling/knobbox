/* -*- c++ -*- */
#pragma once

inline static size_t cheap_strcpy(char* out, const char* in)
{
  size_t i = 0;
  for (; in[i] != '\0'; i++) out[i] = in[i];
  out[i] = '\0';
  return i;
}

inline static void render_uint(char* pt, uint32_t n, size_t fieldsize)
{
  uint32_t value = 1;
  for (; fieldsize > 0; fieldsize--, value*=10) {
    pt[fieldsize-1] = '0' + (n % (10*value)) / value;
  }
}
