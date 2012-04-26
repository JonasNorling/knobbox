#include "stm32.h"
#include "TDisplay.h"
#include "TSpiDmaJob.h"

TDisplay Display;
TSpiDmaQueue SpiDmaQueue;

int main(void)
{
  Display.Init();
  Display.Power(true);

  TDisplay::TPageBuffer* line = Display.GetBuffer();
  if (line) {
    uint8_t length = line->DrawText("Hej hopp!");
    Display.OutputBuffer(line, length, 0, 0);
  }

  return 0;
}
