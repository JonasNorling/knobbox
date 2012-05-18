/* -*- c++ -*- */
#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HOST

#include <spi.h>
#include <systick.h>
#include <nvic.h>
#include <usart.h>
#include <f1/rcc.h>
#include <f1/gpio.h>
#include <f1/dma.h>
#include <timer.h>

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

#else // HOST

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

#endif // HOST

  static const TPin Pin_lcd_a0 =         {GPIOB, GPIO7}; // Discovery
  //static const TPin Pin_lcd_a0 =         {GPIOA, GPIO4};
  static const TPin Pin_lcd_cs =         {GPIOA, GPIO4};
  //static const TPin Pin_lcd_rst =        {GPIOA, GPIO0};
  static const TPin Pin_lcd_rst =        {GPIOA, GPIO1}; // Discovery
  static const TPin Pin_flash_cs =       {GPIOB, GPIO0};
  static const TPin Pin_spi_mosi =       {GPIOA, GPIO7};
  static const TPin Pin_spi_miso =       {GPIOA, GPIO6};
  static const TPin Pin_spi_sck =        {GPIOA, GPIO5};
 
  static const TPin Pin_shift_out=       {GPIOA, GPIO9};
  static const TPin Pin_shift_in =       {GPIOA, GPIO10};
  static const TPin Pin_shift_clk =      {GPIOA, GPIO8};
  static const TPin Pin_shift_out_load = {GPIOB, GPIO8};
  static const TPin Pin_shift_out_en =   {GPIOB, GPIO9};
  static const TPin Pin_shift_in_load =  {GPIOB, GPIO6};
  static const TPin Pin_shift_in_en =    {GPIOB, GPIO7};

  static const TPin Pin_midi_out =       {GPIOA, GPIO2};
  static const TPin Pin_midi_in =        {GPIOA, GPIO3};

  //static const TPin Pin_sw_1 =           {GPIOA, GPIO0}; // Discovery
  static const TPin Pin_sw_1 =           {GPIOB, GPIO5};
  static const TPin Pin_sw_2 =           {GPIOB, GPIO4};
  static const TPin Pin_sw_3 =           {GPIOB, GPIO13};
  static const TPin Pin_sw_4 =           {GPIOB, GPIO12};

  static const TPin Pin_led_b =          {GPIOC, GPIO8}; // Discovery
  static const TPin Pin_led_g =          {GPIOC, GPIO9}; // Discovery


  void clockInit();
  void deviceInit();

  static inline void delay_ms(unsigned n) {
    for (unsigned i=0; i < n*7000; i++) __asm__("nop");
  }

  // Need to declare ISRs as extern "C" for the linker to find them:
  void dma1_channel1_isr(void);
  void dma1_channel2_isr(void); // SPI1_RX
  void dma1_channel3_isr(void); // SPI1_TX
  void dma1_channel4_isr(void); // USART1_TX
  void dma1_channel5_isr(void); // USART1_RX
  void dma1_channel6_isr(void); // USART2_RX
  void dma1_channel7_isr(void); // USART2_TX
  void sys_tick_handler(void);
  void tim2_isr(void);

#ifdef __cplusplus
}
#endif
