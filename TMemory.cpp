#include "TMemory.h"
#include "TSpiDmaJob.h"
#include "TGui.h"
#include <cstring>

static const uint8_t COMMAND_DEVICE_ID = 0x9f;
static const uint8_t COMMAND_READ_STATUS = 0x05;
static const uint8_t COMMAND_READ_ARRAY = 0x03;
static const uint8_t COMMAND_BLOCK_ERASE_4K = 0x20;
static const uint8_t COMMAND_PAGE_PROGRAM = 0x02;
static const uint8_t COMMAND_WRITE_ENABLE = 0x06;
static const uint8_t COMMAND_WRITE_DISABLE = 0x04;
static const uint8_t COMMAND_PROTECT_SECTOR = 0x36;
static const uint8_t COMMAND_UNPROTECT_SECTOR = 0x39;

static const uint8_t SREG_EPE = 1 << 5;
static const uint8_t SREG_WPP = 1 << 4;
static const uint8_t SREG_BSY = 1 << 0;

bool TMemory::FetchBlock(uint8_t n, IMemoryCallback* cb)
{
  if (CurrentOperation.Type != OP_NONE) return false;
  CurrentOperation.Type = OP_READ;
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

  bool ret = true;
  ret &= SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
					TSpiDmaJob::CS_FLASH,
					TSpiDmaJob::LCD_DATA,
					0,
					0));
  ret &= SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CachedBlock + byteno,
						DmaChunkSize),
					TSpiDmaJob::CS_FLASH,
					TSpiDmaJob::LCD_DATA,
					this,
					reinterpret_cast<void*>(moreToRead)));
  if (!ret) {
    // A hang here indicates that there aren't enough SPI DMA
    // buffers. That can't happen.
    while (true);
  }
}

bool TMemory::FetchNames(uint32_t startaddr, uint8_t len,
			 uint8_t count, uint32_t steplen,
			 IMemoryCallback* cb)
{
  if (CurrentOperation.Type != OP_NONE) return false;
  CurrentOperation.Type = OP_READ_NAMELIST;
  CurrentOperation.Callback = cb;

  ::memset(CachedBlock, 'x', sizeof(CachedBlock));
  TNameList* l = reinterpret_cast<TNameList*>(CachedBlock);
  l->StartAddress = startaddr;
  l->Len = len;
  l->Count = count;
  l->FetchedCount = 0;
  l->Steplen = steplen;
  l->Magic = TNameList::MAGIC;

  FetchNextName();

  return true;
}

void TMemory::FetchNextName()
{
  TNameList* l = reinterpret_cast<TNameList*>(CachedBlock);
  const uint32_t address = l->StartAddress +
    l->Steplen * l->FetchedCount;
  CommandBuffer[0] = COMMAND_READ_ARRAY;
  CommandBuffer[1] = (address >> 16) & 0xff;
  CommandBuffer[2] = (address >> 8)  & 0xff;
  CommandBuffer[3] = address & 0xff;

  const bool moreToRead = (l->FetchedCount + 1) < l->Count;

  bool ret = true;
  ret &= SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
					TSpiDmaJob::CS_FLASH,
					TSpiDmaJob::LCD_DATA,
					0,
					0));
  ret &= SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(
    reinterpret_cast<uint8_t*>(l->Names[l->FetchedCount]),
    l->Len),
					TSpiDmaJob::CS_FLASH,
					TSpiDmaJob::LCD_DATA,
					this,
					reinterpret_cast<void*>(moreToRead)));
  if (!ret) {
    // A hang here indicates that there aren't enough SPI DMA
    // buffers. That can't happen.
    while (true);
  }
}

bool TMemory::WriteBlock(uint8_t n, IMemoryCallback* cb)
{
  if (CurrentOperation.Type != OP_NONE) return false;
  CurrentOperation.Type = OP_WRITE;
  CurrentOperation.BlockNo = n;
  CurrentOperation.NextChunk = 0;
  CurrentOperation.State = TOperation::STATE_START_WRITE;
  CurrentOperation.Callback = cb;

  NudgeWriteMachine();
  return true;
}

void TMemory::NudgeWriteMachine()
{
  switch (CurrentOperation.State) {
  case TOperation::STATE_START_WRITE:
    ReadStatus();
    CurrentOperation.State = TOperation::STATE_WAIT_TO_UNPROTECT;
    break;
  case TOperation::STATE_WAIT_TO_UNPROTECT:
    // Status is in CommandBuffer[1..2]
    if ( CommandBuffer[1] & SREG_WPP &&
	~CommandBuffer[1] & SREG_BSY) {
      SendWriteEnable();
      CurrentOperation.State = TOperation::STATE_UNPROTECT_WE_SENT;
    }
    else {
      // Try again
      ReadStatus();
    }
    break;
  case TOperation::STATE_UNPROTECT_WE_SENT:
    SendUnprotectSector();
    CurrentOperation.State = TOperation::STATE_UNPROTECT_SENT;
    break;
  case TOperation::STATE_UNPROTECT_SENT:
    ReadStatus();
    CurrentOperation.State = TOperation::STATE_WAIT_TO_ERASE;
    break;
  case TOperation::STATE_WAIT_TO_ERASE:
    // Status is in CommandBuffer[1..2]
    if ( CommandBuffer[1] & SREG_WPP &&
	~CommandBuffer[1] & SREG_BSY) {
      SendWriteEnable();
      CurrentOperation.State = TOperation::STATE_ERASE_WE_SENT;
    }
    else {
      // Try again
      ReadStatus();
    }
    break;
  case TOperation::STATE_ERASE_WE_SENT:
    SendErasePage();
    CurrentOperation.State = TOperation::STATE_ERASE_SENT;
    break;
  case TOperation::STATE_ERASE_SENT:
    ReadStatus();
    CurrentOperation.State = TOperation::STATE_WAIT_TO_WRITE_DATA;
    break;
  case TOperation::STATE_WAIT_TO_WRITE_DATA:
    // Status is in CommandBuffer[1..2]
    if ( CommandBuffer[1] & SREG_WPP &&
	~CommandBuffer[1] & SREG_BSY) {
      SendWriteEnable();
      CurrentOperation.State = TOperation::STATE_CHUNK_WE_SENT;
    }
    else {
      // Try again
      ReadStatus();
    }
    break;
  case TOperation::STATE_CHUNK_WE_SENT:
    if (CurrentOperation.NextChunk * DmaChunkSize < BlockSize) {
      CurrentOperation.State = TOperation::STATE_CHUNK_SENT;
    }
    else {
      CurrentOperation.State = TOperation::STATE_LAST_CHUNK_SENT;
    }
    SendWriteData();
    break;
  case TOperation::STATE_CHUNK_SENT:
    ReadStatus();
    CurrentOperation.State = TOperation::STATE_WAIT_TO_WRITE_DATA;
    break;
  case TOperation::STATE_LAST_CHUNK_SENT:
    // We shouldn't be called for this!
    break;
  }
}

void TMemory::SendWriteEnable()
{
  CommandBuffer[0] = COMMAND_WRITE_ENABLE;
  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 1),
				 TSpiDmaJob::CS_FLASH,
				 TSpiDmaJob::LCD_DATA,
				 this,
				 reinterpret_cast<void*>(true)));
};

void TMemory::SendUnprotectSector()
{
  const uint8_t page = CurrentOperation.BlockNo;
  CommandBuffer[0] = COMMAND_UNPROTECT_SECTOR;
  CommandBuffer[1] = page >> 4;
  CommandBuffer[2] = page << 4;
  CommandBuffer[3] = 0;
  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
				 TSpiDmaJob::CS_FLASH,
				 TSpiDmaJob::LCD_DATA,
				 this,
				 reinterpret_cast<void*>(true)));
}

void TMemory::SendErasePage()
{
  const uint8_t page = CurrentOperation.BlockNo;
  CommandBuffer[0] = COMMAND_BLOCK_ERASE_4K;
  CommandBuffer[1] = page >> 4;
  CommandBuffer[2] = page << 4;
  CommandBuffer[3] = 0;
  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
				 TSpiDmaJob::CS_FLASH,
				 TSpiDmaJob::LCD_DATA,
				 this,
				 reinterpret_cast<void*>(true)));
}

void TMemory::SendWriteData()
{
  const uint8_t page = CurrentOperation.BlockNo;
  const uint16_t byteno = CurrentOperation.NextChunk * DmaChunkSize;
  CommandBuffer[0] = COMMAND_PAGE_PROGRAM;
  CommandBuffer[1] = page >> 4;
  CommandBuffer[2] = (page << 4) | byteno >> 8;
  CommandBuffer[3] = byteno & 0xff;

  CurrentOperation.NextChunk++;

  bool ret = true;
  ret &= SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
					TSpiDmaJob::CS_FLASH,
					TSpiDmaJob::LCD_DATA,
					0,
					0));
  ret &= SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CachedBlock + byteno,
						DmaChunkSize),
					TSpiDmaJob::CS_FLASH,
					TSpiDmaJob::LCD_DATA,
					this,
					reinterpret_cast<void*>(true)));
  if (!ret) {
    // A hang here indicates that there aren't enough SPI DMA
    // buffers. That can't happen.
    while (true);
  }
}

void TMemory::ReadStatus()
{
  CommandBuffer[0] = COMMAND_READ_STATUS;
  CommandBuffer[1] = 0;
  CommandBuffer[2] = 0;

  // Status register is read into CommandBuffer[1..2]

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 3),
				 TSpiDmaJob::CS_FLASH,
				 TSpiDmaJob::LCD_DATA,
				 this,
				 reinterpret_cast<void*>(true)));
}

void TMemory::Identify()
{
  CommandBuffer[0] = COMMAND_DEVICE_ID;
  CommandBuffer[1] = 0;
  CommandBuffer[2] = 0;
  CommandBuffer[3] = 0;

  // Device ID register is read into CommandBuffer[1..3]

  SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(CommandBuffer, 4),
				 TSpiDmaJob::CS_FLASH,
				 TSpiDmaJob::LCD_DATA,
				 this, 0));
}

void TMemory::DmaFinished(void* context)
{
  if (CurrentOperation.Type == OP_READ) {
    const bool moreToRead = static_cast<bool>(context);
    if (moreToRead) {
      FetchNextChunk();
    }
    else {
      if (CurrentOperation.Callback) {
	CurrentOperation.Callback->
	  MemoryOperationFinished(CurrentOperation.Type,
				  CurrentOperation.BlockNo);
	CurrentOperation.Callback = 0;
      }
      CurrentOperation.Type = OP_NONE;
    }
  }
  else if (CurrentOperation.Type == OP_WRITE) {
    if (CurrentOperation.State == TOperation::STATE_LAST_CHUNK_SENT) {
      if (CurrentOperation.Callback) {
	CurrentOperation.Callback->
	  MemoryOperationFinished(CurrentOperation.Type,
				  CurrentOperation.BlockNo);
	CurrentOperation.Callback = 0;
      }
      CurrentOperation.Type = OP_NONE;      
    }
    else {
      NudgeWriteMachine();
    }
  }
  else if (CurrentOperation.Type == OP_READ_NAMELIST) {
    TNameList* l = reinterpret_cast<TNameList*>(CachedBlock);
    l->FetchedCount++;
    const bool moreToRead = static_cast<bool>(context);
    if (moreToRead) {
      FetchNextName();
    }
    else {
      if (CurrentOperation.Callback) {
	CurrentOperation.Callback->
	  MemoryOperationFinished(CurrentOperation.Type, 0);
	CurrentOperation.Callback = 0;
      }
      CurrentOperation.Type = OP_NONE;
    }
  }
}
