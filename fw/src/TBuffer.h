/* -*- c++ -*- */
#pragma once

/**
 * Encapsulate a data buffer. No allocation is made in this class.
 */
class TBuffer
{
public:
    TBuffer() :
        Data(0), Length(0) {}
    TBuffer(uint8_t* data, int len) :
        Data(data), Length(len) {}

    uint8_t* GetData() const { return Data; }
    uint8_t GetLength() const { return Length; }

private:
    uint8_t* Data;
    int Length;
};
