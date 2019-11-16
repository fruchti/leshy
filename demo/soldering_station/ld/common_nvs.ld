ENTRY(Reset_Handler)

SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata*)
        KEEP(*(.init))
        KEEP(*(.fini))
        _end_text = .;
    } >flash

    /* Non-volatile storage area in flash */
    .nvstore (NOLOAD) :
    {
        . = ALIGN(1K);
        *(.nvstore*)
        . = ALIGN(1K);
        _end_nvstore = .;
    } > flash_nvstore

    /* C++ initialiser code segments */
    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } >flash
    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } >flash

    /* C++ destructor code segments */
    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } >flash

    /* Initialised data in SRAM (copied from flash in startup.S) */
    _start_init_data = LOADADDR(.data);
    .data :
    {
        . = ALIGN(4);
        _start_data = .;
        *(.data*)
        _end_data = .;
    } >sram AT >flash

    /* Unitialised data in SRAM */
    .bss :
    {
        _start_bss = .;
        *(.bss*)
        *(COMMON)
        _end_bss = .;
    } >sram

    . = ALIGN(4);

    _start_stack = .;
    
    PROVIDE(_end_stack = ORIGIN(sram) + LENGTH(sram));
}

_end = .;
