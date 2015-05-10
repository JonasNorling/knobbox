/* -*- c++ -*- */
#pragma once

#include <cstdint>
#include "TBitmask.h"
#include "TDisplay.h"
#include "TEvent.h"
#include "TScheduler.h"

class TTopMenu
{
public:
    TTopMenu() { }
    void Render(uint8_t n, TDisplay::TPageBuffer* line, bool haveFocus);
    void Event(TEvent event);
};


/**
 * TGui keeps stuff together.
 */
class TGui
{
public:
    static const int Lines = 8;

    TGui();

    void Process();
    void UpdateAll() {
        TScheduler::Wake(SCHEDULER_TASK_GUI);
        DirtyLines = TBitmask::Init(Lines);
    }
    void UpdateLine(uint8_t n) {
        TScheduler::Wake(SCHEDULER_TASK_GUI);
        TBitmask::Set(DirtyLines, n);
    }
    void Event(TEvent event) {
        TScheduler::Wake(SCHEDULER_TASK_GUI);
        PendingEvent = event;
    }

    bool EventIsPending() const { return event_code(PendingEvent) != NO_EVENT; }
    bool HaveDirtyLines() const { return TBitmask::HaveSetBits(DirtyLines); }

    TEvent WaitForEvent();

    void Show() __attribute__((noreturn));

private:
    uint8_t DirtyLines;
    volatile TEvent PendingEvent;
};

extern TGui Gui;
