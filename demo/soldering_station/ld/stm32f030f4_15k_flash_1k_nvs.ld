MEMORY
{
    flash (rx)                  : ORIGIN = 0x08000000, LENGTH = 15K
    flash_nvstore (rw)          : ORIGIN = 0x08003c00, LENGTH = 1K
    sram (xrw)                  : ORIGIN = 0x20000000, LENGTH = 4K
}

INCLUDE ld/common_nvs.ld
