/*
 * TMidi.cpp
 *
 *  Created on: Dec 20, 2012
 *      Author: jonas
 */

#include "TMidi.h"

bool TMidi::EnqueueByte(uint8_t d)
{
    // Disable the interrupt when modifying the queue
    DisableTxInterrupt();
    bool status = OutQueue.Add(d);
    EnableTxInterrupt();
    return status;
}

void TMidi::SendEvent(const TMidiEvent& event)
{
    EnqueueByte(event.Data[0]);
    EnqueueByte(event.Data[1]);
    EnqueueByte(event.Data[2]);
}
