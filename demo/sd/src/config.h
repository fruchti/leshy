#pragma once

// Hardware revision.
#define LESHY_HARDWARE_MAJOR    3
#define LESHY_HARDWARE_MINOR    1


#define _VERSION_(major, minor) #major "." #minor
#define _VERSION(major, minor)  _VERSION_(major, minor)
#define LESHY_HARDWARE_REVISION _VERSION(LESHY_HARDWARE_MAJOR, LESHY_HARDWARE_MINOR)
