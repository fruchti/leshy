#pragma once

#include "config.h"

#define PIN_BUTTON              0   // PA0  - Push button
#define PIN_LED_1               1   // PA1  - Status LED
#define PIN_LED_2               2   // PA2  - Status LED
#define PIN_LED_3               3   // PA3  - Status LED
#define PIN_LED_4               9   // PA9  - Status LED
#define PIN_LED_5               10  // PA10 - Status LED

#if LESHY_HAS_SD_PMOS
// Adapt if the SD power MOSFET's gate is connected to another pin. Note that
// the code assumes the GPIO clock already enabled.
#define GPIO_SD_PMOS            GPIOA
#define PIN_SD_PMOS             1   // PA1  - SD power MOSFET
#endif
