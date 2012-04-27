#ifdef __cplusplus
extern "C" {
#endif

#ifndef HOST
#include <spi.h>
#include <nvic.h>
#include <f1/rcc.h>
#include <f1/gpio.h>
#include <f1/dma.h>
#endif

extern "C" {
  void dma1_channel7_isr(void);
}

#ifdef __cplusplus
}
#endif
