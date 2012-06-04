#include "TMemory.h"
#include "TSpiDmaJob.h"

static const uint8_t COMMAND_DEVICE_ID = 0x9f;
static const uint8_t COMMAND_READ_STATUS = 0x05;
static const uint8_t COMMAND_READ_ARRAY = 0x03;

void TMemory::FetchBlock(uint8_t page)
{
  /// \todo We're reading only the first 128 bytes here. Need to chain
  /// reads to get the whole block, and keep status of where we are.
  const uint16_t byteno = 0;
  CommandBuffer[0] = COMMAND_READ_ARRAY;
  CommandBuffer[1] = page >> 4;
  CommandBuffer[2] = (page << 4) | byteno >> 8;
  CommandBuffer[3] = byteno & 0xff;

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));
  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CachedBlock, 128),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));
  CachedBlockNo = page;
}

void TMemory::ReadStatus()
{
  /// \todo This will destroy CachedBlock, which is probably not what we want.
  CommandBuffer[3] = COMMAND_READ_STATUS;

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer + 3, 3),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));
  CachedBlockNo = BLOCK_STATUS_REG;
}

void TMemory::Identify()
{
  CommandBuffer[3] = COMMAND_DEVICE_ID;

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer + 3, 6),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));
  CachedBlockNo = BLOCK_DEVICE_ID;
}

void TMemory::DmaFinished(void* /*context*/)
{
}
