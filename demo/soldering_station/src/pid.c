#include "pid.h"
#include "config.h"
#include "pinning.h"

volatile unsigned int PID_CurrentTemperature;
unsigned int PID_SetTemperature = CONFIG_DEFAULT_TEMP;

void PID_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;


    // Configure PA10 in alternate function mode. AF2 corresponds to TIM1_CH3
    GPIOA->MODER |= (2 << (PIN_PWM << 1));
    GPIOA->AFR[1] |= (2 << ((PIN_PWM << 2) - 32));
#ifdef PID_PWM_OC
    GPIOA->OTYPER |= (1 << PIN_PWM);
#endif

    // Configure PB1 in analog mode
    GPIOB->MODER |= (3 << (PIN_PTC << 1));


    // Deactivate ADC
    ADC1->CR &= ~ADC_CR_ADEN;

    // Start calibration and wait for completion
    ADC1->CR |= ADC_CR_ADCAL;
    while(ADC1->CR & ADC_CR_ADCAL);

    // (Re-)Activate ADC
    ADC1->CR |= ADC_CR_ADEN;
    ADC1->CFGR1 = 0;

    // Set sampling time
    ADC1->SMPR = ADC_SMPR_SMP_1 | ADC_SMPR_SMP_0;

    // Select channel 9 (PB1)
    ADC1->CHSELR = ADC_CHSELR_CHSEL9;

    // Start first conversion
    ADC1->CR |= ADC_CR_ADSTART;


    // Configure PWM frequency (assumes 48 MHz system clock)
    TIM1->PSC = 48000 / PID_PWM_FREQ - 1;
    TIM1->ARR = 1000;

    // Duty cycle should be 0 at the beginning
    TIM1->CCR3 = 0;

    // Enable outputs
    TIM1->BDTR |= TIM_BDTR_MOE;

    // Configure PWM mode 1, optionally invert output signal
    TIM1->CCMR2 = TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;
#ifdef PID_PWM_INVERT
    TIM1->CCER = TIM_CCER_CC3E | TIM_CCER_CC3P;
#else
    TIM1->CCER = TIM_CCER_CC3E;
#endif

    // Generate update event to load CCR3 value and remove update flag
    // afterwards
    TIM1->EGR = TIM_EGR_UG;
    TIM1->SR = 0;

    // Enable timer and update interrupt (used for control)
    TIM1->CR1 = TIM_CR1_CEN;
    TIM1->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    static float erri = 0;  // Error sum for I control
    static float erro = 0;  // Error from last interrupt for D control

    // Read conversion result from ADC (the conversion was started in PID_Init()
    // or the previous timer interrupt) and calculate temperature
    volatile unsigned int adc = ADC1->DR;
    
    if(adc < 3000)
    {
        PID_CurrentTemperature = 1.0f / PID_M * ((float)PID_REXT * (PID_ADCMAX)
            / (PID_ADCMAX - adc) - PID_REXT - PID_T);

        float err = (float)PID_SetTemperature - (float)PID_CurrentTemperature;
        erri += err;
        if(erri > PID_ILIMIT)
        {
            erri = PID_ILIMIT;
        }
        else if(erri < - PID_ILIMIT)
        {
            erri = - PID_ILIMIT;
        }

        float cout = PID_KP * err
            + PID_KI * erri * PID_TS
            + (err - erro) * PID_KD / PID_TS;
        cout = cout / PID_VOLTAGE;
        if(cout < 0)
        {
            cout = 0;
        }
        if(cout > 1)
        {
            cout = 1;
        }
        erro = err;

        TIM1->CCR3 = (int)(cout * 1000);
    }
    else
    {
        // No soldering iron connected

        PID_CurrentTemperature = -1;
        TIM1->CCR3 = 200;
    }

    // Start next conversion
    ADC1->CR |= ADC_CR_ADSTART;

    TIM1->SR &= ~TIM_SR_UIF;
}