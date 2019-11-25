#include "main.h"

int SetTemperatureDisplayCounter = 0;
int DisplayUpdateCounter = 0;

int main(void)
{
    LCD_Init();

    const uint8_t heart[] =
    {
        0b00000,
        0b00000,
        0b01010,
        0b11111,
        0b01110,
        0b00100,
        0b00000,
        0b00000,
    };
    LCD_SaveGlyph(0, heart);

    LCD_Home();
    LCD_PutString("leshy HD44780  ");
    LCD_PutChar(0);
    LCD_SetCursor(0, 1);
    LCD_PutChar(0);
    LCD_PutString("   demo project");

    while(1)
    {
        __WFI();
    }

    return 0;
}
