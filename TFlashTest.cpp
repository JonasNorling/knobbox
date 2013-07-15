#include "TFlashTest.h"
#include "TMemory.h"
#include "TDisplay.h"
#include "utils.h"
#include "TFlash.h"
#include "TGui.h"

#include <cstring>

const int len = 64;
static uint8_t data[len];
const int address = 0;

void TFlashTest::RunRead()
{
    ::memset(data, 0, len);
    Flash.Read(address, TBuffer(data, len), this);
}

void TFlashTest::RunWrite()
{
    for (int i = 0; i < len; i++) {
        data[i] = i;
    }
    Flash.Write(address, TBuffer(data, len), this);
}

void TFlashTest::FlashJobFinished(TFlashJob& job)
{
    State = 1;
    if (job.Operation == TFlashJob::OP_READ) {
        State = 2;
        if (job.Buffer.GetLength() == 64) {
            State = 3;

            int i = 0;
            for (; i < job.Buffer.GetLength(); i++) {
                if (job.Buffer.GetData()[i] != i) {
                    break;
                }
            }

            if (i == job.Buffer.GetLength()) {
                State = 4;
            }
        }
    }
    else if (job.Operation == TFlashJob::OP_WRITE) {
        State = 0xff;
    }
    Gui.UpdateAll();
}
