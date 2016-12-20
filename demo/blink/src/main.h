#ifndef MAIN_H_
#define MAIN_H_

#include <stdbool.h>
#include "stm32f030x6.h"
#include "pinning.h"
#include "buildid.h"

int main(void);

void TIM14_IRQHandler(void);

#endif
