MEMORY
{
    flash (rx)                  : ORIGIN = 0x08000000, LENGTH = 16K
    sram (xrw)                  : ORIGIN = 0x20000000, LENGTH = 4K
}

INCLUDE ld/common.ld
