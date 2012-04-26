#include "stm32.h"
#include "TDisplay.h"

TDisplay Display;

int main(void)
{
  Display.Init();
  Display.Power(true);

  TDisplay::TPageBuffer* line = Display.GetBuffer();
  uint8_t length = line->DrawText("Hej hopp!");
  Display.OutputBuffer(line, length, 0, 0);

  while (true) {
  }

  return 0;
}
