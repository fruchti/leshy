#ifndef SD_H_
#define SD_H_

#include "stm32f030x6.h"
#include "ff.h"

#define SD_PIN_SWITCH       1   // PB1 - SD present switch
#define SD_PIN_CS           4   // PA4 - SD chip select
#define SD_PIN_SCK          5   // PA5 - SD SCK
#define SD_PIN_MISO         6   // PA6 - SD MISO
#define SD_PIN_MOSI         7   // PA7 - SD MOSI

void SD_Init(void);
int SD_DetectMedia(void);

void disk_timerproc(void);

#endif
