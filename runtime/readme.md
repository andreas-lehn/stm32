Runtime Module
==============

The runtime module provides everything that is necessary to execete an embedded application on the STM32.
It starts with the start up code provided by `startup_stm32fxxx.s`.
The startup code sets up the Cortex-M vector table,
calls `SystemInit`, initializes the C runtime environment and finally calls `main`.

[ARM Cortex-M Startup Code](https://iotality.com/arm-cortex-m4-startup-code/)
explains the startup code in more detail.
[STM32F4xx-Startup](https://github.com/G33KatWork/STM32F4xx-Startup) is a Git repository
that provides the startup code as a C file.
It is for STM32F4xx but maybe this can be taken over to STM32F1xx.

`main` is the entry point to a user defined C program.
The C program may or may not use functions of the standard library.
Typically `libc` is not linked to an embedded program because it comes with a lot of overhead.
There is a much smaller replacement of `libc` called [newlib](https://sourceware.org/newlib/).

`newlib` implements a part of the standard C library very light weight.
But the implementation is not complete.
It needs some basic function implemented by the embedded application.
These functions are called [System Calls](https://sourceware.org/newlib/libc.html#Syscalls).
An allmost empty implementation of these functions is provided by `syscalls.c` ande `sysmem.c`.

If you want to use `newlib` than you must tell the linker to link against this library.
This is done with the linker option `-specs=nano.specs`.
You do this in a `cmake` file with the command `target_link_options`:

    target_link_options(<target> PUBLIC -specs=nano.specs)

Additionally you have to implement the functions needed by `newlib`
or simply take over the files `syscalls.c` and `sysmem.c`.

If you do not use any functions of the standard C library
you can use `libnosys` instead of `newlib`.
Then you do not have to implement any functions at all.
You tell the linker to link against `libnosys` with the option `-specs=nosys.specs`.

The linker needs a linker script file to generate a program that can be flash into the device.
The linker script file tells the linker at which addresses it should link the program.
`STM32F103C8TX_FLASH.ld` is a linker script file for the STM32F103.
If you want to flash your program into this device you must tell the linker to use this script file.
You do this with the `-T` option of the linker.

In a `cmake` file you specifiy the linker script file together with the library option:

    target_link_options(<target> PUBLIC
        -T ${CMAKE_SOURCE_DIR}/runtime/STM32F103C8TX_FLASH.ld
        -spec=nosys.specs
    )

The compiler and linker need some options to compile the C sources and link the program correctly for the device.
I put these options that are necessary for the STM32 together with the compilers in a 
[cmake toolchain file](../stm32-toolchain.cmake).

[Run a C program bare metal on an ARM Cortex M3](https://jacobmossberg.se/posts/2018/08/11/run-c-program-bare-metal-on-arm-cortex-m3.html)
is a very nice article that shows the minimum necessary to run a C program on an ARM Cortex-M.
That is really minimal!


Resources
---------

* https://www.embedded-software-engineering.de/bare-bones-mit-gcc-und-c-a-674430
* https://www.mikrocontroller.net/articles/ARM_GCC#Code-Gr.C3.B6.C3.9Fe_optimieren
