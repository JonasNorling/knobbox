/*
 * TFlash.h
 *
 *  Created on: Jul 14, 2013
 *      Author: jonas
 */

#pragma once

#include <stdint.h>
#include "TBuffer.h"
#include "TQueue.h"

class TFlashJob;

class IFlashCallback
{
public:
    virtual void FlashJobFinished(TFlashJob& job) = 0;
};

class TFlashJob
{
public:
    enum OperationType {
        OP_READ,
        OP_WRITE
    };

    OperationType Operation;
    uint32_t Address;
    TBuffer Buffer;
    IFlashCallback* Callback;
};

class TFlash
{
public:
    TFlash();
    void FlashTask() __attribute__((noreturn));
    bool Inited() { return IsInited; }

    bool Read(uint32_t address, const TBuffer& buffer, IFlashCallback* callback);
    bool Write(uint32_t address, const TBuffer& buffer, IFlashCallback* callback);

private:
    bool IsInited;
    TQueue<TFlashJob, 2> JobQueue;

    void Init();

    void SetCS(bool state);
    uint8_t ReadStatus();
    void DoRead(TFlashJob& job);
    void DoWrite(TFlashJob& job);
};

extern TFlash Flash;
