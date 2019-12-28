#pragma once

#include <stdbool.h>
#include <string.h>

#include "stm32f0xx.h"
#include "pinning.h"

#define WS2812B_INVERT_DO       0

#define WS2812B_ZERO_LENGTH     19
#define WS2812B_ONE_LENGTH      38

typedef struct
{
    uint8_t g;
    uint8_t r;
    uint8_t b;
} __attribute__((packed, aligned(1))) WS2812B_Colour_t;

extern WS2812B_Colour_t WS2812B_Colour;
extern int WS2812B_LEDCount;
extern volatile bool WS2812B_Transferring;

void WS2812B_Init(void);
void WS2812B_StartTransfer(void);
