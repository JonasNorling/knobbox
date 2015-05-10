/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "TDisplayPageBase.h"

class TStartAnimation : public TDisplayPageBase
{
public:

    TStartAnimation() : State(0) { }

    void Show();
    void Render(uint8_t n, TDisplay::TPageBuffer* line);

private:
    int State;
};
