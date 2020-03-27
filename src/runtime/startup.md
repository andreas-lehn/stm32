Startup of Cortex-M Processor
=============================

The start up of Cortex-M processor is the phase from power on until `main` is called.
Typically a C programer does not care about this phase because his main function is somehow called by the operation system after the application was started.
When developing bare metal application on a micro controller like the Cortex-M
there is no operating system that starts the application.

The big advantage of not having an operation system is that you can control what is happening in that phase.
The dark side is that you have to know what has to be done to get a C program started on a micro controller.
In this document I explain how my startup code works.
It is written completly in C to make it as understandable as possible.


Vector Table
------------

https://developer.arm.com/docs/dui0552/a
https://developer.arm.com/docs/dui0552/a/the-cortex-m3-processor/exception-model/exception-types
https://developer.arm.com/docs/dui0552/a/the-cortex-m3-processor/exception-model/vector-table


Linkers and Loaders
-------------------


Bringing up the C Environment
-----------------------------


Post main
---------


Resources
---------

 * https://allthingsembedded.net/2019/01/03/arm-cortex-m-startup-code-for-c-and-c/ 
 * https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2018/12/06/understand_the_gnul-PTm6
 * https://github.com/JoeMerten/Stm32/blob/master/EgaTest/Cxx-F103-TraceNone/system/src/newlib/_startup.c
 * https://arm-software.github.io/CMSIS_5/Core/html/startup_c_pg.html
 * https://developer.arm.com/docs/dui0552/a/the-cortex-m3-instruction-set/cmsis-functions