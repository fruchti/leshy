#pragma once

// Hardware revision.
#define LESHY_HARDWARE_MAJOR    3
#define LESHY_HARDWARE_MINOR    1

// From revision 2 onwards, leshy has a MOSFET to cut off the SD card's power
// supply. The pin its gate is connected to can be adjusted in pinning.h.
#define LESHY_HAS_SD_PMOS       (LESHY_HARDWARE_MAJOR >= 2)
// Set to 0 instead if the MOSFET isn't placed (and JP5 shorted)
// #define LESHY_HAS_SD_PMOS       0

#define _VERSION_(major, minor) #major "." #minor
#define _VERSION(major, minor)  _VERSION_(major, minor)
#define LESHY_HARDWARE_REVISION _VERSION(LESHY_HARDWARE_MAJOR,\
    LESHY_HARDWARE_MINOR)
