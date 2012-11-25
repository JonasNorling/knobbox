#include "TFlashTest.h"
#include "TMemory.h"
#include "TDisplay.h"
#include "utils.h"

#include <cstring>

static const int Blockno = 65;

void TFlashTest::RunRead()
{
  State = 0;
  uint8_t data = '?';
  ::memset(Memory.GetCachedBlock(), data, TMemory::BlockSize);
  Memory.FetchBlock(Blockno, this);
}

void TFlashTest::RunWrite()
{
  State = 1;
  uint8_t data = 'Q';
  ::memset(Memory.GetCachedBlock(), data, TMemory::BlockSize);
  Memory.WriteBlock(Blockno, this);
}

void TFlashTest::MemoryOperationFinished(TMemory::OperationType type, uint8_t block)
{
  switch (State) {
  case 0: {
    int len = 0;
    TDisplay::TPageBuffer* line = 0;

    line = Display.GetBuffer();
    if (line) {
        line->Clear();
        len = line->DrawText("Finished", 0);
        Display.OutputBuffer(line, len, 4, 0);
    }

    int right = 0;
    for (unsigned i = 0; i < TMemory::BlockSize; i++) {
        if (Memory.GetCachedBlock()[i] == 'Q') right++;
    }

    char text[20];
    cheap_strcpy(text, "Correct: xxxx");
    render_uint(text+9, right, 4);

    line = Display.GetBuffer();
    if (line) {
        line->Clear();
        len = line->DrawText(text, 0);
        Display.OutputBuffer(line, len, 5, 0);
    }
    break;
  }
  case 1: {
    int len = 0;
    TDisplay::TPageBuffer* line = 0;

    line = Display.GetBuffer();
    if (line) {
        line->Clear();
        len = line->DrawText("Wrote.", 0);
        Display.OutputBuffer(line, len, 4, 0);
    }
  }
  }
}
