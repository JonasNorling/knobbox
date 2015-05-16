/*
 * TMidiParser.h
 *
 *  Created on: Dec 21, 2012
 *      Author: jonas
 */

#ifndef TMIDIPARSER_H_
#define TMIDIPARSER_H_

#include <cstdint>
#include "TMidi.h"

class TMidiParser
{
public:
    enum ParseResult {
        NEED_MORE_FOOD,
        GOT_REALTIME,
        GOT_EVENT
    };

    TMidiParser() : State(0), RealtimeMessage(0) {}

    /** Parse another byte. Return true if an event is ready to be read. */
    ParseResult Feed(uint8_t data);

    const TMidiEvent& GetEvent() const { return Event; }
    uint8_t GetRealtimeMessage() const { return RealtimeMessage; }

private:
    TMidiEvent Event;
    uint8_t State;
    uint8_t RealtimeMessage;
};

#endif /* TMIDIPARSER_H_ */
