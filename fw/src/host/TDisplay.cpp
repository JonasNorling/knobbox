#include "TDisplay.h"

#include <wx/wx.h>

#include "TBitmask.h"
#include "device.h"
#include "logging.h"
#include "TScheduler.h"

TDisplay::TDisplay() :
          BufferAllocMask(TBitmask::Init(BufferCount))
{
}

void TDisplay::Init()
{
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
    if (LcdDisplay) {
        for (int y = 0; y < 8; y++) {
            for (int i = 0; i < length; i++) {
                LcdDisplay->Framebuffer[col + i][8*page + y] = !!(buffer->Data[i] & (1 << y));
            }
        }

        LcdDisplay->Refresh();
    }
    ::usleep(2000);

    uint8_t bufferid = buffer - Buffers;
    TBitmask::Release(BufferAllocMask, bufferid);
}

void TDisplay::DmaFinished(void* context)
{
}

bool TDisplay::Power(bool on)
{
    return true;
}

//#define FONT_LIQUID
#ifdef FONT_LIQUID
#include "liquid-2.0/fontliqsting.inc"
#else
#include "font.h"
#endif

uint8_t TDisplay::TPageBuffer::DrawText(const char* text, uint8_t offset, bool invert)
{
    for (const char* pt = text; *pt != '\0'; pt++) {
#ifdef FONT_LIQUID
        glyph_id_t glyph = fontliqstingmono_obj.first_glyph((uint8_t)*pt);
        if (glyph) {
            for (int c = 0; c < GlyphWidth; c++) {
                if (offset >= Width-1) goto out;
                Data[offset++] = fontliqstingmono_obj.glyph_data(glyph, c);
            }
        }
#else
        if ((uint8_t)*pt < FONT_GLYPHS) {
            for (int c = 0; c < GlyphWidth; c++) {
                if (offset >= Width-1) goto out;
                Data[offset++] = invert ? ~Font[(uint8_t)*pt][c] : Font[(uint8_t)*pt][c];
            }
        }
#endif

        else {
            for (int c = 0; c < GlyphWidth; c++) {
                if (offset >= Width-1) goto out;
                Data[offset++] = 0xff;
            }
        }
        assert(offset < Width);
    }

    out:
    return offset;
}
