#include "encoder.h"

volatile int Encoder_Delta;
volatile int Encoder_D = 0;
volatile int Encoder_Velocity = ENCODER_ACCEL;

#if ENCODER_RESOLUTION == 4
static const int8_t Encoder_Table[16] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};
#elif ENCODER_RESOLUTION == 2
static const int8_t Encoder_Table[16] = {0, 0, -1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, -1, 0, 0};
#else
static const int8_t Encoder_Table[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0};
#endif

void Encoder_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOFEN;

    GPIOF->PUPDR |= (1 << (PIN_ENC_A << 1)) | (1 << (PIN_ENC_B << 1));
}

void Encoder_Poll(void)
{
    static int8_t iodata = 0;
    iodata = (iodata << 2) & 0x0f;
    if(~GPIOF->IDR & (1 << PIN_ENC_A))
    {
        iodata |= 1;
    }
    if(~GPIOF->IDR & (1 << PIN_ENC_B))
    {
        iodata |= 2;
    }
    Encoder_D +=  Encoder_Table[iodata];
    if(Encoder_D < -1)
    {
        Encoder_D += 2;
        Encoder_Delta -= Encoder_Velocity;
        Encoder_Velocity = ENCODER_ACCEL;
    }
    else if(Encoder_D > 1)
    {
        Encoder_D -= 2;
        Encoder_Delta += Encoder_Velocity;
        Encoder_Velocity = ENCODER_ACCEL;
    }
    Encoder_Velocity--;
    if(Encoder_Velocity == 0)
    {
        Encoder_Velocity = 1;
    }
}

int Encoder_Read(void)
{
    int delta;
    __disable_irq();
    delta = Encoder_Delta;
    Encoder_Delta = 0;
    __enable_irq();
    return delta;
}
