#include "TMemory.h"
#include "TSpiDmaJob.h"

static const uint8_t COMMAND_DEVICE_ID = 0x9f;
static const uint8_t COMMAND_READ_STATUS = 0x05;
static const uint8_t COMMAND_READ_ARRAY = 0x03;

bool TMemory::FetchBlock(uint8_t n, IMemoryCallback* cb)
{
  if (CurrentOperation.State != TOperation::OP_NONE) return false;
  CurrentOperation.State = TOperation::OP_READ;
  CurrentOperation.BlockNo = n;
  CurrentOperation.NextChunk = 0;
  CurrentOperation.Callback = cb;

  FetchNextChunk();
  return true;
}

void TMemory::FetchNextChunk()
{
  const uint8_t page = CurrentOperation.BlockNo;
  const uint16_t byteno = CurrentOperation.NextChunk * DmaChunkSize;
  CommandBuffer[0] = COMMAND_READ_ARRAY;
  CommandBuffer[1] = page >> 4;
  CommandBuffer[2] = (page << 4) | byteno >> 8;
  CommandBuffer[3] = byteno & 0xff;

  CurrentOperation.NextChunk++;
  const bool moreToRead = CurrentOperation.NextChunk * DmaChunkSize < BlockSize;

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 0, 0));
  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CachedBlock, 128),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, reinterpret_cast<void*>(moreToRead)));
}

void TMemory::ReadStatus()
{
  /// \todo This will destroy CachedBlock, which is probably not what we want.
  CommandBuffer[3] = COMMAND_READ_STATUS;

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer + 3, 3),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));
  CurrentOperation.BlockNo = BLOCK_STATUS_REG;
}

void TMemory::Identify()
{
  CommandBuffer[3] = COMMAND_DEVICE_ID;

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer + 3, 6),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));
  CurrentOperation.BlockNo = BLOCK_DEVICE_ID;
}

void TMemory::DmaFinished(void* context)
{
  const bool moreToRead = static_cast<bool>(context);
  if (moreToRead) {
    FetchNextChunk();
  }
  else {
    if (CurrentOperation.Callback) {
      CurrentOperation.Callback->MemoryOperationFinished(CurrentOperation.BlockNo);
      CurrentOperation.Callback = 0;
    }
    CurrentOperation.State = TOperation::OP_NONE;
  }
}
