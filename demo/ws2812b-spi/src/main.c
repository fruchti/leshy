#include "main.h"

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    GPIOA->MODER |= (1 << (PIN_LED_1 << 1));

    LED_Init();

    const uint8_t brightness = 5;
    for(int i = 0; i < LED_COUNT; i++)
    {
        LED_Buffer[i].R = i * brightness / LED_COUNT;
        LED_Buffer[i].G = (uint8_t)((i - LED_COUNT / 3) * brightness / LED_COUNT) % brightness;
        LED_Buffer[i].B = (uint8_t)((i - 2 * LED_COUNT / 3) * brightness / LED_COUNT) % brightness;
    }

    TIM14->PSC = 10000;
    TIM14->ARR = 500;
    TIM14->CNT = 0;
    TIM14->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM14_IRQn);
    TIM14->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;

    for(;;);

    return 0;
}

void TIM14_IRQHandler(void)
{
    if(TIM14->SR & TIM_SR_UIF)
    {
        GPIOA->BSRR = (1 << PIN_LED_1);

        LED_Color_t first = LED_Buffer[0];
        for(unsigned int i = 0; i < LED_COUNT; i++)
        {
            if(i == LED_COUNT - 1)
            {
                LED_Buffer[i].R = first.R;
                LED_Buffer[i].G = first.G;
                LED_Buffer[i].B = first.B;
            }
            else
            {
                LED_Buffer[i].R = LED_Buffer[i + 1].R;
                LED_Buffer[i].G = LED_Buffer[i + 1].G;
                LED_Buffer[i].B = LED_Buffer[i + 1].B;
            }
        }

        LED_Refresh();

        GPIOA->BRR = (1 << PIN_LED_1);

        TIM14->SR &= ~TIM_SR_UIF;
    }
}
