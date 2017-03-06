#include "main.h"

volatile unsigned int counter = 0;
volatile bool inverse = false;

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    GPIOA->MODER |= (1 << (PIN_LED_1 << 1)) | (1 << (PIN_LED_2 << 1)) | (1 << (PIN_LED_3 << 1)) | (1 << (PIN_LED_4 << 1)) | (1 << (PIN_LED_5 << 1));
    GPIOA->PUPDR |= (1 << (PIN_BUTTON << 1));

    TIM14->PSC = 10000;
    TIM14->ARR = 2000;
    TIM14->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM14_IRQn);
    TIM14->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;

    while(1)
    {
        while(GPIOA->IDR & (1 << PIN_BUTTON));
        inverse = true;

        while(~GPIOA->IDR & (1 << PIN_BUTTON));
        inverse = false;
    }

    return 0;
}

void TIM14_IRQHandler(void)
{
    if(TIM14->SR & TIM_SR_UIF)
    {
        uint8_t pattern = 1 << counter;

        uint16_t odr = ((!!(pattern & 1)) << PIN_LED_1) |\
                       ((!!(pattern & 2)) << PIN_LED_2) |\
                       ((!!(pattern & 4)) << PIN_LED_3) |\
                       ((!!(pattern & 8)) << PIN_LED_4) |\
                       ((!!(pattern & 16)) << PIN_LED_5);

        GPIOA->BRR = ~odr;
        GPIOA->BSRR = odr;

        if(inverse)
            counter += 1;
        else
            counter += 5;
        counter %= 6;

        TIM14->SR &= ~TIM_SR_UIF;
    }
    NVIC_ClearPendingIRQ(TIM14_IRQn);
}
