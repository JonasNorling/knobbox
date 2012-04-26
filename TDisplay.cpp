#include "TDisplay.h"
#include "stm32.h"
#define assert(x)
#include "liquid-2.0/fontliqsting.inc"

TDisplay::TDisplay()
  : BufferAllocMask(0)
{ }

void TDisplay::Init()
{
  TPageBuffer* buffer(GetBuffer());
  if (buffer) {
    buffer->Control[0] = 0xe2; // System reset
    buffer->Control[1] = 0xae; // Disable display
    EnqueueDmaJob(buffer, 0, 2);
  }
}

TDisplay::TPageBuffer* TDisplay::GetBuffer()
{
  // Check BufferAllocMask for an unused buffer
  for (uint8_t i = 0; i < BufferCount; i++) {
    const uint8_t bit = 1 << i;
    if ((BufferAllocMask & bit) == 0) {
      BufferAllocMask |= bit;
      return &Buffers[i];
    }
  }
  return 0;
}

void TDisplay::OutputBuffer(const TPageBuffer* buffer, uint8_t length,
			    uint8_t page, uint8_t col)
{
}

void TDisplay::DmaFinished(void* context)
{
  // Clear the right bit in BufferAllocMask
  const uint8_t bufferid = reinterpret_cast<uint32_t>(context);
  BufferAllocMask &= ~(1 << bufferid);
}

bool TDisplay::Power(bool on)
{
  TPageBuffer* buffer(GetBuffer());
  if (!buffer) {
    return false;
  }
  if (on) {
    // Leave sleep mode
    buffer->Control[0] = 0xa4; // All pixels off
    buffer->Control[1] = 0xaf; // Enable on
  } else {
    // Enter sleep mode
    buffer->Control[0] = 0xae; // Enable off
    buffer->Control[1] = 0xa5; // All pixels on
  }
  EnqueueDmaJob(buffer, 0, 2);
  return true;
}

uint8_t TDisplay::TPageBuffer::DrawText(const char* text, uint8_t offset)
{
  for (const char* pt = text; *pt != '\0'; pt++) {
    glyph_id_t glyph = fontliqstingmono_obj.first_glyph(*pt);
    if (glyph) {
      for (int c = 0; c < 6; c++) {
	Data[offset++] = fontliqstingmono_obj.glyph_data(glyph, c);
      }
    } else {
      for (int c = 0; c < 6; c++) {
	Data[offset++] = 0xff;
      }
    }
    Data[offset++] = 0x00;
  }

  return offset;
}
