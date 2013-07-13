#include "TSpiDmaJob.h"
#include <cstring>

TSpiDmaQueue::TSpiDmaQueue()
{
}

bool TSpiDmaQueue::Enqueue(const TSpiDmaJob& job)
{
  if (Jobs.Add(job)) {
#ifdef HOST
    dma1_channel4_isr();
#else
    //    TryStartJob();
#endif
    return true;
  } else {
    return false;
  }
}

bool TSpiDmaQueue::TryStartJob()
{
#ifndef HOST
  // Start the next DMA job in the queue if the SPI interface is
  // available (TXE=1 and BSY=0)

  if ((SPI_SR(LCD_SPI_CHANNEL) & SPI_SR_TXE) && (~SPI_SR(LCD_SPI_CHANNEL) & SPI_SR_BSY)) {

    if (Jobs.Empty()) {
        return false;
    }

    const TSpiDmaJob& job = Jobs.First();
    const TBuffer& buffer = job.GetBuffer();

    assert(!(job.GetChip() == TSpiDmaJob::CS_FLASH_ONESHOT && Pin_flash_cs.Read() == false));
    assert(!(job.GetChip() == TSpiDmaJob::CS_FLASH_START && Pin_flash_cs.Read() == false));
    assert(!(job.GetChip() == TSpiDmaJob::CS_FLASH_END && Pin_flash_cs.Read() == true));

    // SPI receive register should be empty here!
    assert(!(SPI_SR(LCD_SPI_CHANNEL) & SPI_SR_RXNE));

    {
      const uint32_t dma = DMA1;
      const uint32_t channel = LCD_DMA_TX_CHANNEL;
      dma_channel_reset(dma, channel);
      dma_set_peripheral_address(dma, channel, reinterpret_cast<uint32_t>(&SPI_DR(LCD_SPI_CHANNEL)));
      dma_set_memory_address(dma, channel, reinterpret_cast<uint32_t>(buffer.GetData()));
      dma_set_number_of_data(dma, channel, buffer.GetLength());
      dma_set_read_from_memory(dma, channel);
      dma_enable_memory_increment_mode(dma, channel);
      dma_set_peripheral_size(dma, channel, DMA_CCR_PSIZE_8BIT);
      dma_set_memory_size(dma, channel, DMA_CCR_MSIZE_8BIT);
      dma_set_priority(dma, channel, DMA_CCR_PL_LOW);
      //dma_enable_transfer_complete_interrupt(dma, channel);
      dma_enable_channel(dma, channel);
    }

    {
      const uint32_t dma = DMA1;
      const uint32_t channel = LCD_DMA_RX_CHANNEL;
      dma_channel_reset(dma, channel);
      dma_set_peripheral_address(dma, channel, reinterpret_cast<uint32_t>(&SPI_DR(LCD_SPI_CHANNEL)));
      dma_set_memory_address(dma, channel, reinterpret_cast<uint32_t>(buffer.GetData()));
      dma_set_number_of_data(dma, channel, buffer.GetLength());
      dma_enable_memory_increment_mode(dma, channel);
      dma_set_peripheral_size(dma, channel, DMA_CCR_PSIZE_8BIT);
      dma_set_memory_size(dma, channel, DMA_CCR_MSIZE_8BIT);
      dma_set_priority(dma, channel, DMA_CCR_PL_LOW);
      dma_enable_transfer_complete_interrupt(dma, channel);
      dma_enable_channel(dma, channel);
    }

    // Set CS and LCD_A0 lines
    if (job.GetChip() == TSpiDmaJob::CS_LCD) {
      Pin_flash_cs.Set();
      Pin_lcd_cs.Clear();
      if (job.GetLcdData()) {
	Pin_lcd_a0.Set();
      } else {
	Pin_lcd_a0.Clear();
      }
    }
    else {
      Pin_lcd_cs.Set();
      Pin_flash_cs.Clear();
    }

#if 1
    // Kick off the transfer
    spi_enable_rx_dma(LCD_SPI_CHANNEL);
    spi_enable_tx_dma(LCD_SPI_CHANNEL);
#else
    uint8_t temp[buffer.GetLength()];
    for (int i = 0; i < buffer.GetLength(); i++) {
        temp[i] = spi_xfer(LCD_SPI_CHANNEL, buffer.GetData()[i]);
    }
    memcpy(buffer.GetData(), temp, buffer.GetLength());
    Finished();
    return false;
#endif

    return true;
  }
#endif
  return false;
}

void TSpiDmaJob::Finished() const
{
  if (Callback) {
    Callback->DmaFinished(Context);
  }
}
