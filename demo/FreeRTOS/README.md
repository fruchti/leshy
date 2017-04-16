# FreeRTOS example

This example consists of three tasks and one queue. One task lets the LED on PA1
blink. The second turns the PA2-LED on and off, depending on the state of the
push button. It also measures the duration of each button press and uses the
queue to pass it to the third task, which prints the value via the semihositng
debug port.

Note that FreeRTOS is included as a submodule in the repository. In order to
build the project, you will have to run

```
$ git submodule update --init
```
