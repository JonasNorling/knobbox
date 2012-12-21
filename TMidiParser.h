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
    TMidiParser() : State(0) {}

    /** Parse another byte. Return true if an event is ready to be read. */
    bool Feed(uint8_t data);

    const TMidiEvent& GetEvent() const { return Event; }

private:
    TMidiEvent Event;
    uint8_t State;
};

#endif /* TMIDIPARSER_H_ */
