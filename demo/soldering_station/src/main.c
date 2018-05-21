#include "main.h"

int SetTemperatureDisplayCounter = 0;
int DisplayUpdateCounter = 0;

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;

    TIM16->PSC = 47;
    TIM16->ARR = 1000;
    TIM16->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM16_IRQn);
    TIM16->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;

    Display_Init();
    PID_Init();
    Encoder_Init();

    while(1)
    {
        __WFI();
        int delta = Encoder_Read();
        if(delta != 0)
        {
            PID_SetTemperature += delta;
            if(PID_SetTemperature > CONFIG_MAX_TEMP)
            {
                PID_SetTemperature = CONFIG_MAX_TEMP;
            }
            else if(PID_SetTemperature < CONFIG_MIN_TEMP)
            {
                PID_SetTemperature = CONFIG_MIN_TEMP;
            }
            SetTemperatureDisplayCounter = CONFIG_SET_DISPLAY_TIME;
        }

        if(SetTemperatureDisplayCounter > 0)
        {
            Display_Number(PID_SetTemperature);
            Display_Blinking = true;
        }
        else
        {
            if(DisplayUpdateCounter == 0)
            {
                DisplayUpdateCounter = CONFIG_UPDATE_INTERVAL;
                if(PID_CurrentTemperature != -1)
                {
                    Display_Number(PID_CurrentTemperature);
                }
                else
                {
                    Display_ShowSegments(DISPLAY_SEG_G, DISPLAY_SEG_G,
                        DISPLAY_SEG_G);
                }
                Display_Blinking = false;
            }
        }
    }

    return 0;
}

void TIM16_IRQHandler(void)
{
    Encoder_Poll();

    if(SetTemperatureDisplayCounter > 0)
    {
        SetTemperatureDisplayCounter--;
    }
    if(DisplayUpdateCounter > 0)
    {
        DisplayUpdateCounter--;
    }

    TIM16->SR &= ~TIM_SR_UIF;
}