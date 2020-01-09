# Demo projects

This directory contains some example code snippets for a quick start. Each
contains the necessary makefile for building and flashing, so no IDE has to be
installed in order to get started. However, the makefile is currently only
tested on Linux.

| Folder              | Example description                                                                                                                                        |
| ------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `blink`             | Classic minimal LED blinking example. Implemented with a timer update interrupt                                                                            |
| `debug-print`       | Demonstrates printing messages via semihosting while debugging                                                                                             |
| `FreeRTOS`          | Simple FreeRTOS example with tasks blinking an LED, measuring time and transmitting via semihosting                                                        |
| `hd44780`           | Basic driver for HD44780-compatible character LC displays                                                                                                  |
| `sd`                | Uses FatFs to log the internal temperature sensor's output to a CSV file on the SD card                                                                    |
| `soldering_station` | Basic controller for Hakko 907 soldering irons. Includes encoder input, charlieplexed 7-segment driver, control loop, and settings storage in flash memory |
| `ws2812b-hsv`       | Sets H, S, V via three potentiometers and light up a WS2812B strip uniformly                                                                               |
| `ws2812b-spi`       | Drives a WS2812B LED strip with the SPI peripheral and DMA                                                                                                 |

If you want to use one of the demo projects as a starting point, here is a small
table with the MCU peripherals in use and external components the project
contains driver code for:

| Folder              | Microcontroller peripherals                                  | External components                               | Third-party libraries                              |
| ------------------- | ------------------------------------------------------------ | ------------------------------------------------- | -------------------------------------------------- |
| `blink`             | `TIM`                                                        |                                                   |                                                    |
| `debug-print`       |                                                              |                                                   |                                                    |
| `FreeRTOS`          | `SysTick`                                                    |                                                   | [FreeRTOS](https://freertos.org/)                  |
| `hd44780`           |                                                              | HD44780-based display (4-bit mode)                |                                                    |
| `sd`                | `ADC` (internal temperature sensor), `SPI`, `SysTick`, `TIM` | microSD card (SPI mode)                           | [FatFs](http://elm-chan.org/fsw/ff/00index_e.html) |
| `soldering_station` | `ADC`, `FLASH` (non-volatile storage), `TIM`                 | 7-segment display (charlieplexed), rotary encoder |                                                    |
| `ws2812b-hsv`       | `ADC`, `DMA`, `TIM`                                          | WS2812B LED strip (uniform colour)                |                                                    |
| `ws2812b-spi`       | `DMA`, `SPI`, `TIM`                                          | WS2812B LED strip                                 |                                                    |

## Flashing

The makefile supports flashing the controller via
[texane's st-link](https://github.com/texane/stlink). Invoke

```
$ make program
```

to build a binary and flash it at `0x8000000`.

# Debugging

Each project directory includes a OpenOCD script, so you can start OpenOCD like:

```
$ openocd -f flash.cfg
```

As a matter of debugging, only text messages via semihosting are implemented so
far. These are displayed in OpenOCD. See the `debug-print` example for details.
Apart from that, of course `arm-none-eabi-gdb` or a complete IDE can be used.
