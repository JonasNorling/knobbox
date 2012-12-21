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

void TMidi::SendEvent(uint8_t type, uint8_t arg1, uint8_t arg2)
{
    EnqueueByte(type);
    EnqueueByte(arg1);
    EnqueueByte(arg2);
}
