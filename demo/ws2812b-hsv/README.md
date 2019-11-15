# WS2812B HSV controller example

This example is a simple controller for WS2812B strips, setting all LEDs'
colours uniformly based on a HSV input from three potentiometers. The
potentiometers are wired between GND and VDD, their wipers connected to PA0,
PA1, and PA2, respectively.

Since PA10, which is used for the LED data output, is 5 V tolerant, a simple
pullup to 5 V (560 Ω is a good value) is sufficient to generate the levels
necessary to drive a WS2812B.
