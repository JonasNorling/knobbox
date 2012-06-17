/* -*- c++ -*- */
#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#include <spi.h>
#include <systick.h>
#include <nvic.h>
#include <usart.h>
#include <f1/rcc.h>
#include <f1/gpio.h>
#include <f1/dma.h>
#include <timer.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>

#ifdef __cplusplus
}
#endif

/**
 * GPIO pin abstraction. Everything should be inlined, so calls are
 * faster than to libopencm3.
 */
class TPin {
public:
  uint32_t Port;
  uint16_t Pin;

  void SetOutput(uint8_t mode = GPIO_MODE_OUTPUT_50_MHZ,
		 uint8_t cnf = GPIO_CNF_OUTPUT_PUSHPULL) const
    __attribute__((always_inline))
  {
    gpio_set_mode(Port, mode, cnf, Pin);
  }

  void SetInput(uint8_t mode = GPIO_MODE_INPUT,
		uint8_t cnf = GPIO_CNF_INPUT_FLOAT) const
    __attribute__((always_inline))
  {
    gpio_set_mode(Port, mode, cnf, Pin);
  }

  void Toggle() const
    __attribute__((always_inline))
  {
    //gpio_toggle(Port, Pin);
    GPIO_ODR(Port) ^= Pin;
  }

  void Set() const
    __attribute__((always_inline))
  {
    //gpio_set(Port, Pin);
    GPIO_BSRR(Port) = Pin;
  }

  void Clear() const
    __attribute__((always_inline))
  {
    //gpio_clear(Port, Pin);
    GPIO_BRR(Port) = Pin;
  }
};
