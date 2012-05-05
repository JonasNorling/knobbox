/* -*- c++ -*- */
#pragma once

class TBuffer
{
public:
  TBuffer() :
    Data(0), Length(0) {}
  TBuffer(uint8_t* data, uint8_t len) :
    Data(data), Length(len) {}

  uint8_t* GetData() const { return Data; }
  uint8_t GetLength() const { return Length; }

  //start, end
  //xor, slice

private:
  uint8_t* Data;
  uint8_t Length;
};
