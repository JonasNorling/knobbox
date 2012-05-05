#include "TDisplay.h"
#include "TBitmask.h"
#include "stm32.h"
#include "logging.h"
#ifdef HOST
#include <cassert>
#else
#define assert(x)
#endif

static const uint8_t DOGM_SYSTEM_RESET =          0xe2;
static const uint8_t DOGM_POWER_CONTROL =         0x28;
static const uint8_t DOGM_SET_ELECTRONIC_VOLUME = 0x81;
static const uint8_t DOGM_SEG_NORMAL =            0xa0;
static const uint8_t DOGM_SEG_REVERSE =           0xa1;
static const uint8_t DOGM_ALL_PIXELS_OFF =        0xa4;
static const uint8_t DOGM_ALL_PIXELS_ON =         0xa5;
static const uint8_t DOGM_INVERSE_OFF =           0xa6;
static const uint8_t DOGM_INVERSE_ON =            0xa7;
static const uint8_t DOGM_DISABLE_DISPLAY =       0xae;
static const uint8_t DOGM_ENABLE_DISPLAY =        0xaf;
static const uint8_t DOGM_COM_NORMAL =            0xc0;
static const uint8_t DOGM_COM_REVERSE =           0xc8;
static const uint8_t DOGM_SET_PAGE =              0xb0;
static const uint8_t DOGM_SET_COL_HIGH =          0x10;
static const uint8_t DOGM_SET_COL_LOW =           0x00;

TDisplay::TDisplay() :
  BufferAllocMask(TBitmask::Init(BufferCount))
{
}

void TDisplay::Init()
{
  // Reset
#ifndef HOST
  gpio_clear(GPIOA, GPIO0);
  delay_ms(5);
  gpio_set(GPIOA, GPIO0);
  delay_ms(5);
#endif
  TPageBuffer* buffer(GetBuffer());
  if (buffer) {
    const int len = 8;
    buffer->Data[0] = DOGM_SYSTEM_RESET;
    buffer->Data[1] = DOGM_COM_NORMAL;
    buffer->Data[2] = DOGM_SEG_REVERSE;
    buffer->Data[3] = DOGM_POWER_CONTROL | 0x07;
    buffer->Data[4] = DOGM_SET_ELECTRONIC_VOLUME;
    buffer->Data[5] = 0x20;
    buffer->Data[6] = DOGM_ENABLE_DISPLAY;
    buffer->Data[7] = DOGM_INVERSE_ON;

    uint8_t bufferid = buffer - Buffers;
    SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(buffer->Data, len),
				   TSpiDmaJob::CS_LCD, TSpiDmaJob::LCD_CONTROL,
				   this, reinterpret_cast<void*>(bufferid)));
  }
}

bool TDisplay::EnqueueDmaJob(TPageBuffer* pageBuffer,
			     uint8_t len, uint8_t ctrllen)
{
  if (true /* DMA queue space is available */) {
    uint8_t bufferid = pageBuffer - Buffers;
    if (len > 0) {
      SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(pageBuffer->Control, ctrllen),
				     TSpiDmaJob::CS_LCD, TSpiDmaJob::LCD_CONTROL));
      SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(pageBuffer->Data, len),
				     TSpiDmaJob::CS_LCD, TSpiDmaJob::LCD_DATA,
				     this, reinterpret_cast<void*>(bufferid)));
    } else {
      SpiDmaQueue.Enqueue(TSpiDmaJob(TBuffer(pageBuffer->Control, ctrllen),
				     TSpiDmaJob::CS_LCD, TSpiDmaJob::LCD_CONTROL,
				     this, reinterpret_cast<void*>(bufferid)));
    }
    return true;
  } else {
    return false;
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
  for (int y = 0; y < 8; y++) {
    for (int i = 0; i < length; i++) {
      Framebuffer[col + i][8*page + y] = !!(buffer->Data[i] & (1 << y));
    }
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

#ifdef HOST
void TDisplay::DumpPixels()
{
  FILE* fd = ::fopen("display.pbm", "w");
  if (fd) {
    ::fprintf(fd, "P1\n");
    ::fprintf(fd, "# Display dump\n");
    ::fprintf(fd, "%d %d\n", Width*5, Height*5);
    for (int y = 0; y < Height; y++) {
      for (int repeat = 0; repeat < 5; repeat++) {
	for (int x = 0; x < Width; x++) {
	  uint8_t v = (repeat < 4) ? Framebuffer[x][y] : 0;
	  ::fprintf(fd, "%d %d %d %d %d", v, v, v, v, 0);
	}
	::fprintf(fd, "\n");
      }
    }
    ::fclose(fd);
  } else {
    LOG("Failed to open display.pbm");
  }
}
#endif

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
	if (offset >= Width-1) goto out;
	Data[offset++] = fontliqstingmono_obj.glyph_data(glyph, c);
      }
    }
#else
    if ((uint8_t)*pt < FONT_GLYPHS) {
      for (int c = 0; c < 6; c++) {
	if (offset >= Width-1) goto out;
	Data[offset++] = Font[(uint8_t)*pt][c];
      }
    }
#endif

    else {
      for (int c = 0; c < 6; c++) {
	if (offset >= Width-1) goto out;
	Data[offset++] = 0xff;
      }
    }
    assert(offset < Width);
  }

 out:
  return offset;
}
