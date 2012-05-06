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

#ifndef HOST

  class TPin {
  public:
    uint32_t Port;
    uint16_t Pin;

    void SetOutput(uint8_t mode = GPIO_MODE_OUTPUT_50_MHZ,
		   uint8_t cnf = GPIO_CNF_OUTPUT_PUSHPULL) const
    {
      gpio_set_mode(Port, mode, cnf, Pin);
    }

    // Would have been nice, but gcc doesn't like to inline it today
    //void Toggle() const { gpio_toggle(Port, Pin); }
  };

  static const TPin Pin_lcd_a0 =   {GPIOB, GPIO7}; // Discovery board
  //static const TPin Pin_lcd_a0 =   {GPIOA, GPIO4}; // real board
  static const TPin Pin_lcd_cs =   {GPIOA, GPIO4};
  static const TPin Pin_lcd_rst =  {GPIOA, GPIO0};
  static const TPin Pin_flash_cs = {GPIOB, GPIO0};
  static const TPin Pin_spi_mosi = {GPIOA, GPIO7};
  static const TPin Pin_spi_miso = {GPIOA, GPIO6};
  static const TPin Pin_spi_sck =  {GPIOA, GPIO5};

  static const TPin Pin_sw_1 =     {GPIOB, GPIO5};
  static const TPin Pin_sw_2 =     {GPIOB, GPIO4};
  static const TPin Pin_sw_3 =     {GPIOB, GPIO13};
  static const TPin Pin_sw_4 =     {GPIOB, GPIO12};

  static const TPin Pin_led_b = {GPIOC, GPIO8};
  static const TPin Pin_led_g = {GPIOC, GPIO9};

#endif // HOST



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
