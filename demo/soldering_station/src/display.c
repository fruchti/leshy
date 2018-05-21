#include "display.h"
#include "config.h"

bool Display_Blinking = false;
unsigned int Display_Brightness = 900; // Value between  0 and 1000

static const uint8_t Display_SegmentsTable[] =
{
    DISPLAY_SEG_A | DISPLAY_SEG_B | DISPLAY_SEG_C | DISPLAY_SEG_D   // 0
        | DISPLAY_SEG_E | DISPLAY_SEG_F,
    DISPLAY_SEG_B | DISPLAY_SEG_C,                                  // 1
    DISPLAY_SEG_A | DISPLAY_SEG_B | DISPLAY_SEG_D | DISPLAY_SEG_E   // 2
        | DISPLAY_SEG_G,
    DISPLAY_SEG_A | DISPLAY_SEG_B | DISPLAY_SEG_C | DISPLAY_SEG_D   // 3
        | DISPLAY_SEG_G,
    DISPLAY_SEG_B | DISPLAY_SEG_C | DISPLAY_SEG_F | DISPLAY_SEG_G,  // 4
    DISPLAY_SEG_A | DISPLAY_SEG_C | DISPLAY_SEG_D | DISPLAY_SEG_F   // 5
        | DISPLAY_SEG_G,
    DISPLAY_SEG_A | DISPLAY_SEG_C | DISPLAY_SEG_D | DISPLAY_SEG_E   // 6
        | DISPLAY_SEG_F | DISPLAY_SEG_G,
    DISPLAY_SEG_A | DISPLAY_SEG_B | DISPLAY_SEG_C,                  // 7
    DISPLAY_SEG_A | DISPLAY_SEG_B | DISPLAY_SEG_C | DISPLAY_SEG_D   // 8
        | DISPLAY_SEG_E | DISPLAY_SEG_F | DISPLAY_SEG_G,
    DISPLAY_SEG_A | DISPLAY_SEG_B | DISPLAY_SEG_C | DISPLAY_SEG_D   // 9
        | DISPLAY_SEG_F | DISPLAY_SEG_G
};

static uint8_t Display_Segments[DISPLAY_DIGITS];

static inline void Display_SetOutputs(int digit, unsigned segments)
{
    const uint16_t pintable[][DISPLAY_DIGITS] =
    {
        {PIN_DISP_1_SEG_A, PIN_DISP_2_SEG_A, PIN_DISP_3_SEG_A},
        {PIN_DISP_1_SEG_B, PIN_DISP_2_SEG_B, PIN_DISP_3_SEG_B},
        {PIN_DISP_1_SEG_C, PIN_DISP_2_SEG_C, PIN_DISP_3_SEG_C},
        {PIN_DISP_1_SEG_D, PIN_DISP_2_SEG_D, PIN_DISP_3_SEG_D},
        {PIN_DISP_1_SEG_E, PIN_DISP_2_SEG_E, PIN_DISP_3_SEG_E},
        {PIN_DISP_1_SEG_F, PIN_DISP_2_SEG_F, PIN_DISP_3_SEG_F},
        {PIN_DISP_1_SEG_G, PIN_DISP_2_SEG_G, PIN_DISP_3_SEG_G},
        {PIN_DISP_1_DP, PIN_DISP_2_DP, PIN_DISP_3_DP}
    };
    const uint16_t commonpins[DISPLAY_DIGITS] =
    {
        PIN_DISP_1_COMMON, PIN_DISP_2_COMMON, PIN_DISP_3_COMMON
    };

    unsigned moder = GPIOA->MODER & ~DISPLAY_MODER_MASK;
    unsigned odr = GPIOA->ODR & ~DISPLAY_ODR_MASK;

    moder |= ((segments & DISPLAY_SEG_A) ? (1 << (pintable[0][digit]) * 2) : 0)
        | ((segments & DISPLAY_SEG_B) ? (1 << (pintable[1][digit]) * 2) : 0)
        | ((segments & DISPLAY_SEG_C) ? (1 << (pintable[2][digit]) * 2) : 0)
        | ((segments & DISPLAY_SEG_D) ? (1 << (pintable[3][digit]) * 2) : 0)
        | ((segments & DISPLAY_SEG_E) ? (1 << (pintable[4][digit]) * 2) : 0)
        | ((segments & DISPLAY_SEG_F) ? (1 << (pintable[5][digit]) * 2) : 0)
        | ((segments & DISPLAY_SEG_G) ? (1 << (pintable[6][digit]) * 2) : 0)
        | ((segments & DISPLAY_DP) ? (1 << (pintable[7][digit]) * 2) : 0)
        | (1 << (commonpins[digit] * 2));

#ifdef DISPLAY_CC
    odr |= ((segments & DISPLAY_SEG_A) ? (1 << pintable[0][digit]) : 0)
        | ((segments & DISPLAY_SEG_B) ? (1 << pintable[1][digit]) : 0)
        | ((segments & DISPLAY_SEG_C) ? (1 << pintable[2][digit]) : 0)
        | ((segments & DISPLAY_SEG_D) ? (1 << pintable[3][digit]) : 0)
        | ((segments & DISPLAY_SEG_E) ? (1 << pintable[4][digit]) : 0)
        | ((segments & DISPLAY_SEG_F) ? (1 << pintable[5][digit]) : 0)
        | ((segments & DISPLAY_SEG_G) ? (1 << pintable[6][digit]) : 0)
        | ((segments & DISPLAY_DP) ? (1 << pintable[7][digit]) : 0);
#else
    odr |= (1 << commonpins[digit]);
#endif

    GPIOA->ODR = odr;
    GPIOA->MODER = moder;
}

void Display_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    TIM14->PSC = 200;
    TIM14->ARR = 1000;
    TIM14->CCR1 = Display_Brightness;
    TIM14->DIER = TIM_DIER_UIE | TIM_DIER_CC1IE;
    NVIC_EnableIRQ(TIM14_IRQn);
    TIM14->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
}

void Display_Number(int value)
{
    if(value < 0)
    {
        value = 0;
    }
    else if(value > 999)
    {
        value = 999;
    }
    Display_Segments[0] = Display_SegmentsTable[value / 100];
    Display_Segments[1] = Display_SegmentsTable[(value / 10) % 10];
    Display_Segments[2] = Display_SegmentsTable[value % 10];
}

void Display_ShowSegments(uint8_t digit1, uint8_t digit2, uint8_t digit3)
{
    Display_Segments[0] = digit1;
    Display_Segments[1] = digit2;
    Display_Segments[2] = digit3;
}

void TIM14_IRQHandler(void)
{
    static int currentdigit = 0;
    static int testcounter = 0;
    static int blinkcounter = 0;
    if(TIM14->SR & TIM_SR_UIF)
    {
        uint8_t segments = Display_Segments[currentdigit];
        const int testtime = 20;
        const int testdelay = 20;
        if(testcounter < 7 * testtime + testdelay)
        {
            if(testcounter > testdelay)
            {
                segments = 1 << ((testcounter - testdelay) / testtime);
            }
            else
            {
                switch(currentdigit)
                {
                    case 0:
                        segments = Display_SegmentsTable[BUILD_NUMBER / 100];
                        break;
                    case 1:
                        segments = Display_SegmentsTable[BUILD_NUMBER / 10 % 10];
                        break;
                    case 2:
                        segments = Display_SegmentsTable[BUILD_NUMBER % 10];
                        break;
                }
            }
            testcounter++;
        }
        if(Display_Blinking)
        {
            blinkcounter++;
            blinkcounter %= CONFIG_BLINK_PERIOD;
            if(blinkcounter >= CONFIG_BLINK_PERIOD / 2)
            {
                TIM14->CCR1 = Display_Brightness / 4;
            }
            else
            {
                TIM14->CCR1 = Display_Brightness;
            }
        }
        else
        {
            TIM14->CCR1 = Display_Brightness;
        }
        Display_SetOutputs(currentdigit, segments);

        currentdigit++;
        currentdigit %= DISPLAY_DIGITS;

        TIM14->SR &= ~TIM_SR_UIF;
    }
    else if(TIM14->SR & TIM_SR_CC1IF)
    {
        Display_SetOutputs(currentdigit, 0);

        TIM14->SR &= ~TIM_SR_CC1IF;
    }
}
