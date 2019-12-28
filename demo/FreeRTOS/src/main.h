#pragma once

#include <stdbool.h>
#include "stm32f030x6.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pinning.h"
#include "buildid.h"
#include "debug.h"

int main(void);

static void BlinkTask(void *param);
static void ButtonTask(void *param);
static void DebugTask(void *param);


