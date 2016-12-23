# Debug messages example

This code snipped shows how to print text messages to a host PC via the attached
ST-LINK debugger. Currently, `printf()` is not supported, you'll have to use
`Debug_Print()`.

Note that if the code uses the debug print functions and is build with `DEBUG =
yes` in the makefile, it has to be run with a debugger attached and OpenOCD
running. If there is no debugger attached the execution of a debug function
will be followed by a hard fault. Checking `CoreDebug->DHCSR` is not possible
on Cortex M0, so there is no easy way of determining if the MCU currently is
debugged.
