#include "TGui.h"
#include "TDisplay.h"
#include "TBitmask.h"

void TGui::Process()
{
#ifdef CHARMAP
  if (CurrentLine < Lines) {
    TDisplay::TPageBuffer* line = Display.GetBuffer();
    if (line) {
      line->Clear();
      char str[17];
      int i;
      for (i = 0; i < sizeof(str)-1; i++) {
	str[i] = CurrentLine*16 + i;
      }
      str[i] = '\0';
      if (str[0] == '\0') str[0] = ' ';
      uint8_t offset = line->DrawText(str, 3);
      Display.OutputBuffer(line, offset, CurrentLine, 0);
      CurrentLine++;
    }
  }
#else
  if (CurrentLine < Lines) {
    TDisplay::TPageBuffer* line = Display.GetBuffer();
    if (line) {
      line->Clear();
      if (CurrentLine == 0) {
	uint8_t pos = 0;
	uint8_t shadestart = pos = line->DrawText("\020CTRL\021", pos);
	pos = line->DrawText(" SEQ ", pos);
	pos = line->DrawText(" SETUP", pos);
	line->Invert(shadestart, line->GetLength());
      }
      else if (CurrentLine == 1) {
	line->DrawText("c1 Blofeld set 1", 3);
      }
      else if (CurrentLine == 2) {
	line->DrawText("clock ON, seq ON", 3);
	line->Invert(3, 3+6*8);
      }
      else if (CurrentLine == 3) {
	line->DrawText("\005knob3: 122", 3);
      }
      else if (CurrentLine == 4) {
	line->DrawText("\007CC33 Attack", 3);
      }
      else if (CurrentLine == 5) {
	line->DrawText("\005knob9: 39", 3);
      }
      else if (CurrentLine == 6) {
	line->DrawText("\007NPRNxxxx: 114", 3);
      }
      else if (CurrentLine == 7) {
	line->DrawText("tempo, i/o...", 3);
	line->Invert(0, line->GetLength());
      }
      Display.OutputBuffer(line, line->GetLength(), CurrentLine, 0);
      CurrentLine++;
    }
  }
#endif
}
