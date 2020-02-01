#pragma once

#include "ff.h"
#include "config.h"
#include "pinning.h"

#if LESHY_HAS_SD_PMOS
// Set gate low
#define SD_PowerOn()            do { \
                                    GPIO_SD_PMOS->BRR = (1 << PIN_SD_PMOS); \
                                    GPIO_SD_PMOS->MODER = (GPIO_SD_PMOS->MODER \
                                        & ~(0x3 << PIN_SD_PMOS * 2)) \
                                        | (0x1 << PIN_SD_PMOS * 2); \
                                } while(0)
// Set gate high
#define SD_PowerOff()           do { \
                                    GPIO_SD_PMOS->BSRR = (1 << PIN_SD_PMOS); \
                                    GPIO_SD_PMOS->MODER = (GPIO_SD_PMOS->MODER \
                                        & ~(0x3 << PIN_SD_PMOS * 2)) \
                                        | (0x1 << PIN_SD_PMOS * 2); \
                                } while(0)
#else
#define SD_PowerOn()            do {} while(0)
#define SD_PowerOff()           do {} while(0)
#endif

