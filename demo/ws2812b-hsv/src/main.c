#include "main.h"

// Raw ADC conversion results. Updated in ADC end of conversion (EOC) ISR.
static volatile uint16_t ADC_Readings[3];

// Set to true in the ADC's EOC ISR every time new values for all 3 channels are
// available
static volatile bool ADC_Done = false;

int main(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    // ADC pins in analogue mode
    GPIOA->MODER |= (0x03 << PIN_POT_H * 2) | (0x03 << PIN_POT_S * 2)
        | (0x03 << PIN_POT_V * 2);

    // Start calibration
    ADC1->CR |= ADC_CR_ADCAL;
    // Wait for calibration to complete
    while(ADC1->CR & ADC_CR_ADCAL);
    // Activate ADC
    ADC1->CR |= ADC_CR_ADEN;
    // Enable end of conversion interrupt
    ADC1->IER = ADC_IER_EOCIE;
    // Set sampling time to maximum
    ADC1->SMPR = ADC_SMPR_SMP_2 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_0;
    // Enable continuous conversion mode, overrun management mode (overwrites the older conversion result)
    ADC1->CFGR1 = ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD;
    // Select channels
    ADC1->CHSELR = ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL2;
    // Enable interrupt
    NVIC_EnableIRQ(ADC1_IRQn);
    NVIC_SetPriority(ADC1_IRQn, 3);

    ADC1->CR |= ADC_CR_ADSTART;

    WS2812B_Init();

    float h = 0, s = 0, v = 0;

    for(;;)
    {
        __WFI();

        if(ADC_Done)
        {
            ADC_Done = false;

            uint16_t raw[3];
            raw[0] = ADC_Readings[0];
            raw[1] = ADC_Readings[1];
            raw[2] = ADC_Readings[2];
            
            // Apply some basic filtering to ADC inputs
            const float lambda = 0.90;
            h *= lambda;
            s *= lambda;
            v *= lambda;
            h += (1 - lambda) * raw[0] / 4096.0f * 360.0f;
            s += (1 - lambda) * raw[1] / 4096.0f;
            v += (1 - lambda) * raw[2] / 4096.0f;

            float r, g, b;
            float hh, p, q, t, ff;
            int i;

            if(s <= 0.0f)
            {
                r = v;
                g = v;
                b = v;
            }
            else
            {
                hh = h;
                if(hh >= 360.0f)
                {
                    hh = 0.0f;
                }
                hh /= 60.0f;
                i = (int)hh;
                ff = hh - i;
                p = v * (1.0f - s);
                q = v * (1.0f - (s * ff));
                t = v * (1.0f - (s * (1.0f - ff)));

                switch(i)
                {
                    case 0:
                        r = v;
                        g = t;
                        b = p;
                        break;

                    case 1:
                        r = q;
                        g = v;
                        b = p;
                        break;

                    case 2:
                        r = p;
                        g = v;
                        b = t;
                        break;

                    case 3:
                        r = p;
                        g = q;
                        b = v;
                        break;

                    case 4:
                        r = t;
                        g = p;
                        b = v;
                        break;

                    case 5:
                    default:
                        r = v;
                        g = p;
                        b = q;
                        break;
                }
            }

            WS2812B_Colour.r = r * 255;
            WS2812B_Colour.g = g * 255;
            WS2812B_Colour.b = b * 255;
            
            WS2812B_StartTransfer();
        }
    }

    return 0;
}

void ADC1_IRQHandler(void)
{
    static int channel = 0;
    ADC_Readings[channel++] = ADC1->DR;
    if(channel > 2)
    {
        channel = 0;
        ADC_Done = true;
    }
}
