#include "../target/stm32.h"
#include <cstring>

static const uint8_t COMMAND_DEVICE_ID = 0x9f;
static const uint8_t COMMAND_READ_STATUS = 0x05;
static const uint8_t COMMAND_READ_ARRAY = 0x03;
static const uint8_t COMMAND_BLOCK_ERASE_4K = 0x20;
static const uint8_t COMMAND_PAGE_PROGRAM = 0x02;
static const uint8_t COMMAND_WRITE_ENABLE = 0x06;
static const uint8_t COMMAND_WRITE_DISABLE = 0x04;
static const uint8_t COMMAND_PROTECT_SECTOR = 0x36;
static const uint8_t COMMAND_UNPROTECT_SECTOR = 0x39;

static const uint8_t SREG_EPE = 1 << 5;
static const uint8_t SREG_WPP = 1 << 4;
static const uint8_t SREG_BSY = 1 << 0;

static const TPin Pin_flash_cs =       {GPIOB, GPIO0};
static const TPin Pin_spi_mosi =       {GPIOA, GPIO7};
static const TPin Pin_spi_miso =       {GPIOA, GPIO6};
static const TPin Pin_spi_sck =        {GPIOA, GPIO5};
static const TPin Pin_lcd_cs =         {GPIOA, GPIO4};
static const TPin Pin_backlight =        {GPIOB, GPIO15};

static const int Blockno = 65;
static const uint8_t DataByte = 'S';

static void spiTransact(const uint8_t* out, uint8_t* in, int length)
{
  Pin_flash_cs.Clear();
  for (int i=0; i < length; i++) {
      in[i] = spi_xfer(SPI1, out[i]);
  }
  Pin_flash_cs.Set();
}

void mogrify(uint8_t* data, int length)
{
  for (int i=0; i < length; i++) {
      data[i]++;
  }
}

bool flashIdentify()
{
  uint8_t outBuffer[4];
  uint8_t inBuffer[4];
  outBuffer[0] = COMMAND_DEVICE_ID;
  outBuffer[1] = 0;
  outBuffer[2] = 0;
  outBuffer[3] = 0;

  spiTransact(outBuffer, inBuffer, 4);

  if (inBuffer[1] == 0x1f && inBuffer[2] == 0x45 && inBuffer[3] == 0x01) {
      // OK!
      return true;
  }
  else {
      // Not ok
      return false;
  }
}

int pollStatus()
{
  uint8_t outBuffer[3];
  uint8_t inBuffer[3];
  outBuffer[0] = COMMAND_READ_STATUS;
  outBuffer[1] = 0;
  outBuffer[2] = 0;

  int count = 0;
  while (true) {
    count++;
    spiTransact(outBuffer, inBuffer, 3);

    if ((inBuffer[1] & SREG_WPP) && (~inBuffer[1] & SREG_BSY)) {
        return count;
    }
  }
  return -1;
}

bool flashWrite()
{
  const int ChunkSize = 128;
  uint8_t outBuffer[ChunkSize + 4];
  uint8_t inBuffer[ChunkSize + 4];
  uint8_t* const outData = outBuffer + 4;
  uint8_t* const inData = inBuffer + 4;

  memset(outData, DataByte, ChunkSize);
  memset(inData, 0xaa, ChunkSize);

  const uint8_t page = Blockno;

  /*
   * Write enable command
   */

  outBuffer[0] = COMMAND_WRITE_ENABLE;

  spiTransact(outBuffer, inBuffer, 1);

  pollStatus();

  /*
   * Unprotect command
   */

  outBuffer[0] = COMMAND_UNPROTECT_SECTOR;
  outBuffer[1] = page >> 4;
  outBuffer[2] = ((page & 0x0f) << 4);
  outBuffer[3] = 0;

  spiTransact(outBuffer, inBuffer, 4);

  pollStatus();

  /*
   * Write enable command
   */

  outBuffer[0] = COMMAND_WRITE_ENABLE;

  spiTransact(outBuffer, inBuffer, 1);

  pollStatus();

  /*
   * Erase command
   */

  outBuffer[0] = COMMAND_BLOCK_ERASE_4K;
  outBuffer[1] = page >> 4;
  outBuffer[2] = ((page & 0x0f) << 4);
  outBuffer[3] = 0;

  spiTransact(outBuffer, inBuffer, 4);

  pollStatus();

  for (int chunkNumber = 0; chunkNumber * ChunkSize < 4096; chunkNumber++) {
    const uint16_t byteno = chunkNumber * ChunkSize;

    /*
     * Write enable command
     */

    outBuffer[0] = COMMAND_WRITE_ENABLE;

    spiTransact(outBuffer, inBuffer, 1);

    pollStatus();

    /*
     * Program command
     */

    outBuffer[0] = COMMAND_PAGE_PROGRAM;
    outBuffer[1] = page >> 4;
    outBuffer[2] = ((page & 0x0f) << 4) | (byteno >> 8);
    outBuffer[3] = byteno & 0xff;

    spiTransact(outBuffer, inBuffer, ChunkSize + 4);

    pollStatus();
  }

  return true;
}

bool flashRead()
{
  const int ChunkSize = 4096;
  uint8_t outBuffer[ChunkSize + 4];
  uint8_t inBuffer[ChunkSize + 4];

  const uint8_t page = Blockno;
  const uint16_t byteno = 0;

  outBuffer[0] = COMMAND_READ_ARRAY;
  outBuffer[1] = page >> 4;
  outBuffer[2] = ((page & 0x0f) << 4) | (byteno >> 8);
  outBuffer[3] = byteno & 0xff;

  spiTransact(outBuffer, inBuffer, ChunkSize + 4);

  if (inBuffer[4] == DataByte && inBuffer[5] == DataByte && inBuffer[6] == DataByte) {
      // OK!
      return true;
  }
  else {
      // Not ok
      return false;
  }
}

int main()
{
  rcc_clock_setup_in_hsi_out_48mhz();
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_SPI1EN); // SPI1
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN); // GPIOA
  rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN); // GPIOB

  Pin_backlight.Clear();
  Pin_backlight.SetOutput();

  // SPI
  Pin_spi_mosi.SetOutput(GPIO_MODE_OUTPUT_50_MHZ,
                         GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
  Pin_spi_sck.SetOutput(GPIO_MODE_OUTPUT_50_MHZ,
                        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
  Pin_flash_cs.Set();
  Pin_flash_cs.SetOutput();
  Pin_lcd_cs.Set();
  Pin_lcd_cs.SetOutput();

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

  int oks = 0;
  oks += flashIdentify();
  oks += flashWrite();
  oks += flashRead();

  Pin_backlight.SetOutput();
  if (oks == 3) {
      Pin_backlight.Set();
  }
  else {
      Pin_backlight.Clear();
  }

  return 0;
}
