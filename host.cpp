#include "device.h"
#include <cstdio>

void clockInit()
{
}

void deviceInit()
{
}

void usart_send(uint32_t uart, uint8_t byte)
{
  printf("UART%d: 0x%02x\n", uart, byte);
}
