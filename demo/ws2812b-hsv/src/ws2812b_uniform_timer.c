#include "ws2812b_uniform_timer.h"

// Globally accessible variable for setting the colour.
WS2812B_Colour_t WS2812B_Colour;

// Current colour used while transferring the data to the LEDs
static WS2812B_Colour_t WS2812B_CurrentColour;

// Number of LEDs to control
int WS2812B_LEDCount = 60 * 10;

uint16_t WS2812B_DMABuffer[48];     // Internal DMA buffer, 2 * 1 byte per bit
static volatile int WS2812B_CurrentLED;    // Transfer position in the LED string
volatile bool WS2812B_Transferring; // True if a transfer is ongoing

static void WS2812B_UpdateBuffer(void)
{
    int start = 0;
    int led = WS2812B_CurrentLED;

    // If the current LED is odd, the upper half of the buffer has to be filled
    if(led & 1)
    {
        start = 24;
    }

    if(led >= WS2812B_LEDCount)
    {
        // All data has been sent => zero the buffer
        for(int i = 0; i < 24; i++)
        {
            WS2812B_DMABuffer[start + i] = 0;
        }
        return;
    }

    uint8_t *raw = (uint8_t*)(&WS2812B_CurrentColour);
    for(int i = 0; i < 24; i++)
    {
        if(raw[i / 8] & (0x80 >> (i % 8)))
        {
            WS2812B_DMABuffer[start + i] = WS2812B_ONE_LENGTH;
        }
        else
        {
            WS2812B_DMABuffer[start + i] = WS2812B_ZERO_LENGTH;
        }
    }
}

void WS2812B_StartTransfer(void)
{
    while(WS2812B_Transferring);
    WS2812B_Transferring = true;

    WS2812B_CurrentColour = WS2812B_Colour;

    // Fill buffer
    WS2812B_CurrentLED = 0;
    WS2812B_UpdateBuffer();
    WS2812B_CurrentLED = 1;
    WS2812B_UpdateBuffer();

    // Start DMA
    DMA1_Channel5->CMAR = (uint32_t)&(*WS2812B_DMABuffer);
    DMA1_Channel5->CCR = DMA_CCR_PL |           // Top priority
        DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 |     // 16-bit transfers
        DMA_CCR_MINC | DMA_CCR_CIRC |           // Memory increment, circular
        DMA_CCR_DIR |                           // Memory to peripheral
        DMA_CCR_HTIE | DMA_CCR_TCIE |           // Interrupts
        DMA_CCR_EN;                             // Enable DMA

    // Start timer
    TIM1->CR1 = TIM_CR1_URS | TIM_CR1_CEN;
}

void WS2812B_Init(void)
{
    WS2812B_Colour.r = 0;
    WS2812B_Colour.g = 0;
    WS2812B_Colour.b = 0;

    //// ---- CLOCKS ---- ////
    // Output is PA10, TIM1_CH3
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    //// ---- GPIO ---- ////
    // PA10 in open-drain alternate function mode, full speed output
    GPIOA->MODER = (GPIOA->MODER
        & ~(3 << (PIN_LED_DO * 2)))
        | (2 << (PIN_LED_DO * 2));
    GPIOA->OTYPER |= (1 << PIN_LED_DO);
    GPIOA->OSPEEDR |= (3 << PIN_LED_DO * 2);
    GPIOA->AFR[1] = (GPIOA->AFR[1]
        & ~(0x0f << (PIN_LED_DO * 4 - 32)))
        | (2 << (PIN_LED_DO * 4 - 32));

    //// ---- TIMER ---- ////
    // Set prescaler
    TIM1->PSC = 0;
    TIM1->ARR = WS2812B_ZERO_LENGTH + WS2812B_ONE_LENGTH;
    TIM1->CCR3 = 0;
    // Enable  Output compare
    TIM1->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE
        | TIM_CCMR1_OC2FE;
    TIM1->BDTR = TIM_BDTR_MOE;
#if WS2812B_INVERT_DO
    TIM1->CCER = TIM_CCER_CC2E | TIM_CCER_CC2P;
#else
    TIM1->CCER = TIM_CCER_CC3E;
#endif
    // Enable update DMA request
    TIM1->DIER = TIM_DIER_UDE;
    // DMA base address
    TIM1->DCR = ((uint32_t)&(TIM1->CCR3) - (uint32_t)&(TIM1->CR1)) / 4;
    // Enabble TIM1
    TIM1->CR1 = TIM_CR1_URS | TIM_CR1_CEN;

    //// ---- DMA ---- ////
    // Timer 1 update is DMA1 channel 5
    for(int i = 0; i < sizeof(WS2812B_DMABuffer) / 2; i++)
    {
        WS2812B_DMABuffer[i] = WS2812B_ZERO_LENGTH;
    }
    DMA1_Channel5->CNDTR = sizeof(WS2812B_DMABuffer) / 2;
    DMA1_Channel5->CPAR = (uint32_t)&(TIM1->DMAR);
    // DMA1_Channel5->CPAR = (uint32_t)&(TIM1->CCR3);

    NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
    NVIC_SetPriority(DMA1_Channel4_5_IRQn, 0);
}

void DMA1_Channel4_5_IRQHandler(void)
{
    // 24 bits = data for one LED has been transferred
    WS2812B_CurrentLED++;

    // Stop the DMA only after a few LEDs after the end of the data buffer. The
    // routine filling the timer buffer (the for the individual pulse lengths)
    // zeros it for everything beyond the LED count, so a buffer overflow is
    // not an issue. This guarantees a pause in the data stream, which signals
    // the WS2812Bs to latch the received data to their outputs.
    if(WS2812B_CurrentLED == WS2812B_LEDCount + 5)
    {
        DMA1_Channel5->CCR = 0;
        TIM1->CCR3 = 0;
        // Send the last pulse before stopping the timer
        TIM1->CR1 |= TIM_CR1_OPM;
        WS2812B_Transferring = false;
    }
    else
    {
        WS2812B_UpdateBuffer();
    }
    DMA1->IFCR = DMA_IFCR_CGIF5;
}
