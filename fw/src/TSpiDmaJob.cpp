#include "TSpiDmaJob.h"
#include <cstring>

TBlockingSpiDmaJob* TBlockingSpiDmaJob::CurrentJob = 0;

static uint32_t DummyPosition = 0;

TBlockingSpiDmaJob::TBlockingSpiDmaJob(const uint8_t* out, uint8_t* in, int len)
: Out(out), In(in), Len(len), TaskId(0), Finished(false)
{
    assert(Len > 0);
    assert(Out);
}

void TBlockingSpiDmaJob::Run()
{
    TaskId = TScheduler::GetCurrentTaskId();
    CurrentJob = this;
#ifndef HOST
    const uint32_t spi = FLASH_SPI_CHANNEL;
    const uint32_t dma = DMA1;

    assert(SPI_SR(spi) & SPI_SR_TXE);
    assert(~SPI_SR(spi) & SPI_SR_BSY);
    assert(!(SPI_SR(spi) & SPI_SR_RXNE));

    {
        const uint32_t channel = FLASH_DMA_TX_CHANNEL;
        dma_channel_reset(dma, channel);
        dma_set_peripheral_address(dma, channel, reinterpret_cast<uint32_t>(&SPI_DR(spi)));
        dma_set_memory_address(dma, channel, reinterpret_cast<uint32_t>(Out));
        dma_set_number_of_data(dma, channel, Len);
        dma_set_read_from_memory(dma, channel);
        dma_enable_memory_increment_mode(dma, channel);
        dma_set_peripheral_size(dma, channel, DMA_CCR_PSIZE_8BIT);
        dma_set_memory_size(dma, channel, DMA_CCR_MSIZE_8BIT);
        dma_set_priority(dma, channel, DMA_CCR_PL_LOW);
        // dma_enable_transfer_complete_interrupt(dma, channel);
        dma_enable_channel(dma, channel);
    }

    {
        const uint32_t channel = FLASH_DMA_RX_CHANNEL;
        dma_channel_reset(dma, channel);
        dma_set_peripheral_address(dma, channel, reinterpret_cast<uint32_t>(&SPI_DR(spi)));
        dma_set_number_of_data(dma, channel, Len);
        if (In != 0) {
            dma_set_memory_address(dma, channel, reinterpret_cast<uint32_t>(In));
            dma_enable_memory_increment_mode(dma, channel);
        }
        else {
            dma_set_memory_address(dma, channel, reinterpret_cast<uint32_t>(&DummyPosition));
        }
        dma_set_peripheral_size(dma, channel, DMA_CCR_PSIZE_8BIT);
        dma_set_memory_size(dma, channel, DMA_CCR_MSIZE_8BIT);
        dma_set_priority(dma, channel, DMA_CCR_PL_LOW);
        dma_enable_transfer_complete_interrupt(dma, channel);
        dma_enable_channel(dma, channel);
    }

    // Kick off the transfer
    spi_enable_rx_dma(spi);
    spi_enable_tx_dma(spi);

    // Wait for completion
    TScheduler::BlockUntil(&Finished);
#endif
}

/*
 * *****************************************************
 */

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
    // We treat both SPI busses (flash and display) as one, so no concurrent jobs
    // are allowed.

    const uint32_t spi = LCD_SPI_CHANNEL;
    if ((SPI_SR(spi) & SPI_SR_TXE) && (~SPI_SR(spi) & SPI_SR_BSY)) {

        if (Jobs.Empty()) {
            return false;
        }

        const TSpiDmaJob& job = Jobs.First();
        const TBuffer& buffer = job.GetBuffer();

        // SPI receive register should be empty here!
        assert(!(SPI_SR(spi) & SPI_SR_RXNE));

        const uint32_t dma = DMA1;
        {
            const uint32_t channel = (job.GetChip() == TSpiDmaJob::CS_LCD) ? LCD_DMA_TX_CHANNEL : FLASH_DMA_TX_CHANNEL;
            dma_channel_reset(dma, channel);
            dma_set_peripheral_address(dma, channel, reinterpret_cast<uint32_t>(&SPI_DR(spi)));
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
            const uint32_t channel =  (job.GetChip() == TSpiDmaJob::CS_LCD) ? LCD_DMA_RX_CHANNEL : FLASH_DMA_RX_CHANNEL;
            dma_channel_reset(dma, channel);
            dma_set_peripheral_address(dma, channel, reinterpret_cast<uint32_t>(&SPI_DR(spi)));
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
        Pin_lcd_cs.Clear();
        if (job.GetLcdData()) {
            Pin_lcd_a0.Set();
        } else {
            Pin_lcd_a0.Clear();
        }

        // Kick off the transfer
        spi_enable_rx_dma(spi);
        spi_enable_tx_dma(spi);

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
