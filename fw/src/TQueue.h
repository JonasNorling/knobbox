/*
 * TQueue.h
 *
 *  Created on: Jul 14, 2013
 *      Author: jonas
 */

#pragma once

#include "TCircularBuffer.h"
#include "TScheduler.h"

/**
 * A queue connected to a task.
 */
template<typename T, int size>
class TQueue
{
public:
    TQueue(uint8_t taskId) : TaskId(taskId) { }

    bool Enqueue(const T& t) {
        bool res = Jobs.Add(t);
        if (res) {
            // We wake the task even if it is not explicitly waiting on this queue.
            // That means that a task always needs to check /why/ is was awakened.
            TScheduler::Wake(TaskId);
        }
        return res;
    }

    /**
     * Block waiting for an item to appear in the queue.
     */
    T Get() {
        while (Jobs.Empty()) {
            TScheduler::Block();
        }

        T t(Jobs.First());
        Jobs.Remove();
        return t;
    }

private:
    uint8_t TaskId;
    TCircularBuffer<T, size> Jobs;
};
