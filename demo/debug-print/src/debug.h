#pragma once

#include <stdlib.h>
#include <string.h>

#include "stm32f030x6.h"

#ifdef DEBUG
void Debug_Print(const char *message);
void Debug_PutChar(char c);
#else
#define Debug_Print(x) do{}while(0);
#define Debug_PutChar(x) do{}while(0);
#endif


