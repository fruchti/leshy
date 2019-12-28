#pragma once

#include "stm32f030x6.h"

#include "pinning.h"

#define LCD_DELAY_BOOT          15000
#define LCD_DELAY_ENABLE        20
#define LCD_DELAY_WRITE_DATA    46
#define LCD_DELAY_COMMAND       42

#define LCD_DELAY_SOFT_RESET_1  5000
#define LCD_DELAY_SOFT_RESET_2  1000
#define LCD_DELAY_SOFT_RESET_3  1000
#define LCD_DELAY_4_BIT_MODE    5000

#define LCD_DELAY_CLEAR         2000
#define LCD_DELAY_HOME          2000

#define LCD_DDADR_LINE0         0x00
#define LCD_DDADR_LINE1         0x40
#define LCD_DDADR_LINE2         0x10
#define LCD_DDADR_LINE3         0x50

void LCD_Init(void);

// Clear LCD contents
void LCD_Clear(void);

// Set cursor to top-left corner
void LCD_Home(void);

// Set cursor to position
void LCD_SetCursor(unsigned int col, unsigned int row);

// Write a single character to the LCD
void LCD_PutChar(uint8_t data);

// Write a string to the LCD
void LCD_PutString(const char *data);

// Add a user-defined glyph (code 0-7) to the display's CGRAM
void LCD_SaveGlyph(uint8_t code, const uint8_t *bitmap);

// Transfer a command to the display
void LCD_Command(uint8_t data);

// Clear display (0b00000001)
#define LCD_CLEAR_DISPLAY       0x01

// Cursor home (0b0000001x)
#define LCD_CURSOR_HOME         0x02

// Set entry mode (0b000001xx)
#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_DECREASE      0x00
#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01

// Set display (0b00001xxx)
#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01

// Set shift (0b0001xxxx)
#define LCD_SET_SHIFT           0x10

#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04

// Set function (0b001xxxxx)
#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00
#define LCD_FUNCTION_5X10       0x04

#define LCD_SOFT_RESET          0x30

// Set character generator RAM address (0b01xxxxxx)
#define LCD_SET_CGADR           0x40

#define LCD_GC_CHAR0            0
#define LCD_GC_CHAR1            1
#define LCD_GC_CHAR2            2
#define LCD_GC_CHAR3            3
#define LCD_GC_CHAR4            4
#define LCD_GC_CHAR5            5
#define LCD_GC_CHAR6            6
#define LCD_GC_CHAR7            7

// Set display data RAM address (0b1xxxxxxx)
#define LCD_SET_DDADR           0x80
