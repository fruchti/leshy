#pragma once

#include <stdbool.h>

#include "stm32f030x6.h"

typedef struct
__attribute__((packed))
{
    unsigned int set_temperature;
} NVS_Data_t;

extern NVS_Data_t *const NVS_Data;

// Returns true if the data was successfully loaded from flash and false if the
// defaults were restored instead
bool NVS_Load(void);

// Stores the current contents of NVS_Data to flash
void NVS_Save(void);


