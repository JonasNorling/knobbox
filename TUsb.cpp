#include "TUsb.h"
#include "device.h"
#include "TControllers.h"

extern "C" {
  void UsbMidiInit();
};

void TUsb::Init()
{
#ifndef HOST
  UsbMidiInit();
#endif
}

#ifndef HOST
void TUsb::DataCallback(uint8_t ep __attribute__((unused)))
{
  const int BUFLEN = 64;
  char buf[BUFLEN];
  int len = usbd_ep_read_packet(1, buf, BUFLEN);
  if (len) {
    Controllers.IncreaseValue(0, 1);
  }
}

void UsbDataCallback(uint8_t ep)
{
  TUsb::DataCallback(ep);
}
#endif
