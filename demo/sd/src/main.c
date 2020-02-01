#include "main.h"

#define INTERVAL 100       // Logging intverval in milliseconds

FATFS FatFs;
FIL logfile;
int logtime = 0;
int logging = 0;
int buttonready = 1;

void LED_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;

    GPIOA->MODER |= (1 << (PIN_LED_2 << 1)) | (1 << (PIN_LED_3 << 1))
        | (1 << (PIN_LED_4 << 1))| (1 << (PIN_LED_5 << 1));
}

static void StartLogging(void)
{
    SD_PowerOn();

    FRESULT rc;
    rc = f_mount(&FatFs, "", 1);
    if(rc == FR_OK)
    {
        rc = f_open(&logfile, "log.csv", FA_WRITE | FA_CREATE_ALWAYS);
        if(rc == FR_OK)
        {
            f_printf(&logfile,
                    "This file was written by a simple leshy SD card example. "
                    "More specifically, by build number %d, created on %d for "
                    "hardware revision " LESHY_HARDWARE_REVISION ".\n"
                    "The example provides a simple temperature logger:\n"
                    "Time (ms), Temperature (°C)\n", BUILD_NUMBER, BUILD_DATE);

            // SD card is mounted and the log file open.
            // Everything went OK, so LED 3 can be turned off
            GPIOA->BRR = (1 << PIN_LED_3);

            // Enable the timer
            TIM3->CR1 |= TIM_CR1_CEN;

            f_sync(&logfile);
            GPIOA->BSRR |= (1 << PIN_LED_2);
            logging = 1;
        }
    }
}

static void StopLogging(void)
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
    f_sync(&logfile);
    f_close(&logfile);
    GPIOA->BRR = (1 << PIN_LED_2);
    logging = 0;

    SD_PowerOff();
}

int main(void)
{
    LED_Init();
    // LED 3 should be lit up during the initialisation
    GPIOA->BSRR = (1 << PIN_LED_3);

    // Enable button pull-up
    GPIOA->PUPDR |= (1 << (PIN_BUTTON << 1));

    // The ADC is used for simple and inaccurate temperature measurements
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC1->CR |= ADC_CR_ADEN;
    while(~ADC1->ISR & ADC_ISR_ADRDY);
    ADC1->CHSELR = ADC_CHSELR_CHSEL16;
    ADC1->SMPR = 7;
    ADC->CCR |= ADC_CCR_TSEN;

    // It's time to initialise the timer for our logging purposes
    // It should issue an interrupt request every n seconds
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = INTERVAL - 1;
    TIM3->ARR = 48000;

    // Enable the overflow interrupt
    TIM3->DIER = TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM3_IRQn);

    // Start logging immediately
    StartLogging();

    while(1)
    {
        // Toggle logging on button presses
        if(GPIOA->IDR & (1 << PIN_BUTTON))
        {
            buttonready = 1;
        }
        else if(buttonready)
        {
            buttonready = 0;
            if(logging)
            {
                StopLogging();
            }
            else
            {
                StartLogging();
            }
        }
    }

    return 0;
}

void TIM3_IRQHandler(void)
{
    // Enable LED 5 to indicate the CPU's working
    GPIOA->BSRR = (1 << PIN_LED_5);

    ADC1->CR |= ADC_CR_ADSTART;
    while(ADC1->CR & ADC_CR_ADSTART);
    //int temp = ((1.43f - (ADC1->DR / 4095.0f * 3.3f)) / 4.3e-3f + 25.0f) * 100;
    int temp = (3575580 - 1874 * ADC1->DR) / 100;

    f_printf(&logfile, "%08d, %d.%02d\n", logtime * INTERVAL, temp / 100, temp % 100);
    logtime++;

    // Disable LED 5 again and clear the interrupt flag
    GPIOA->BRR = (1 << PIN_LED_5);
    TIM3->SR &= ~TIM_SR_UIF;
}
