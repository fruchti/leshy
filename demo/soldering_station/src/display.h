#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdbool.h>

#include "stm32f030x6.h"

#include "pinning.h"
#include "buildid.h"

#define DISPLAY_DIGITS          3
// #define DISPLAY_CC   // Common cathode mode

#define DISPLAY_SEG_A           1
#define DISPLAY_SEG_B           2
#define DISPLAY_SEG_C           4
#define DISPLAY_SEG_D           8
#define DISPLAY_SEG_E           16
#define DISPLAY_SEG_F           32
#define DISPLAY_SEG_G           64
#define DISPLAY_DP              128

#define DISPLAY_SEG_ODR_MASK    ((1 << PIN_DISP_1_SEG_A) |\
                                (1 << PIN_DISP_1_SEG_B) |\
                                (1 << PIN_DISP_1_SEG_C) |\
                                (1 << PIN_DISP_1_SEG_D) |\
                                (1 << PIN_DISP_1_SEG_E) |\
                                (1 << PIN_DISP_1_SEG_F) |\
                                (1 << PIN_DISP_1_SEG_G) |\
                                (1 << PIN_DISP_1_DP) |\
                                (1 << PIN_DISP_2_SEG_A) |\
                                (1 << PIN_DISP_2_SEG_B) |\
                                (1 << PIN_DISP_2_SEG_C) |\
                                (1 << PIN_DISP_2_SEG_D) |\
                                (1 << PIN_DISP_2_SEG_E) |\
                                (1 << PIN_DISP_2_SEG_F) |\
                                (1 << PIN_DISP_2_SEG_G) |\
                                (1 << PIN_DISP_2_DP) |\
                                (1 << PIN_DISP_3_SEG_A) |\
                                (1 << PIN_DISP_3_SEG_B) |\
                                (1 << PIN_DISP_3_SEG_C) |\
                                (1 << PIN_DISP_3_SEG_D) |\
                                (1 << PIN_DISP_3_SEG_E) |\
                                (1 << PIN_DISP_3_SEG_F) |\
                                (1 << PIN_DISP_3_SEG_G) |\
                                (1 << PIN_DISP_3_DP))

#define DISPLAY_COM_ODR_MASK    ((1 << PIN_DISP_1_COMMON) |\
                                (1 << PIN_DISP_2_COMMON) |\
                                (1 << PIN_DISP_3_COMMON))

#define DISPLAY_ODR_MASK        (DISPLAY_COM_ODR_MASK | DISPLAY_SEG_ODR_MASK)

#define DISPLAY_MODER_MASK      ((3 << (PIN_DISP_1_SEG_A << 1)) |\
                                (3 << (PIN_DISP_1_SEG_B << 1)) |\
                                (3 << (PIN_DISP_1_SEG_C << 1)) |\
                                (3 << (PIN_DISP_1_SEG_D << 1)) |\
                                (3 << (PIN_DISP_1_SEG_E << 1)) |\
                                (3 << (PIN_DISP_1_SEG_F << 1)) |\
                                (3 << (PIN_DISP_1_SEG_G << 1)) |\
                                (3 << (PIN_DISP_1_DP << 1)) |\
                                (3 << (PIN_DISP_1_COMMON << 1)) |\
                                (3 << (PIN_DISP_2_SEG_A << 1)) |\
                                (3 << (PIN_DISP_2_SEG_B << 1)) |\
                                (3 << (PIN_DISP_2_SEG_C << 1)) |\
                                (3 << (PIN_DISP_2_SEG_D << 1)) |\
                                (3 << (PIN_DISP_2_SEG_E << 1)) |\
                                (3 << (PIN_DISP_2_SEG_F << 1)) |\
                                (3 << (PIN_DISP_2_SEG_G << 1)) |\
                                (3 << (PIN_DISP_2_DP << 1)) |\
                                (3 << (PIN_DISP_2_COMMON << 1)) |\
                                (3 << (PIN_DISP_3_SEG_A << 1)) |\
                                (3 << (PIN_DISP_3_SEG_B << 1)) |\
                                (3 << (PIN_DISP_3_SEG_C << 1)) |\
                                (3 << (PIN_DISP_3_SEG_D << 1)) |\
                                (3 << (PIN_DISP_3_SEG_E << 1)) |\
                                (3 << (PIN_DISP_3_SEG_F << 1)) |\
                                (3 << (PIN_DISP_3_SEG_G << 1)) |\
                                (3 << (PIN_DISP_3_DP << 1)) |\
                                (3 << (PIN_DISP_3_COMMON << 1)))

extern bool Display_Blinking;

void Display_Init(void);
void Display_Number(int value);
void Display_ShowSegments(uint8_t digit1, uint8_t digit2, uint8_t digit3);

#endif
