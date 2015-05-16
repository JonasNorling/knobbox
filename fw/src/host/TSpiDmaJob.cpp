#include "TSpiDmaJob.h"
#include <cstring>

TBlockingSpiDmaJob* TBlockingSpiDmaJob::CurrentJob = 0;

TBlockingSpiDmaJob::TBlockingSpiDmaJob(const uint8_t* out, uint8_t* in, int len)
: Out(out), In(in), Len(len), TaskId(0), Finished(false)
{
    assert(Len > 0);
    assert(Out);
}

void TBlockingSpiDmaJob::Run()
{
    TaskId = TScheduler::GetCurrentTaskId();
    CurrentJob = this;

    // FIXME: This is a hack that returns a valid flash device id
    // it this looks like such a transaction.
    if (In && Out && Len >= 3 && Out[0] == 0x9f) {
        In[0] = 0xff;
        In[1] = 0x1f;
        In[2] = 0x25;
    }
}

/*
 * *****************************************************
 */

TSpiDmaQueue::TSpiDmaQueue()
{
}

bool TSpiDmaQueue::Enqueue(const TSpiDmaJob& job)
{
    if (Jobs.Add(job)) {
        return true;
    } else {
        return false;
    }
}

bool TSpiDmaQueue::TryStartJob()
{
    return false;
}

void TSpiDmaJob::Finished() const
{
    if (Callback) {
        Callback->DmaFinished(Context);
    }
}
