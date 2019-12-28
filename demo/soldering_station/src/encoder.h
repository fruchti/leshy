#pragma once

#include "stm32f030x6.h"

#include "pinning.h"

#define ENCODER_RESOLUTION      2
#define ENCODER_ACCEL           20

void Encoder_Init(void);
void Encoder_Poll(void);
int Encoder_Read(void);


