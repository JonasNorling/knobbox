#include "TSpiDmaJob.h"
#include "TDisplay.h"


bool TSpiDmaQueue::Enqueue(const TSpiDmaJob& job)
{
  if (Jobs.Add(job)) {
#ifdef HOST
    dma1_channel3_isr();
#else
    //    TryStartJob();
#endif
    return true;
  } else {
    return false;
  }
}

void TSpiDmaQueue::TryStartJob()
{
#ifndef HOST
  // Start the next DMA job in the queue if the SPI interface is
  // available (TXE=1 and BSY=0)

  if (!Jobs.Empty() &&
      (SPI1_SR & SPI_SR_TXE) &&
      !(SPI1_SR & SPI_SR_BSY)) {
    const TSpiDmaJob& job = Jobs.First();
    const TBuffer& buffer = job.GetBuffer();

    // Set CS and LCD_A0 lines
    if (job.GetChip() == TSpiDmaJob::CS_LCD) {
      // FIXME: Move pin mapping to a method in TDisplay
      gpio_clear(GPIOA, GPIO4);
      if (job.GetLcdData()) {
	gpio_set(GPIOB, GPIO7);
      } else {
	gpio_clear(GPIOB, GPIO7);
      }
    }

    // FIXME: Don't have to do all this every time.
#define DMA DMA1
#define CHANNEL DMA_CHANNEL3
    dma_channel_reset(DMA, CHANNEL);
    dma_set_peripheral_address(DMA, CHANNEL, reinterpret_cast<uint32_t>(&SPI1_DR));
    dma_set_memory_address(DMA, CHANNEL, reinterpret_cast<uint32_t>(buffer.GetData()));
    dma_set_number_of_data(DMA, CHANNEL, buffer.GetLength());
    dma_set_read_from_memory(DMA, CHANNEL);
    dma_enable_memory_increment_mode(DMA, CHANNEL);
    dma_set_peripheral_size(DMA, CHANNEL, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size(DMA, CHANNEL, DMA_CCR_MSIZE_8BIT);
    dma_set_priority(DMA, CHANNEL, DMA_CCR_PL_LOW);
    dma_enable_transfer_complete_interrupt(DMA, CHANNEL);
    dma_enable_channel(DMA, CHANNEL);
    spi_enable_tx_dma(SPI1);
    gpio_set(GPIOC, GPIO9);
  }
#endif
}
