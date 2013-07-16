#include "TMemory.h"
#include "TScheduler.h"

bool TMemory::ReadBlocking(uint32_t address, const TBuffer& buffer)
{
    JobFinished = false;
    bool res = Flash.Read(address, buffer, this);
    if (res) {
        TaskId = TScheduler::GetCurrentTaskId();
        TScheduler::BlockUntil(&JobFinished);
    }
    return res;
}

void TMemory::FlashJobFinished(TFlashJob& job)
{
    JobFinished = true;
    TScheduler::Wake(TaskId);
}
