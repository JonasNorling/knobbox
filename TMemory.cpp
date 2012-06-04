#include "TMemory.h"
#include "TSpiDmaJob.h"

static const uint8_t COMMAND_DEVICE_ID = 0x9f;

void TMemory::FetchBlock(uint8_t /*n*/)
{
  const uint8_t len = 1;
  CommandBuffer[0] = COMMAND_DEVICE_ID;

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, len),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CachedBlock, 5),
				 TSpiDmaJob::CS_FLASH, TSpiDmaJob::LCD_DATA,
				 this, 0));
}

void TMemory::DmaFinished(void* /*context*/)
{
}
