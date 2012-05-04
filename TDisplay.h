/* -*- c++ -*-
 *
 * Device driver for EA DOGS102W-6 LCD display on SPI.  The display is
 * 102x64 pixels and is controlled by a UC1701x driver chip (capable
 * of driving a 132x65 pixel display). The display isn't mapped as a
 * regular framebuffer, but as horizontal bands ("pages") 8 pixels
 * high and 132 pixels wide (of which 102 pixels actually exist on
 * this particular display, of course). Each byte in the display RAM
 * corresponds to 8 vertical pixels in the page. This arrangement
 * makes it easy to draw text with a 8-pixel font (that could be
 * variable width), but a bit fiddly to draw graphics.
 *
 * The display is mainly treated as if it's a 17x8 character display,
 * and a 6x8 font (with empty space top and right) is used to render
 * text. Each character is thus 6 bytes in size.
 */
#pragma once

#include "TSpiDmaJob.h"
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <functional>

typedef uint8_t TCharacter[6];

class TBuffer
{
public:
  TBuffer(uint8_t* data, uint8_t len) :
    Data(data), Length(len) {}

  uint8_t* GetData() const { return Data; }
  uint8_t GetLength() const { return Length; }

  //start, end
  //xor, slice

private:
  TBuffer();
  TBuffer(const TBuffer&);
  const TBuffer& operator=(const TBuffer&);

  uint8_t* Data;
  uint8_t Length;
};

class TDisplay : public IDmaCallback
{
private:
  static const int Width = 102;
  static const int Height = 64;
  static const int WidthChars = 17;
  static const int HeightChars = 8;
  static const int BufferCount = 2;

public:
  class TPageBuffer {
  public:
    friend class TDisplay;
    uint8_t DrawText(const char* text, uint8_t offset = 0);
    void Clear()
    {
      // Fixme: Is this optimised to 32-bit?
      for (int i=0; i<Width; i++) Data[i] = 0;
    }
    void Invert(uint8_t start, uint8_t end)
    {
      // Fixme: Is this optimised to 32-bit?
      for (int i=start; i<end; i++) Data[i] = ~Data[i];
    }
    uint8_t GetLength() const { return Width; }
  private:
    TPageBuffer() {}
    const TPageBuffer& operator=(const TPageBuffer&);    
    uint8_t Data[Width];
    uint8_t Control[3];
  };

  TDisplay() { }
  void Init();

  uint8_t GetWidthChars() { return WidthChars; }
  uint8_t GetHeightChars() { return HeightChars; }

  bool Power(bool on);

  TPageBuffer* GetBuffer();
  void OutputBuffer(TPageBuffer* buffer, uint8_t length,
		    uint8_t page, uint8_t col);

#ifdef HOST
  void DumpPixels();
#endif

  /* IDmaCallback */

  virtual void DmaFinished(void* context);

private:
  bool EnqueueDmaJob(TPageBuffer* pageBuffer,
		     uint8_t len, uint8_t ctrllen = 3)
  {
    if (true /* DMA queue space is available */) {
      uint8_t bufferid = (pageBuffer - &Buffers[0]) /
	sizeof(Buffers[0]);
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

  /* We keep a little memory pool here that can be used to store data
     for DMA jobs. It's a bit wasteful to use [Width] bytes when
     sending a little command, but whatever. */
  TPageBuffer Buffers[BufferCount];
  uint8_t BufferAllocMask;

#ifdef HOST
  uint8_t Framebuffer[Width][Height];
#endif
};

extern TDisplay Display;
