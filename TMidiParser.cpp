/*
 * TMidiParser.cpp
 *
 *  Created on: Dec 21, 2012
 *      Author: jonas
 */
#include "TMidiParser.h"

bool TMidiParser::Feed(uint8_t data)
{
    if ((data & 0xf8) == TMidiEvent::MIDI_REALTIME) {
        // Realtime event can interrupt the data stream, ignore them.
        return false;
    }

    Event.Data[State] = data;
    State++;
    if (State == 3) {
        State = 0;
        return true;
    }
    return false;
}
