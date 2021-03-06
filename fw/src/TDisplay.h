/* -*- c++ -*- */
#pragma once

#include "TSpiDmaJob.h"
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <functional>

typedef uint8_t TCharacter[6];

/**
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
 *
 * \todo The font is rather horrible.
 *
 * Status: Fairly complete and tested. Power/standby not implemented.
 */
class TDisplay : public IDmaCallback
{
public:
    static const int Width = 102;
    static const int Height = 64;
    static const int WidthChars = 17;
    static const int HeightChars = 8;
    static const int BufferCount = 2;
    static const int GlyphWidth = 6;

    /**
     * Represent a "page", an 8-pixel high band, on the display.
     */
    class TPageBuffer {
    public:
        friend class TDisplay;
        uint8_t DrawText(const char* text, uint8_t offset = 0, bool invert = false);
        /// Advance a character width.
        uint8_t Advance(uint8_t offset) { return offset + TDisplay::GlyphWidth; }

        void Clear()
        {
            for (int i=0; i<Width; i++) Data[i] = 0;
        }
        void Clear(uint8_t start, uint8_t end)
        {
            for (int i=start; i<end; i++) Data[i] = 0;
        }
        void Invert(uint8_t start, uint8_t end, uint8_t pattern = 0xff)
        {
            for (int i=start; i<end; i++) Data[i] = Data[i] ^ pattern;
        }
        uint8_t GetLength() const { return Width; }
        uint8_t Data[Width];

    private:
        TPageBuffer() {}
        const TPageBuffer& operator=(const TPageBuffer&);
        uint8_t Control[3];
    };

    TDisplay();
    void Init();

    uint8_t GetWidthChars() { return WidthChars; }
    uint8_t GetHeightChars() { return HeightChars; }

    bool Power(bool on);

    /// Get a buffer that will later be sent to the display.
    /// \return 0 if no buffers are available.
    TPageBuffer* GetBuffer();
    /// Enqueue the buffer to be sent to the display. This call will
    /// always succeed -- the fact that you got a buffer from
    /// GetBuffer() guarantees that it can be sent to the display.
    void OutputBuffer(TPageBuffer* buffer, uint8_t length,
            uint8_t page, uint8_t col);

    /* IDmaCallback */
    virtual void DmaFinished(void* context);

private:
    bool EnqueueDmaJob(TPageBuffer* pageBuffer, uint8_t len, uint8_t ctrllen);

    /* We keep a little memory pool here that can be used to store data
     for DMA jobs. It's a bit wasteful to use [Width] bytes when
     sending a little command, but whatever. */
    TPageBuffer Buffers[BufferCount];
    uint8_t BufferAllocMask;
};

extern TDisplay Display;
