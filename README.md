STM32 Microcontroller Projects
==============================

STM32 devices are amazing: They are cheap and powerful.
They use state-of-the-art Cortex-M processors which makes it fun to program them.

In this repository are a collection of projects that I have done with STM32 board.
My favorite board - because it is so cheap and powerful - is the Nucleo 32 with an STM32F103C
8Tx device.


Development Approach
--------------------

I like simplistic approaches and so my goal is to keep my projects as simple as possible and understandable.
Embedded software is often a mess: Spaghetti code with a lot of global variables.
Basic and fundamental software design techniques are often ignored.
That leads to very buggy and hard to maintain software.
My goal is to do that better and my projects shall inspire you to do it better also.

STM delivers an IDE called _STM32CubeIDE_ which includes a code generator called _STM32CubeMX_.
With that IDE you can configure graphically your STM32.
It than generates code to which you must added your code called _user code_.
This is already a starting point for a big mess!
I tried it out an recognize, that a regeneration of the code overwrites my own code.
That's a no go!

Additionally STM deliveres a software library called _Hardware Abstraction Layer_ (HAL).
This Layer should ease the access to the devices.
Because it adds an additional layer between you and your hardware it increases complexitiy
and makes your projects less understandible and error prone.

I am convinced that if you want to understand what is going on in an embedded system
you have to understand the hardware you are running on and the software you are writing or using.
A said above STM32 devices are great devices.
Learning how they function is a win and gives you the power to create great applications.

This is the reason why I program my devices _bare metal_.
The only software that I use is CMSIS.
CMSIS is not much more than some header files that allow you the access to the hardware.
As toolchain I use the standard GNU toolchain for ARM processors: `arm-none-eabi-gcc`
driven by `cmake`.

I use the _Single Wire Debug_ protocol for flashing and debugging.
There are a number of very cheap programmers out there.
These devices are able to flash the software
and they start a GDB server to debug the software on the device remotely.

When it comes to debugging an IDE is of help.
Because every tools I use in the development of the STM32 software are very common
there are a bunch of tools and IDEs useable on top of the basic toolchain.

I use _Visual Studio Code_ for editing and debugging.
All the tools mentioned above are very well integrated in VSCode.
And most important: It does nothing under the hood.
Everything is in your files.
The IDE is just there for convenience.

I explain all my programs in detail to show you how clear and easy to understand they are.
But: If you know how to do it better, please tell me!
I am always searching for better solutions...


A Word About Architecture
-------------------------

Embedded system running on a micro controller are often real time systems:
The control a technical process physically in the real world.
There are sensors and acutators and moving parts as a result of combining all this.


Dynamic Architecture
--------------------

Because a mircro controller has to react timely the design approach is different to classical IT software.
In a command line tool there is a `main` function that reads some input, performs a function,
makes some output and exits.
In an application with a graphical user interface callback function are called whenever the user does something of relevance.
These callback functions react to the user input a perform some tasks.

An embedded system is designed differently:
It has a hart beat and executes cyclically at every beat the same software all the time.
This is called _Rate Monotonic Scheduling_.
Sometimes _Multi Rate Monotonic Scheduling_ is used, but that is a detail.

There is also a `main` function because the software is often written in C.
But this function is not very userful.
In my programms it has the role to trigger the application specific initialisations
and to start the heart beat.
A every beat it calls a function, that executes the application specific functionality.
I call this function `step`.

How fast the heart beat has to be depends on the application
and is often determined by the dynamics of the technical process that has to be controlled.
The `main` function can be written very generic and can then be reused in all projects.
The application specific code has to implement three functions:
1. `setup`: This is the first function that is called by `main`.
   Its purpose is to set up the board an configure als the peripherals.
   At the end the board is useable for the application specific code.
2. `init`: This function has to initialize the application.
   It is called before the first step and after the setup of the hardware.
   So it can use the hardware already when executing the initialisation code.
   `init` returns the heart beats per second.
   With this frequency the `step` function is called.
3. `step` is called cyclically when ever a heart beat occurs.
   It should finish inside the heart beat.
   Otherwise some heart beats are missed.

This is the dynamic structure of all my programms.
It scheduling is more advanced a scheduler can be triggered by `step`.
The scheduler than executes pieces of the software as required.
These pieces are often called _tasks_.


Modularization
--------------

Software can be devided in different parts called _modules_.
In C there is no programming language concept called _module_.
There are files and translation units instead.
With these elements modules can be constructed.

On the topmost level there are three modules:
1. `runtime`: The runtime module does everything that has to do with the execution of the three functions `setup`, `init` and `step` as described above.
   It contains the typical startup code necessary for the start of the C runtime
   but also the `main` function.
2. `board`: The board module is the module that implements the `setup` function
   and additional functions to access the board.
   The board module depends on the board used for the application.
   It provides a thin layer of application specific functions to acces the hardware.
   With the board module in place, the `app` module does not depend on the hardware any more.
3. `app`: The app module contains all the application specific functionality.
   It does not use the hardware directly.
   Therefore it uses the `board` module.
   The `app` module implements the `init` and `step` function of the runtime framework.

The reason for this modularization is to keep the dependencies under control
and enable reuse:

The `runtime` module must be compiled for each board but is portable.
It uses only CMSIS functions that are available on every board.
So this module can be reused for all projects.

The `board` module is the most specific module.
It is board and application specifc.
But it does not contain application logis.
It only provides access function to the hardware.
This makes the `app` module independant of the hardware.

The `app` module contains the application specific logic.
But it is independent of the hardware.
That means, it does not include any `stm32f1xx` headers.
It can be compiled, unit tested and executed on the host system.
To execute it, a stub or mock for the board module must be written.
