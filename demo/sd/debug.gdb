target remote :3333
file build/main.elf
monitor reset halt
monitor flash write_image erase build/main.bin 0x8000000
monitor reset halt
monitor arm semihosting enable
