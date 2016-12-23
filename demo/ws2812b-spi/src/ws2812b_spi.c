#include "ws2812b_spi.h"

volatile LED_Color_t LED_Buffer[LED_COUNT];
volatile uint8_t LED_DMABuffer[LED_DMA_BYTES + 1];
#ifndef LED_INVERT_DO
const static uint8_t LED_PatternTable[4] = {0b100100, 0b100110, 0b110100, 0b110110};
#else
const static uint8_t LED_PatternTable[4] = {0b011011, 0b011001, 0b001011, 0b001001};
#endif

void LED_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_DMAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // GPIO initialisation
    GPIOA->MODER |= (2 << (LED_PIN_DO << 1));

    // SPI initialisation
    SPI1->CR1 = SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE |\
                SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_MSTR;
    SPI1->CR2 = SPI_CR2_DS_2 | SPI_CR2_DS_0 | SPI_CR2_FRF;

    // DMA configuration (DMA channel 3 is used)
    DMA1_Channel3->CCR = DMA_CCR_PL_1 | DMA_CCR_MINC | DMA_CCR_DIR;
    DMA1_Channel3->CPAR = (uint32_t)(&SPI1->DR);
    SPI1->CR2 |= SPI_CR2_TXDMAEN;

    // Enable SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}

static inline void LED_StartDMA(void)
{
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
    DMA1_Channel3->CNDTR = LED_DMA_BYTES + 1;
    DMA1_Channel3->CMAR = (uint32_t)(&LED_DMABuffer);
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void LED_Refresh(void)
{
    for(unsigned int i = 0; i < LED_DMA_BYTES; i++)
    {
        unsigned int byteindex = i / 4;
        uint8_t bitindex = 6 - (i - 4 * byteindex) * 2;
        uint8_t bitmask = 3 << bitindex;
        uint8_t *byte = (uint8_t*)LED_Buffer + byteindex;
        LED_DMABuffer[i + 1] = LED_PatternTable[(*byte & bitmask) >> bitindex];
    }
#ifdef LED_INVERT_DO
    LED_DMABuffer[0] = 0xff;
#else
    LED_DMABuffer[0] = 0;
#endif
    LED_StartDMA();
}
