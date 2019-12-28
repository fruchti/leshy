#pragma once

#include "stm32f030x6.h"

#define PID_PWM_FREQ            100
#define PID_PWM_INVERT
#define PID_PWM_OC

#define PID_REXT                100
#define PID_T                   40.7f
#define PID_M                   0.214755f
#define PID_ADCMAX              4095U

#define PID_VOLTAGE             24                      // V

#define PID_KP                  3.0f                    // V/K
#define PID_KI                  1.0f                    // V/Ks
#define PID_KD                  0.001f                  // Vs/K

#define PID_TS                  (1.0f / PID_PWM_FREQ)   // s
#define PID_ILIMIT              (0.5f * PID_VOLTAGE / PID_TS / PID_KI)

void PID_Init(void);

extern volatile unsigned int PID_CurrentTemperature;
extern unsigned int PID_SetTemperature;

