#include "device.h"

/**
 * \file stm32.cpp STM32 initialization functions.
 */

void clockInit()
{
  /// \todo Want to run at 48 MHz.
  //rcc_clock_setup_in_hse_8mhz_out_24mhz();
  rcc_clock_setup_in_hsi_out_48mhz();

  /*
   * Systick timer striking every 1ms (1kHz):
   * div 8 --> 24MHz/8 = 3MHz
   * reload 2999 --> 3000000/(2999+1) --> 1000 overflows/second
   */
  systick_set_clocksource(STK_CTRL_CLKSOURCE_AHB_DIV8);
  systick_set_reload(rcc_ppre2_frequency / 8 / 1000 - 1);
  systick_interrupt_enable();
  systick_counter_enable();

  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN); // GPIOA
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN); // GPIOB
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN); // GPIOC

  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_SPI1EN); // SPI1
  rcc_peripheral_enable_clock(&RCC_AHBENR, RCC_AHBENR_DMA1EN); // DMA1
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_USART1EN); // USART1
  rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_USART2EN); // USART2
  rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN); // TIM2
  rcc_peripheral_enable_clock(&RCC_AHBENR, RCC_AHBENR_OTGFSEN); // USB
}


void deviceInit()
{
  /*
   * *********************************************************** *
   * GPIO
   */

  Pin_lcd_a0.SetOutput();
  Pin_lcd_cs.SetOutput();
  Pin_lcd_rst.SetOutput();
  Pin_flash_cs.SetOutput();
  Pin_shift_out_load.SetOutput();
  Pin_shift_out_en.SetOutput();
  Pin_shift_in_load.SetOutput();
  Pin_shift_in_en.SetOutput();

  // Discovery: LEDs on PC8 and PC9
  //Pin_led_b.SetOutput(GPIO_MODE_OUTPUT_2_MHZ);
  //Pin_led_g.SetOutput(GPIO_MODE_OUTPUT_50_MHZ);

  // Switches
  Pin_sw_1.SetInput(GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN);
  Pin_sw_2.SetInput(GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN);
  Pin_sw_3.SetInput(GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN);
  Pin_sw_4.SetInput(GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN);

  // SPI
  Pin_spi_mosi.SetOutput(GPIO_MODE_OUTPUT_50_MHZ,
			 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
  Pin_spi_sck.SetOutput(GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);

  // USART1
  Pin_shift_out.SetOutput(GPIO_MODE_OUTPUT_50_MHZ,
			  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
  Pin_shift_clk.SetOutput(GPIO_MODE_OUTPUT_50_MHZ,
			  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);

  // USART2
  Pin_midi_out.SetOutput(GPIO_MODE_OUTPUT_50_MHZ,
			 GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);

  Pin_vpullup.SetOutput(GPIO_MODE_OUTPUT_2_MHZ);

  /*
   * *********************************************************** *
   * SPI
   */

  spi_set_unidirectional_mode(SPI1);
  spi_disable_crc(SPI1);
  spi_set_dff_8bit(SPI1);
  spi_set_full_duplex_mode(SPI1);
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_32);
  spi_set_master_mode(SPI1);
  spi_set_clock_polarity_1(SPI1);
  spi_set_clock_phase_1(SPI1);
  spi_enable(SPI1);

  /*
   * *********************************************************** *
   * USART1: Shift registers
   */

  usart_set_baudrate(USART1, 1000000);

  usart_set_databits(USART1, 8);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  // Output clock, also on last bit
  USART_CR2(USART1) |= USART_CR2_CLKEN | USART_CR2_LBCL;
  usart_enable(USART1);

  /*
   * *********************************************************** *
   * USART2: MIDI
   */

  usart_set_baudrate(USART2, 31250);

  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX_RX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
  usart_enable(USART2);

  /*
   * *********************************************************** *
   * Interrupt priorities
   */
  /*
   * PREGROUP in AIRCR is reset to 0, so interrupts are grouped based
   * on all but the least significant bit. Lower number is higher
   * priority, and interrupts can preempt each other as long as their
   * priorities differ by at least two (that's the grouping). There
   * are 16 priority levels in this device.
   */

  // Enable can't-happen faults so we don't just get a hard fault
  nvic_enable_irq(NVIC_NMI_IRQ);
  nvic_enable_irq(NVIC_HARD_FAULT_IRQ);
  nvic_enable_irq(NVIC_MEM_MANAGE_IRQ);
  nvic_enable_irq(NVIC_BUS_FAULT_IRQ);
  nvic_enable_irq(NVIC_USAGE_FAULT_IRQ);

  // SPI DMA transmission done interrupt
  nvic_set_priority(NVIC_DMA1_CHANNEL3_IRQ, 4);
  nvic_enable_irq(NVIC_DMA1_CHANNEL3_IRQ);

  // SPI DMA reception done interrupt
  nvic_set_priority(NVIC_DMA1_CHANNEL2_IRQ, 4);
  nvic_enable_irq(NVIC_DMA1_CHANNEL2_IRQ);

  // Shift register USART DMA transmission done interrupt
  nvic_set_priority(NVIC_DMA1_CHANNEL4_IRQ, 4);
  nvic_enable_irq(NVIC_DMA1_CHANNEL4_IRQ);
  nvic_set_priority(NVIC_DMA1_CHANNEL5_IRQ, 4);
  nvic_enable_irq(NVIC_DMA1_CHANNEL5_IRQ);

  // Sequencer timer
  nvic_enable_irq(NVIC_TIM2_IRQ);
  nvic_set_priority(NVIC_TIM2_IRQ, 2);

  // Millisecond timer
  nvic_set_priority(NVIC_SYSTICK_IRQ, 2);

  // MIDI USART
  nvic_enable_irq(NVIC_USART2_IRQ);
  nvic_set_priority(NVIC_USART2_IRQ, 2);
}
