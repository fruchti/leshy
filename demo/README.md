# Demo projects

This directory contains some example code snippets for a quick start. Each
contains the necessary makefile for building and flashing, so no IDE has to be
installed in order to get started. However, the makefile is currently only
tested on linux.

| Folder              | Example                                                                                                                                                     |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `blink`             | Classic minimal LED blinking example. Implemented with a timer update interrupt                                                                             |
| `debug-print`       | Demonstrates printing messages via semihosting while debugging                                                                                              |
| `FreeRTOS`          | Simple FreeRTOS example with tasks blinking an LED, measuring time and transmitting via semihosting.                                                        |
| `sd`                | Uses FatFs to log the internal temperature sensor's output to a CSV file on the SD card                                                                     |
| `soldering_station` | Basic controller for Hakko 907 soldering irons. Includes encoder input, charlieplexed 7-segment driver, control loop, and settings storage in flash memory. |
| `ws2812b-hsv`       | Set H, S, V via three potentiometers and light up a WS2812B strip uniformly.                                                                                |
| `ws2812b-spi`       | Drives a WS2812B LED strip with the SPI peripheral and DMA                                                                                                  |

The folder `common` contains libraries such as FreeRTOS, linker scripts and
other supplementary files.


## Flashing and debugging

The makefile currently supports two methods of flashing the controller: Either
[texane's st-link](https://github.com/texane/stlink) or OpenOCD can be used.
OpenOCD also allows debugging, so if the makefile detects a running OpenOCD
instance, it will automatically upload the firmware via OpenOCD. If there's
none running, it will fall back to `st-flash`.

Each project directory includes a OpenOCD script, so you just have to start
OpenOCD like:

```
$ openocd -f flash.cfg
```

As a matter of debugging, only text messages via semihosting are implemented so
far. These are displayed in OpenOCD. See the `debug-print` example for details.
Apart from that, of course `arm-none-eabi-gdb` or a complete IDE can be used.


## Using the projects as templates

If you want to use the demo projects as a starting point for your own
applications, you will have to adapt the makefile slightly. The projects
reference `config.mk` and some files in the folder `common`. Move the necessary
files to the project directory and adjust the paths and you're good to go!
