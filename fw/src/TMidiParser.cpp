/*
 * TMidiParser.cpp
 *
 *  Created on: Dec 21, 2012
 *      Author: jonas
 */
#include "TMidiParser.h"

TMidiParser::ParseResult TMidiParser::Feed(uint8_t data)
{
    if ((data & TMidiEvent::MIDI_REALTIME_MASK) == TMidiEvent::MIDI_REALTIME_MASK) {
        // Realtime event can interrupt the data stream, ignore them.
        RealtimeMessage = data;
        return GOT_REALTIME;
    }

    if (State == 0 && !(data & 0x80)) {
        // Running status -- reuse last status byte
        State++;
    }

    Event.Data[State] = data;
    State++;
    if (State == 3) {
        State = 0;
        return GOT_EVENT;
    }
    return NEED_MORE_FOOD;
}
