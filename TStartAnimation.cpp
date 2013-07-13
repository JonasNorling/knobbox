#include "TStartAnimation.h"
#include "TGui.h"
#include "TKnobs.h"

void TStartAnimation::Show()
{
    Gui.UpdateAll();

    for (int knob = 0; knob < TKnobs::Knobs; knob++) {
      Knobs.LedIntensity[Knobs.COLOR_RED][knob] = 0x00;
      Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 0x00;
    }

    while (true) {
        TEvent event = Gui.WaitForEvent();

        switch (event_code(event)) {
        case RENDER_LINE: {
            const uint8_t n = event_value(event);
            TDisplay::TPageBuffer* line = Display.GetBuffer();
            if (line) {
                line->Clear();
                Render(n, line);
                Display.OutputBuffer(line, line->GetLength(), n, 0);
            }
            else {
                Gui.UpdateLine(n); // Try again
            }

            if (!Gui.HaveDirtyLines()) {
                State += 4;

                if (State >= 256) {
                    return;
                }

                Gui.UpdateAll();

                for (int knob = 0; knob < TKnobs::Knobs; knob++) {
                  Knobs.LedIntensity[Knobs.COLOR_RED][knob] = (State >> 4) >= knob ? 0xff : 0;
                  Knobs.LedIntensity[Knobs.COLOR_GREEN][knob] = 256 - State;
                }
            }
            break;
        }
        }
    }
}

void TStartAnimation::Render(uint8_t n, TDisplay::TPageBuffer* line)
{
    //line->Invert(0, line->GetLength());

    for (int i = 0; i < line->GetLength(); i++) {
        for (int bit = 0; bit < 8; bit++) {
            line->Data[i] |= ((rand() - (State << 22)) > RAND_MAX/2) ? (1 << bit) : 0x00;
        }
    }
}
