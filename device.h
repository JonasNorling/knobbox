/* -*- c++ -*- */
#pragma once

#ifdef HOST
#include "host.h"
#else
#include "stm32.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
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
