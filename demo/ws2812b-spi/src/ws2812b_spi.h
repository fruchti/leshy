#ifndef WS2812B_SPI_H
#define WS2812B_SPI_H

#include "stm32f030x6.h"
#include "pinning.h"

// The WS2812B data line has to be connected to PA7 (SPI MOSI). Unfortunately,
// there is only one SPI interface, so this library cannot be used together with
// an SD card.
#define LED_PIN_DO          7


// The number of LEDs daisy-chained
#define LED_COUNT           12

// Invert the output (if a transistor is used for level-shifting)
//#define LED_INVERT_DO

#define LED_DMA_BYTES       (LED_COUNT * 3 * 4)

typedef struct
{
    uint8_t G, R, B;
} LED_Color_t;

extern volatile LED_Color_t LED_Buffer[LED_COUNT];

void LED_Init(void);
void LED_Refresh(void);

#endif
