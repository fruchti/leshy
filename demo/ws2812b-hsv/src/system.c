#include <stdint.h>
#include "stm32f0xx.h"

void SystemInit(void)
{
    // Activate HSI and wait for it to be ready
    // RCC->CR = RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    // Set PLL to x12 (-> 40MHz system clock)
    RCC->CFGR = RCC_CFGR_PLLMUL12;

    // Activate PLL and wait
    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY));

    // Select PLL as clock source
    RCC->CFGR |= RCC_CFGR_SW_1;

    // Disable all interrupts
    RCC->CIR = 0x00000000;
}

