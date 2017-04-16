#include "main.h"

QueueHandle_t DurationQueue;

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    GPIOA->MODER |= (1 << (PIN_LED_1 << 1)) | (1 << (PIN_LED_2 << 1)) | (1 << (PIN_LED_3 << 1)) | (1 << (PIN_LED_4 << 1)) | (1 << (PIN_LED_5 << 1));
    GPIOA->PUPDR |= (1 << (PIN_BUTTON << 1));

    DurationQueue = xQueueCreate(2, sizeof(TickType_t));

    xTaskCreate(BlinkTask, "Blink", 50, NULL, 2, NULL);
    xTaskCreate(ButtonTask, "Button", 50, NULL, 3, NULL);
    xTaskCreate(DebugTask, "Debug", 100, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1)
    {
    }

    return 0;
}

static void BlinkTask(void *param)
{
    TickType_t lastwaketime = xTaskGetTickCount();
    for(;;)
    {
        vTaskDelayUntil(&lastwaketime, 500);
        GPIOA->BSRR = (1 << PIN_LED_1);
        vTaskDelayUntil(&lastwaketime, 500);
        GPIOA->BRR = (1 << PIN_LED_1);
    }
}

static void ButtonTask(void *param)
{
    TickType_t lastwaketime = xTaskGetTickCount();
    TickType_t pressdowntime = lastwaketime;
    for(;;)
    {
        vTaskDelayUntil(&lastwaketime, 10);
        if(GPIOA->IDR & (1 << PIN_BUTTON))
        {
            if(GPIOA->ODR & (1 << PIN_LED_2))
            {
                pressdowntime = xTaskGetTickCount() - pressdowntime;
                xQueueSend(DurationQueue, &pressdowntime, portMAX_DELAY);
            }
            GPIOA->BRR = (1 << PIN_LED_2);
        }
        else
        {
            if(~GPIOA->ODR & (1 << PIN_LED_2))
            {
                pressdowntime = xTaskGetTickCount();
            }
            GPIOA->BSRR = (1 << PIN_LED_2);
        }
    }
}

static void DebugTask(void *param)
{
    TickType_t pressdowntime;
    for(;;)
    {
        xQueueReceive(DurationQueue, &pressdowntime, portMAX_DELAY);
        char buffer[12];
        itoa((int)pressdowntime, buffer, 10);
        Debug_Print("The button was pressed down for ");
        Debug_Print(buffer);
        Debug_Print(" ticks.\n");
    }
}
