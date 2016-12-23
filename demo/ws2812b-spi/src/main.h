#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "stm32f030x6.h"
#include "pinning.h"
#include "buildid.h"
#include "ws2812b_spi.h"

int main(void);

void TIM14_IRQHandler(void);

#endif
