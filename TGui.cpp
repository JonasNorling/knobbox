#include "TGui.h"
#include "TDisplay.h"
#include "TBitmask.h"

void TGui::Process()
{
  if (CurrentLine < Lines) {
    TDisplay::TPageBuffer* line = Display.GetBuffer();
    if (line) {
      line->Clear();
      char str[17];
      for (int i = 0; i < 16; i++) {
	str[i] = CurrentLine*16 + i;
      }
      if (str[0] == '\0') str[0] = ' ';
      str[16] = '\0';
      uint8_t length = line->DrawText(str);
      Display.OutputBuffer(line, length, CurrentLine, 0);
      CurrentLine++;
    }
  }
}
