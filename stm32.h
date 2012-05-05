#pragma once
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

  static inline void delay_ms(unsigned n) {
    for (unsigned i=0; i < n*7000; i++) __asm__("nop");    
  }

  void dma1_channel1_isr(void);
  void dma1_channel2_isr(void);
  void dma1_channel3_isr(void);
  void dma1_channel4_isr(void);
  void dma1_channel5_isr(void);
  void dma1_channel6_isr(void);
  void dma1_channel7_isr(void);

#ifdef __cplusplus
}
#endif
