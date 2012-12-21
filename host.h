/* -*- c++ -*- */
#pragma once

#include <cstdint>

#define GPIO_MODE_OUTPUT_50_MHZ 0
#define GPIO_CNF_OUTPUT_PUSHPULL 0
#define GPIO_MODE_INPUT 0
#define GPIO_CNF_INPUT_FLOAT 0
#define GPIOA 'A'
#define GPIOB 'B'
#define GPIOC 'C'
#define GPIO0 0
#define GPIO1 1
#define GPIO2 2
#define GPIO3 3
#define GPIO4 4
#define GPIO5 5
#define GPIO6 6
#define GPIO7 7
#define GPIO8 8
#define GPIO9 9
#define GPIO10 10
#define GPIO11 11
#define GPIO12 12
#define GPIO13 13
#define GPIO14 14
#define GPIO15 15
#define USART1 1
#define USART2 2

static inline void critical_section()
{
}

static inline void end_critical_section()
{
}

class TPin {
public:
  uint32_t Port;
  uint16_t Pin;

  void SetOutput(uint8_t __attribute__((unused)) mode = GPIO_MODE_OUTPUT_50_MHZ,
		 uint8_t __attribute__((unused)) cnf = GPIO_CNF_OUTPUT_PUSHPULL) const { }
  void SetInput(uint8_t __attribute__((unused)) mode = GPIO_MODE_INPUT,
		uint8_t __attribute__((unused)) cnf = GPIO_CNF_INPUT_FLOAT) const { }
  void Toggle() const { }
  void Set() const { }
  void Clear() const { }
};

static inline uint16_t gpio_port_read(uint32_t) { return 0; }
void usart_send(uint32_t uart, uint8_t byte);
