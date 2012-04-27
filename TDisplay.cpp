#include "TDisplay.h"
#include "TBitmask.h"
#include "stm32.h"
#include "logging.h"
#ifdef HOST
#include <cassert>
#else
#define assert(x)
#endif

static const uint8_t DOGM_SYSTEM_RESET = 0xe2;
static const uint8_t DOGM_DISABLE_DISPLAY = 0xae;
static const uint8_t DOGM_ENABLE_DISPLAY = 0xaf;
static const uint8_t DOGM_ALL_PIXELS_OFF = 0xa4;
static const uint8_t DOGM_ALL_PIXELS_ON = 0xa5;
static const uint8_t DOGM_SET_PAGE = 0xb0;
static const uint8_t DOGM_SET_COL_HIGH = 0x10; // FIXME
static const uint8_t DOGM_SET_COL_LOW = 0x00; // FIXME

TDisplay::TDisplay()
  : BufferAllocMask(TBitmask::Init(BufferCount))
{ }

void TDisplay::Init()
{
  TPageBuffer* buffer(GetBuffer());
  if (buffer) {
    buffer->Control[0] = DOGM_SYSTEM_RESET;
    buffer->Control[1] = DOGM_DISABLE_DISPLAY;
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
  buffer->Control[0] = DOGM_SET_PAGE | page;
  buffer->Control[1] = DOGM_SET_COL_HIGH | col >> 4;
  buffer->Control[2] = DOGM_SET_COL_LOW | (col & 0x0f);
  EnqueueDmaJob(buffer, length, 3);
}

void TDisplay::DmaFinished(void* context)
{
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
    buffer->Control[0] = DOGM_ALL_PIXELS_OFF;
    buffer->Control[1] = DOGM_ENABLE_DISPLAY;
  } else {
    // Enter sleep mode
    buffer->Control[0] = DOGM_DISABLE_DISPLAY;
    buffer->Control[1] = DOGM_ALL_PIXELS_ON;
  }
  EnqueueDmaJob(buffer, 0, 2);
  return true;
}


//#define FONT_LIQUID
#ifdef FONT_LIQUID
#include "liquid-2.0/fontliqsting.inc"
#else
#include "font.h"
#endif

uint8_t TDisplay::TPageBuffer::DrawText(const char* text, uint8_t offset)
{
  for (const char* pt = text; *pt != '\0'; pt++) {
#ifdef FONT_LIQUID
    glyph_id_t glyph = fontliqstingmono_obj.first_glyph((uint8_t)*pt);
    if (glyph) {
      for (int c = 0; c < 6; c++) {
	Data[offset++] |= fontliqstingmono_obj.glyph_data(glyph, c);
      }
    }
#else
    if ((uint8_t)*pt < FONT_GLYPHS) {
      for (int c = 0; c < 6; c++) {
	Data[offset++] |= Font[(uint8_t)*pt][c];
      }
    }
#endif

    else {
      for (int c = 0; c < 6; c++) {
	Data[offset++] = 0xff;
      }
    }
    assert(offset < Width);
  }

  return offset;
}
