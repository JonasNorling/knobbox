/*
 * TScheduler.cpp
 *
 *  Created on: Dec 2, 2012
 *      Author: jonas
 */

#include "device.h"
#include "TScheduler.h"
#include "logging.h"
#include <pthread.h>
#include <cstdio>

pthread_mutex_t TScheduler::Mutex;

void* createThread(void* context)
{
    void* (*entry)() = reinterpret_cast<void*(*)()>(context);
    pthread_mutex_lock(&TScheduler::Mutex);
    entry();
    return 0;
}

void TScheduler::Init()
{
    pthread_mutex_init(&Mutex, NULL);
    pthread_mutex_lock(&Mutex);

    for (int i = 0; i < SCHEDULER_NUM_TASKS; i++) {
        const TTask& task = Tasks[i];
        LOG("Creating task %s\n", task.Name);
        pthread_t pthread;
        // The new thread must try to lock the first thing it does -- create a wrapper function!
        pthread_create(&pthread, NULL, createThread, reinterpret_cast<void*>(task.EntryPoint));
    }

    // There is no implicit main task in the host implementation, so we just unlock
    // here and this thread can go on to do something else.
    pthread_mutex_unlock(&Mutex);
}

void TScheduler::Switch()
{
}

void TScheduler::Yield()
{
    pthread_mutex_unlock(&Mutex);
    pthread_mutex_lock(&Mutex);
}
