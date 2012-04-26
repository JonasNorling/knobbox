#include "TDisplay.h"
#include "stm32.h"

TCharacter NarrowFont[128] = { };

TDisplay::TDisplay()
  : BufferAllocMask(0)
{
  BufferAllocMask |= 1 << 0;
  TPageBuffer* buffer(GetBuffer());
  if (buffer) {
    buffer->Control[0] = 0xe2; // System reset
    buffer->Control[1] = 0xae; // Disable display
    EnqueueDmaJob(buffer, 0, 2);
  }
}

TDisplay::TPageBuffer* TDisplay::GetBuffer()
{
  // FIXME: Check BufferAllocMask for an unused buffer
  return 0;
}

void TDisplay::DmaFinished(void* context)
{
  // FIXME: Clear the right bit in BufferAllocMask
}

void TDisplay::Power(bool on)
{
  if (on) {
    // Leave sleep mode
    //Spi.Send(0xa4); // All pixels off
    //Spi.Send(0xae | on); // Enable on
  } else {
    // Enter sleep mode
    //Spi.Send(0xae | on); // Enable off
    //Spi.Send(0xa5); // All pixels on
  }
}

