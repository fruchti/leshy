# SD card example

This example reads out the microcontroller's internal temperature sensor every
second and saves the values to `log.csv` on the SD card. Logging is stopped and
started again by clicking the switch attached to PA0.

LED 1 (PA1) indicates ongoing initialisation, while LED 2 (PA2) is lit up as
long as logging is taking place. LED 5 (PA 10) flashes for every saved data
point.

The button should be pressed before removing the SD card, because the write
cache would not be flushed otherwise.

## Some notes on the code
As you might notice, the compiled code takes up a lot of flash. That's because
it is not really optimised at all, but rather a quick port of FatFs. Depending
on the application, a number of unneeded features can be disabled in `ffconf.h`,
reducing the code size. After this first step, there probably is even more room
for flash size reduction. If this still isn't enough, a smaller FatFs variant
can be used.
