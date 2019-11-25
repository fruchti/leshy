#include "hd44780.h"

static void LCD_Delay(uint32_t microseconds)
{
    // Caution: Clock frequency dependent
    uint32_t cycles = 48 * microseconds / 3;
    // Subtraction takes 1 cycle, the branch 2
    __asm__(".syntax unified\n"
            "L: \n"
            "subs %[cycles], %[cycles], #1\n"
            "bne L\n"
            ".syntax divided"
            :
            : [cycles] "r" (cycles));
}

const uint8_t LCD_LineDDADR[] =
{
    LCD_DDADR_LINE0,
    LCD_DDADR_LINE1,
    LCD_DDADR_LINE2,
    LCD_DDADR_LINE3
};

static void LCD_Enable(void)
{
    GPIOA->BSRR = (1 << PIN_LCD_EN);
    LCD_Delay(LCD_DELAY_ENABLE);
    GPIOA->BRR = (1 << PIN_LCD_EN);
}

static void LCD_Nibble(uint8_t data)
{
    uint32_t odr = GPIOA->ODR;
    // This may look horribly inefficient, but the compiler is able to optimise
    // every unneccessary complication away
    odr &= ~(1 << PIN_LCD_D7) & ~(1 << PIN_LCD_D6) & ~(1 << PIN_LCD_D5)
        & ~(1 << PIN_LCD_D4);
    odr |= ((data & 0x80) ? (1 << PIN_LCD_D7) : 0)
        | ((data & 0x40) ? (1 << PIN_LCD_D6) : 0)
        | ((data & 0x20) ? (1 << PIN_LCD_D5) : 0)
        | ((data & 0x10) ? (1 << PIN_LCD_D4) : 0);
    GPIOA->ODR = odr;

    LCD_Enable();
}

void LCD_Init(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~(3 << (PIN_LCD_EN << 1)) & ~(3 << (PIN_LCD_RS << 1))
        & ~(3 << (PIN_LCD_D7 << 1)) & ~(3 << (PIN_LCD_D6 << 1))
        & ~(3 << (PIN_LCD_D5 << 1)) & ~(3 << (PIN_LCD_D4 << 1));
    GPIOA->MODER |= (1 << (PIN_LCD_EN << 1)) | (1 << (PIN_LCD_RS << 1))
        | (1 << (PIN_LCD_D7 << 1)) | (1 << (PIN_LCD_D6 << 1))
        | (1 << (PIN_LCD_D5 << 1)) | (1 << (PIN_LCD_D4 << 1));

    LCD_Delay(LCD_DELAY_BOOT);

    LCD_Nibble(LCD_SOFT_RESET);
    LCD_Delay(LCD_DELAY_SOFT_RESET_1);

    LCD_Enable();
    LCD_Delay(LCD_DELAY_SOFT_RESET_2);

    LCD_Enable();
    LCD_Delay(LCD_DELAY_SOFT_RESET_3);

    LCD_Nibble(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT);
    LCD_Delay(LCD_DELAY_4_BIT_MODE);

    LCD_Command(LCD_SET_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINE
        | LCD_FUNCTION_5X7);
    LCD_Command(LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_OFF
        | LCD_BLINKING_OFF); 
    LCD_Command(LCD_SET_ENTRY | LCD_ENTRY_INCREASE | LCD_ENTRY_NOSHIFT);

    LCD_Clear();
}

void LCD_PutChar(uint8_t data)
{
    GPIOA->BSRR = (1 << PIN_LCD_RS);

    LCD_Nibble(data);
    LCD_Nibble(data << 4);

    LCD_Delay(LCD_DELAY_WRITE_DATA);
}

void LCD_Command(uint8_t data)
{
    GPIOA->BRR = (1 << PIN_LCD_RS);

    LCD_Nibble(data);
    LCD_Nibble(data << 4);

    LCD_Delay(LCD_DELAY_COMMAND);
}

void LCD_Clear(void)
{
    LCD_Command(LCD_CLEAR_DISPLAY);
    LCD_Delay(LCD_DELAY_CLEAR);
}

void LCD_Home(void)
{
    LCD_Command(LCD_CURSOR_HOME);
    LCD_Delay(LCD_DELAY_HOME);
}
 
void LCD_SetCursor(unsigned int col, unsigned int row)
{
    uint8_t data;

    if(row < sizeof(LCD_LineDDADR))
    {
        data = LCD_SET_DDADR | LCD_LineDDADR[row] | (col & 0x0f);
    }
    else
    {
        return;
    }

    LCD_Command(data);
}

void LCD_PutString(const char *data)
{
    while(*data != '\0')
    {
        LCD_PutChar(*data++);
    }
}
 
void LCD_SaveGlyph(uint8_t code, const uint8_t *bitmap)
{
    LCD_Command(LCD_SET_CGADR | (code << 3));

    for(uint8_t i = 0; i < 8; i++)
    {
        LCD_PutChar(bitmap[i]);
    }
}
