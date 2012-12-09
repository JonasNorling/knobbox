#include "../libopencm3/include/libopencm3/stm32/f1/rcc.h"
#include "../device.h"


int main()
{
    clockInit();

    Pin_shift_out_load.SetOutput();
    Pin_shift_out_en.Set();
    Pin_shift_out_en.SetOutput();
    Pin_shift_in_load.SetOutput();
    Pin_shift_in_en.SetOutput();

    Pin_shift_out.SetOutput(GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);
    Pin_shift_clk.SetOutput(GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL);

    // Turn off backlight, pullups
    Pin_vpullup.Clear();
    Pin_vpullup.SetOutput();

    usart_set_baudrate(USART1, 1000000);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    // Output clock, also on last bit
    USART_CR2(USART1) |= USART_CR2_CLKEN | USART_CR2_LBCL;
    usart_enable(USART1);

    delay_ms(500);

    Pin_vpullup.Set(); // Turn on backlight, pullups
    Pin_shift_out_en.Clear(); // Turn on LED driver
    Pin_shift_out_load.Clear();
    Pin_shift_in_en.Clear();


    const int ShiftLength = 2;
    static const uint8_t Knobs = 4;
    enum Color {
      COLOR_RED = 0,
      COLOR_GREEN = 1
    };

    uint8_t SwitchData[ShiftLength] = {0, 0};
    uint8_t CurrentPwmStep = 0;
    uint8_t LedControl[ShiftLength];
    uint8_t LedIntensity[2][Knobs];

    LedIntensity[COLOR_RED][0] = 0xff;
    LedIntensity[COLOR_GREEN][0] = 0x20;
    LedIntensity[COLOR_RED][1] = 0x80;
    LedIntensity[COLOR_GREEN][1] = 0x40;
    LedIntensity[COLOR_RED][2] = 0x40;
    LedIntensity[COLOR_GREEN][2] = 0x80;
    LedIntensity[COLOR_RED][3] = 0x20;
    LedIntensity[COLOR_GREEN][3] = 0xff;

    // Initialize DMA channels
    const uint32_t dma = DMA1;
    const uint32_t txchannel = DMA_CHANNEL4;
    const uint32_t rxchannel = DMA_CHANNEL5;
    DMA_CPAR(dma, rxchannel) = reinterpret_cast<uint32_t>(&USART1_DR); // peripheral addr
    DMA_CMAR(dma, rxchannel) = reinterpret_cast<uint32_t>(SwitchData); // memory addr
    DMA_CPAR(dma, txchannel) = reinterpret_cast<uint32_t>(&USART1_DR); // peripheral addr
    DMA_CMAR(dma, txchannel) = reinterpret_cast<uint32_t>(LedControl); // memory addr

    usart_recv(USART1);
    usart_recv(USART1);

#define USE_DMA

    while (true) {
#ifdef USE_DMA
        // Disable channels
        DMA_CCR(dma, txchannel) &= ~DMA_CCR_EN;
        DMA_CCR(dma, rxchannel) &= ~DMA_CCR_EN;
        // Reset interrupts
        DMA_IFCR(dma) |= DMA_IFCR_CIF(txchannel) | DMA_IFCR_CIF(rxchannel);
#endif

        // Load last data to LED drivers
        Pin_shift_out_load.Set();
        Pin_shift_out_load.Clear();

        // Sample encoders
        Pin_shift_in_load.Clear();
        Pin_shift_in_load.Set();

#ifdef USE_DMA
        DMA_CCR(dma, rxchannel) = (DMA_CCR_PL_HIGH | // prio
                                   DMA_CCR_MSIZE_8BIT |
                                   DMA_CCR_PSIZE_8BIT |
                                   DMA_CCR_MINC | // memory increment mode
                                   0/*DMA_CCR_TCIE*/ // transfer complete interrupt
                                   );
        DMA_CNDTR(dma, rxchannel) = ShiftLength; // number of data

        DMA_CCR(dma, txchannel) = (DMA_CCR_PL_LOW | // prio
                                   DMA_CCR_MSIZE_8BIT |
                                   DMA_CCR_PSIZE_8BIT |
                                   DMA_CCR_MINC | // memory increment mode
                                   DMA_CCR_DIR // read from memory
                                   );
        DMA_CNDTR(dma, txchannel) = ShiftLength; // number of data

        // Enable DMAs
        DMA_CCR(dma, rxchannel) |= DMA_CCR_EN;
        DMA_CCR(dma, txchannel) |= DMA_CCR_EN;
        // Enable RX/TX DMA, kicks off the transfer
        USART_CR3(USART1) |= USART_CR3_DMAR | USART_CR3_DMAT;
#else
        // Clock out/in data
        usart_send_blocking(USART1, LedControl[0]);
        SwitchData[0] = usart_recv_blocking(USART1); // Read encoder positions
        usart_send_blocking(USART1, LedControl[1]);
        SwitchData[1] = usart_recv_blocking(USART1); // Read encoder push buttons
#endif

        // Calculate the next PWM step.
        uint8_t value = 0;
        for (int bit = 0; bit < 8; bit++) {
          const uint8_t color = bit & 0x01;
          const uint8_t knob = bit >> 1;
          value = (value << 1) | (LedIntensity[color][knob] > CurrentPwmStep);
        }
        LedControl[0] = 0;
        LedControl[1] = value;
        CurrentPwmStep++;

#ifdef USE_DMA
        while (!(DMA_ISR(dma) & DMA_ISR_TCIF5));
#endif
    }
}
