#include "TUsb.h"
#include "device.h"
#include "TControllers.h"

extern "C" {
  void UsbHidInit();
  void UsbMidiInit();
};

void TUsb::Init()
{
#ifndef HOST
  UsbMidiInit();
  //UsbHidInit();
#endif
}
