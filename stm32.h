/* -*- c++ -*- */
#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/systick.h>
#include <libopencm3/stm32/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/f1/dma.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/usb/usbd.h>

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

  bool Read() const
    __attribute__((always_inline))
  {
    return GPIO_IDR(Port) & Pin;
  }
};
