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
    void hard_fault_handler(void);
    void mem_manage_handler(void);
    void bus_fault_handler(void);
    void usage_fault_handler(void);
    void pend_sv_handler(void);
    void dma1_channel1_isr(void);
    void dma1_channel2_isr(void); // SPI1_RX
    void dma1_channel3_isr(void); // SPI1_TX
    void dma1_channel4_isr(void); // USART1_TX, SPI2_RX
    void dma1_channel5_isr(void); // USART1_RX, SPI2_TX
    void dma1_channel6_isr(void); // USART2_RX
    void dma1_channel7_isr(void); // USART2_TX
    void usart1_isr(void) __attribute__((optimize(3)));
    void usart2_isr(void);
    void sys_tick_handler(void);
    void tim2_isr(void);
#ifdef __cplusplus
}
#endif

#define LCD_SPI_CHANNEL SPI2
#define LCD_DMA_RX_CHANNEL DMA_CHANNEL4
#define LCD_DMA_TX_CHANNEL DMA_CHANNEL5

#define FLASH_SPI_CHANNEL SPI1
#define FLASH_DMA_RX_CHANNEL DMA_CHANNEL2
#define FLASH_DMA_TX_CHANNEL DMA_CHANNEL3

static const TPin Pin_lcd_a0 =         {GPIOB, GPIO14};
static const TPin Pin_lcd_cs =         {GPIOB, GPIO12};
static const TPin Pin_flash_cs =       {GPIOA, GPIO15};
static const TPin Pin_spi1_mosi =      {GPIOA, GPIO7};
static const TPin Pin_spi1_miso =      {GPIOA, GPIO6};
static const TPin Pin_spi1_sck =       {GPIOA, GPIO5};
static const TPin Pin_spi2_mosi =      {GPIOB, GPIO15};
static const TPin Pin_spi2_miso =      {GPIOB, GPIO14};
static const TPin Pin_spi2_sck =       {GPIOB, GPIO13};

static const TPin Pin_shift_out =      {GPIOA, GPIO9};
static const TPin Pin_shift_in =       {GPIOA, GPIO10};
static const TPin Pin_shift_clk =      {GPIOA, GPIO8};
static const TPin Pin_shift_out_load = {GPIOB, GPIO8};
static const TPin Pin_shift_out_en =   {GPIOB, GPIO9};
static const TPin Pin_shift_in_load =  {GPIOB, GPIO6};
static const TPin Pin_shift_in_en =    {GPIOB, GPIO7};

static const TPin Pin_midi1_out =      {GPIOA, GPIO2};
static const TPin Pin_midi1_in =       {GPIOA, GPIO3};
static const TPin Pin_midi2_out =      {GPIOB, GPIO10};
static const TPin Pin_midi2_in =       {GPIOB, GPIO11};

static const TPin Pin_sw_1 =           {GPIOC, GPIO0};
static const TPin Pin_sw_2 =           {GPIOC, GPIO1};
static const TPin Pin_sw_3 =           {GPIOC, GPIO2};
static const TPin Pin_sw_4 =           {GPIOC, GPIO3};
static const TPin Pin_sw_5 =           {GPIOC, GPIO4};

static const TPin Pin_led_1 =          {GPIOC, GPIO6};
static const TPin Pin_led_2 =          {GPIOC, GPIO7};
static const TPin Pin_led_3 =          {GPIOC, GPIO8};
static const TPin Pin_led_4 =          {GPIOC, GPIO9};
static const TPin Pin_led_tp9 =        {GPIOA, GPIO4};
static const TPin Pin_led_tp16 =       {GPIOA, GPIO1};
static const TPin Pin_lcd_backlight =  {GPIOB, GPIO0};
static const TPin Pin_led_enc1 =       {GPIOB, GPIO1};
static const TPin Pin_led_enc2 =       {GPIOC, GPIO10};

static const TPin Pin_vpullup =        {GPIOC, GPIO5};

void clockInit();
void deviceInit();

static inline void delay_ms(unsigned n) {
    for (unsigned i=0; i < n*10000; i++) __asm__("nop");
}

enum TMode {
    MODE_FIRST = 0,

    MODE_CONTROLLER = 0,
    MODE_SEQ = 1,
    MODE_SETTINGS = 2,

    MODE_LAST = MODE_SETTINGS
};
extern int Mode;

#ifndef SCHEDULER_NUM_TASKS
#define SCHEDULER_NUM_TASKS 2
#endif
