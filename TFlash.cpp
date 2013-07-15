#include "TFlash.h"
/*
 * TFlash.cpp
 *
 *  Created on: Jul 14, 2013
 *      Author: jonas
 *
 * Driver for AT45DB081D flash.
 * A big part of the logic mess in here is about exposing a continous array even
 * though the device's page size is 264 bytes.
 */

#include "utils.h"
#include "device.h"
#include "TSpiDmaJob.h"
#include "TScheduler.h"
#include "TBuffer.h"
#include "TFlash.h"
#include <cstring>

static const uint8_t COMMAND_READ_ARRAY = 0x03;
static const uint8_t COMMAND_DEVICE_ID = 0x9f;
static const uint8_t COMMAND_READ_STATUS = 0xd7;
static const uint8_t COMMAND_FLASH_TO_BUFFER1 = 0x53;
static const uint8_t COMMAND_BUFFER1_WRITE = 0x84;
static const uint8_t COMMAND_BUFFER1_PROGRAM_ERASE = 0x83;

static const uint8_t STATUS_RDY = 0x80;

static const int PAGE_SIZE = 256;

// Operations to pick apart a linear address to an AT45 page+byte address
#define AT45_PAGE_ADDR(a) ((a) >> 8)
#define AT45_BYTE_ADDR(a) ((a) & 0xff)
// 264-byte mode addresses always look like this:
#define AT45_ADDR(page, byte) (((page) << 9) | ((byte) & 0x1ff))

/*
 * ********************************************************
 */

TFlash::TFlash() : IsInited(false), JobQueue(SCHEDULER_TASK_FLASH)
{
}

void TFlash::FlashTask()
{
    Init();

    while (true) {
        TFlashJob job(JobQueue.Get());
        switch (job.Operation) {
        case TFlashJob::OP_READ:
            DoRead(job);
            break;
        case TFlashJob::OP_WRITE:
            DoWrite(job);
            break;
        }
        job.Callback->FlashJobFinished(job);
    }
}

bool TFlash::Read(uint32_t address, const TBuffer& buffer, IFlashCallback* callback)
{
    TFlashJob job;
    job.Operation = TFlashJob::OP_READ;
    job.Address = address;
    job.Buffer = buffer;
    job.Callback = callback;
    return JobQueue.Enqueue(job);
}

bool TFlash::Write(uint32_t address, const TBuffer& buffer, IFlashCallback* callback)
{
    TFlashJob job;
    job.Operation = TFlashJob::OP_WRITE;
    job.Address = address;
    job.Buffer = buffer;
    job.Callback = callback;
    return JobQueue.Enqueue(job);
}

void TFlash::SetCS(bool state)
{
    if (state) {
        Pin_flash_cs.Clear();
    }
    else {
        Pin_flash_cs.Set();
    }
}

void TFlash::Init()
{
    // Device ID register is read into buffer[1..3]
    uint8_t buffer[4];
    ::memset(buffer, 0, sizeof(buffer));
    buffer[0] = COMMAND_DEVICE_ID;
    SetCS(true);
    TBlockingSpiDmaJob spijob(TBuffer(buffer, sizeof(buffer)));
    spijob.Run();
    SetCS(false);

    assert(buffer[1] == 0x1f);
    assert(buffer[2] == 0x25);
    IsInited = true;
}

uint8_t TFlash::ReadStatus()
{
    uint8_t buffer[2];
    ::memset(buffer, 0, sizeof(buffer));
    buffer[0] = COMMAND_READ_STATUS;
    SetCS(true);
    TBlockingSpiDmaJob spijob(TBuffer(buffer, sizeof(buffer)));
    spijob.Run();
    SetCS(false);
    return buffer[1];
}

void TFlash::DoRead(TFlashJob& job)
{
    uint8_t buffer[4];

    uint32_t a = job.Address;
    uint8_t* data = job.Buffer.GetData();
    int remaininglen = job.Buffer.GetLength();

    while (remaininglen > 0) {
        uint16_t pageAddr = AT45_PAGE_ADDR(a);
        uint16_t byteAddr = AT45_BYTE_ADDR(a);
        uint32_t at45address = AT45_ADDR(pageAddr, byteAddr);
        const int readlen = std::min(remaininglen, PAGE_SIZE - byteAddr);

        buffer[0] = COMMAND_READ_ARRAY;
        buffer[1] = (at45address >> 16) & 0xff;
        buffer[2] = (at45address >> 8) & 0xff;
        buffer[3] = at45address & 0xff;

        SetCS(true);
        {
            TBlockingSpiDmaJob spijob(TBuffer(buffer, 4));
            spijob.Run();
        }
        {
            TBlockingSpiDmaJob spijob(TBuffer(data, readlen));
            spijob.Run();
        }
        SetCS(false);

        data += readlen;
        a += readlen;
        remaininglen -= readlen;
    }
}

void TFlash::DoWrite(TFlashJob& job)
{
    uint8_t buffer[4];

    uint32_t a = job.Address;
    uint8_t* data = job.Buffer.GetData();
    int remaininglen = job.Buffer.GetLength();

    while (remaininglen > 0) {
        while (!(ReadStatus() & STATUS_RDY));

        uint16_t pageAddr = AT45_PAGE_ADDR(a);
        uint16_t byteAddr = AT45_BYTE_ADDR(a);
        uint32_t at45address = AT45_ADDR(pageAddr, byteAddr);
        const int writelen = std::min(remaininglen, PAGE_SIZE - byteAddr);

        // Read flash array to the buffer 1
        buffer[0] = COMMAND_FLASH_TO_BUFFER1;
        buffer[1] = (at45address >> 16) & 0xff;
        buffer[2] = (at45address >> 8) & 0xff;
        buffer[3] = at45address & 0xff;

        SetCS(true);
        {
            TBlockingSpiDmaJob spijob(TBuffer(buffer, 4));
            spijob.Run();
        }
        SetCS(false);

        while (!(ReadStatus() & STATUS_RDY));

        // Load the new data into buffer 1
        buffer[0] = COMMAND_BUFFER1_WRITE;
        buffer[1] = (at45address >> 16) & 0xff;
        buffer[2] = (at45address >> 8) & 0xff;
        buffer[3] = at45address & 0xff;

        SetCS(true);
        {
            TBlockingSpiDmaJob spijob(TBuffer(buffer, 4));
            spijob.Run();
        }
        {
            TBlockingSpiDmaJob spijob(TBuffer(data, writelen));
            spijob.Run();
        }
        SetCS(false);
        __asm__("nop");

        // Erase+program the buffer into the flash array
        buffer[0] = COMMAND_BUFFER1_PROGRAM_ERASE;
        buffer[1] = (at45address >> 16) & 0xff;
        buffer[2] = (at45address >> 8) & 0xff;
        buffer[3] = at45address & 0xff;

        SetCS(true);
        {
            TBlockingSpiDmaJob spijob(TBuffer(buffer, 4));
            spijob.Run();
        }
        SetCS(false);

        while (!(ReadStatus() & STATUS_RDY));

        data += writelen;
        a += writelen;
        remaininglen -= writelen;
    }
}
