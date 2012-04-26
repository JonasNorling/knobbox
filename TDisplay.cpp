#include "TDisplay.h"
#include "TBitmask.h"
#include "stm32.h"
#include "logging.h"
#ifdef HOST
#include <cassert>
#else
#define assert(x)
#endif
#include "liquid-2.0/fontliqsting.inc"

TDisplay::TDisplay()
  : BufferAllocMask(TBitmask::Init(BufferCount))
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
  int8_t bit = TBitmask::FindFree(BufferAllocMask, BufferCount);
  if (bit == -1) {
    return 0;
  } else {
    return &Buffers[bit];
  }
}

void TDisplay::OutputBuffer(TPageBuffer* buffer, uint8_t length,
			    uint8_t page, uint8_t col)
{
#ifdef HOST
  for (int line = 0; line < 8; line++) {
    char s[1024] = {};
    for (int i = 0; i < length; i++) {
      s[i] = buffer->Data[i] & (1 << line) ? 'X' : ' ';
    }
    LOG("LCD: %s\n", s);
  }
#endif
  buffer->Control[0] = 0x00; // FIXME: page command
  buffer->Control[1] = 0x00; // FIXME: high col command
  buffer->Control[2] = 0x00; // FIXME: low col command
  EnqueueDmaJob(buffer, length, 3);
}

void TDisplay::DmaFinished(void* context)
{
  LOG("DMA finished\n");
  const uint8_t bufferid = reinterpret_cast<intptr_t>(context);
  TBitmask::Release(BufferAllocMask, bufferid);
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
  }

  return offset;
}
