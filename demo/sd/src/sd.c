#include "sd.h"

void SD_Init(void)
{
    SysTick_Config(48000000 / 1000 - 1);
}

int SD_DetectMedia(void)
{
    return !(GPIOB->IDR & (1 << SD_PIN_SWITCH));
}

void SysTick_Handler(void)
{
    disk_timerproc();
}
