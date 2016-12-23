#include "main.h"

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;

    GPIOA->MODER |= (1 << (PIN_LED_1 << 1)) | (1 << (PIN_LED_2 << 1)) | (1 << (PIN_LED_3 << 1)) | (1 << (PIN_LED_4 << 1)) | (1 << (PIN_LED_5 << 1));
    GPIOA->PUPDR |= (1 << (PIN_BUTTON << 1));

    Debug_Print("System up and running!\n");
    char buffer[50] = "This is build number ";
    itoa(BUILD_NUMBER, buffer + strlen(buffer), 10);
    buffer[strlen(buffer)] = '\n';
    buffer[strlen(buffer) + 1] = 0;
    Debug_Print(buffer);
    strcpy(buffer, "The build date is ");
    itoa(BUILD_DATE, buffer + strlen(buffer), 10);
    buffer[strlen(buffer)] = '\n';
    buffer[strlen(buffer) + 1] = 0;
    Debug_Print(buffer);

    while(1)
    {
        while(GPIOA->IDR & (1 << PIN_BUTTON));
        GPIOA->BSRR |= (1 << PIN_LED_1);
        Debug_Print("Button is pressed.\n");

        while(~GPIOA->IDR & (1 << PIN_BUTTON));
        GPIOA->BRR |= (1 << PIN_LED_1);
        Debug_Print("Button is released.\n");
    }

    return 0;
}
