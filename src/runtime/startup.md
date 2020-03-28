Startup
=======

Startup is the phase from power on until `main` is called.
Typically a C programer does not care about this phase
because his main function is somehow called by the operation system after the application was started.
When developing bare metal application on a micro controller like the Cortex-M
there is no operating system that starts the application.

The big advantage of not having an operation system is that you can control what is happening in that phase.
The dark side is that you have to know what has to be done to get a C program started on a micro controller.
In this document I explain how my startup code works.
It is written completly in C to make it as understandable as possible.


Startup of Cortex-M Processor
-----------------------------

The Cortex-M processor expects a so called
[Vector Table](https://developer.arm.com/docs/dui0552/a/the-cortex-m3-processor/exception-model/vector-table)
at address 0.
In this vector table the address of all exception and interrupt handlers are stored.
Additionally the starting address of the stack and programm start address are stored in this table.
[`vector_table.c`](vector_table.c) provides this table.

Here are the first few entries of this table:

    const void * const _vector_table[] = {
        &__stack,
        on_reset,
        on_nmi,
        on_hard_fault,
        on_mem_mgnt_fault,

The first entry `_stack_top` is the start of the stack.
The other entries are function pointers.
These functions are declare in the following way:

    void on_reset()           __attribute__ ((weak, alias("default_handler")));
    void on_nmi()             __attribute__ ((weak, alias("default_handler")));
    void on_hard_fault()      __attribute__ ((weak, alias("default_handler")));
    void on_mem_mgnt_fault()  __attribute__ ((weak, alias("default_handler")));

All these functions are declare with the weak linker attribute
and aliased by a function called `default_handler`.
The default handler is also defined in `vector_table.c`.
It simply implements an endless loop:

    static void default_handler() {
        while(1);
    }

Whenever an event (a exception or an interrupt) occurs
the processor calls the function that is stored in the vector table at the correspond location.
E.g. if an non maskable interrupt occurs the processors jumps to the function
that is stored at entry three,
because slot three is the slot associated with the non maskable interrupt.

The exception of the Cortex-M3 processor are documented in the
[user manual](https://developer.arm.com/docs/dui0552/a/the-cortex-m3-processor/exception-model/exception-types
).
Additionally a device manufactorer defines additional interrupts
depending on the peripheral devices that are used.
For all these interrupts entries have to be added in the vector table with handler functions.

Because all these handler functions are called event based
I followed the typical naming convention for events and called them `on_<event>`.
If an event function is not defined elsewhere in the programm
the event end in the endless loop of the default handler

So at least one function must be defined
if the processor should do something meaningful.
This is `on_reset` which is called by the processor immediately after a reset.
There is only one thing it does before it calls this function:
It stores the address of `_top_stack` in the stack pointer.


Starting a C program
--------------------

A C program starts with its `main` function.
But before the main function is startet
the C runtime environment has to be set up.
To see what that means we take a look at a very simple C program:

    int a;
    int b = 7;
    const int c = 5;

    int main(int argc, char **argv) {
        printf("a = %d, b = %d, c = %d\n", a, b, c);
        return 0;
    }

The output of this simple program is clear:

    a = 0, b = 7, c = 5

It is guarenteed by the C standard that `a` is zero altough it is not initialized.
`b` is writeable so it has to be located in RAM.
`c` is constant and my stay in ROM.
Because `b` is located in RAM it has to be initialized with 7 before `main` is called.
In summeray the C runtine has to set the variables `a` and `b` which are located somewhere in RAM
to there correct values of 0 or 7 respectively.

Hint: In C++ constructors must be called for global objects before `main`
and the corresponting destructors after `main`.

In summeray the following steps have to be performed by the C runtime:
 1. All uninitialized global variables are set to zero.
 2. To all initialized global variables their value is assigned.
 3. C++ constructors are called for global objects.
 4. `main` is called with `argc`/`argv` parameters.
 5. C++ destructors are called after `main` has returned.the 

All these initializations and the call of `main` are done by `libc`
when a program is run under an operating system.
But this is not the case on a micro controller.
Building an executable program with:

    arm-none-eabi-gcc -o example-main.elf example-main.c

leads to error messages of the linker (`ld`):

    ld: libc.a(lib_a-exit.o): in function `exit': undefined reference to `_exit'
    ld: libc.a(lib_a-sbrkr.o): in function `_sbrk_r': undefined reference to `_sbrk'
    ld: libc.a(lib_a-writer.o): in function `_write_r': undefined reference to `_write'
    ld: libc.a(lib_a-closer.o): in function `_close_r': undefined reference to `_close'
    ld: libc.a(lib_a-fstatr.o): in function `_fstat_r': undefined reference to `_fstat'
    ld: libc.a(lib_a-isattyr.o): in function `_isatty_r': undefined reference to `_isatty'
    ld: libc.a(lib_a-lseekr.o): in function `_lseek_r': undefined reference to `_lseek'
    ld: libc.a(lib_a-readr.o): in function `_read_r': undefined reference to `_read'
    collect2: error: ld returned 1 exit status

`libc` expects these functions to be implemented by the underlying operation system.
The ARM compiler version of the GNU compiler comes with [Newlib](https://sourceware.org/newlib).
_Newlib_ is an resource optimized implementation of of the C standard library for embedded environment like Cortex-M and Cortex-R.
But it is still an implementation of the C standard library.
Linking against Newlib can be enforced done with the option `--specs=nano.specs`.
The results are the same may be because Newlib is the default C library the ARM compiler uses.

But things change if we use the option `--specs=nosys.specs`.
With this option the compiler links against `libnosys`
which is intended - as the name says - for system without an operationg system.
If fills in missing system calls with stubs.
With this option our example C program can be linked sucessfully.

With `nm` we can have a look into the generated ELF file to see what is in there.
This is an extract:

    00008000 T _init
    00008018 T exit
    000080e4 T _mainCRTStartup
    000080e4 T _start
    000081f8 T main
    00008248 T __libc_init_array
    0000841c T printf
    00013380 R c
    0002367c d __preinit_array_end
    0002367c d __preinit_array_start
    0002367c d __init_array_start
    00023684 d __init_array_end
    00023684 d __fini_array_start
    00023688 d __fini_array_end
    00023688 D __data_start
    0002368c D b
    00024040 D _edata
    00024040 B __bss_start
    00024098 B a
    000240c0 B errno
    000240c4 B __bss_end__
    00080000 N _stack

We can see that there is our `main` function and also `printf`.
But there are much more functions.
For example `_start` and `_mainCRTStartup` which have the same starting address.
`objdump` tells us more about the file:

    example-main.elf:     file format elf32-littlearm
    architecture: armv4t, flags 0x00000112:
    EXEC_P, HAS_SYMS, D_PAGED
    start address 0x000080e4

Here we can see that the start address of the program is `0x000080e4`
which is exaclty the address of `_start`.
That indicates that the entry point of the program is `_start`.

`_start` is implemented by `libc`.
But the standard function is not useable in an embedded environment:
 1. It does not copy the data segment form ROM to RAM.
    That leads to uninitialized variable.
    In our example the value of variable `b` will be undefined but not 7.
    At least in the most cases...
 2. `_start` take a lot of care about the exit of the program.
    But in an embedded environment `main` typically does not end
    and the program does not exit.
    Because of this `_start` introduces a lot of unnecessary code.

These are the reason why we have to do the initialisations on our own.
This is done [crt.c](crt.c).
First we have to implement the function `on_reset`:

    void on_reset() {
        system_init();
        _start();
        system_reset();
    }

It calls `system_init` to initialise the hardware as proposed by CMSIS.
Then it calls our own `_start` function which starts up the C program.
Whenever `main` and then `_start` ends it does a system reset with the call of `system_reset`.
The functions `system_init()` and `system_reset` are implemented in [system.c](system.c).
With that design we can keep [crt.c](crt.c) independant from the hardware.

The most simple version of `_start` is this:

    void _start() {
        main(0, NULL);
    }

But this is not sufficient because the global variables are not initialized correctly.
To understand how we can initialise the global variables correctly
and call constructors and destructors
we have to take a look at what a compiler and linker does.


C Compiler, Linker and Loader
-----------------------------

When a C file is compiled the compiler transforms it to an object file.
The GNU compiler that we use generates files in the so called _Executable and Linkable Format_
[ELF](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format).

This object file contains the maschine code of the functions
and all the data that is assigned to variables in that source file.
Additionally it contains all symbol names defined in or referenced from that source file.
Symbol names are the names of variables and functions.

The symbol names are needed by the linker to bring all the object file together
and transform them to an executable program.
This step is called linking.
The linker ensures that when a function is called for one object file
that the call finds the right function in another object file.
Same with variable used in different object files.

You can see what is included in an object or executable file with the `nm` command.
Lets have a look what the object file of `vector_table.c` contains.
This is the output on `nm`.

             U _stack
    00000000 R vector_table
    00000000 t default_handler
    00000000 W on_adc1_2
    00000000 W on_bus_fault
    00000000 W on_can1_rx1
    00000000 W on_can1_sce
    00000000 W on_debug_mon

The first column is the address of the symbol.
The address are all zero because the symbols are not yet located to a specific address.
This is done in the last step by the linker.

The only exeption is `_stack` which does not even have an address.
The reason for this is that this symbol is not defined in this object file.
It is undefined and mark with `U`.
This symbol must be defined in exaclty on other object file.
Otherwise the linker will complain that there is an undefined symbol
or a symbol is defined twice.

`vector_table` is defined in this object file.
It has a address (of zero) and is mark with a `R` for `rodata`.
`default_handler` is also defined in that object file but with a `t` for text.
It has a small `t` because it is `static` and not visible outside of the file.
The functions like `on_adc1_2` are the weak definition.
This definitions can be overriden by a definition in another file.

So it is done with `on_reset`.
We have defined `on_reset` in [crt.c](crt.c).
Here is what `nm` says to this file/definition:

             U main
    00000000 T on_reset
    00000000 T _start
             U system_init
             U system_reset

In the object file of [crt.c](crt.c) there are two functions defined: `on_reset` and `_start`.
The function `main`, `system_init` and `system_reset` are undefined.
`system_init` and `system_reset` are defined in [system.c](system.c).
`main` must be defined by the application programmer in his application program.

So the name of all C variables and functions are stored in the object files.
The linker brings it all together and assigns addresses to these objects.
But the linker can do even more:
The linker can define symbols by itself.
This symbols can be accessed from C functions as external symbols.
There is not destinction between symbols from the linker and symbols in other object file.

When we now build our program with our own `_start` function
we get an error from the linker:

    ld: in function `_start':
        crt.c:(.text+0x0): multiple definition of `_start'; crt0.o:(.text+0x0): first defined here
    collect2: error: ld returned 1 exit status

Now the sysbol `_start` are defined twice: Once in our file [crt.c]
and once again in the C library file `crt0.o`.
To get rid of this error we have to tell the linker
that it should not use the standard start files.
This can be done with the option `-nostartfiles`.
With this option in place our program links to an executable file.

We can now have a look in this executable file.
Here is an extract:

    00008000 T main
    00008050 t default_handler
    0000805c T _start
    00008164 T on_reset
    000082cc T printf
    00012fb8 R c
    00012fd4 R vector_table
    00023398 D __data_start
    00023398 D b
    00023d44 D _edata
    00023d44 B __bss_start__
    00023d80 B a
    00023da8 B errno
    00023dac B __bss_end__
    00080000 N _stack

First, we see that the symbols now have addresses.
And we see that all objects and functions we defined are in the executable file.
But there are additional symbols that we did not define.
`__data_start`, `__bss_end__` or `_stack` for example.
Where do these symbols come from?

And we have another problem:
The addresses of the symbols do not match to the memory layout of our controller.
Our RAM start at address 0x20000000 and out flash memory is locatet at 0x08000000.
How can we fix this?


Linker Script File
------------------



Lets have a look at the `_start` function:

    void _start() {
        for (int *p = &__bss_start__; p < &__bss_end__; p++) *p = 0;
        for (int *p = &__data_start__, *from = &__etext; p < &__data_end__; p++, from++) *p = *from;

        for (void (**f)() = &__preinit_array_start; f < &__preinit_array_end; f++) (*f)();
        for (void (**f)() = &__init_array_start; f < &__init_array_end; f++) (*f)();

        main(0, NULL);

        for (void (**f)() = &__fini_array_start - 1; f >= &__fini_array_end; f--) (*f)();
    }

There are some `for` loops bevor `main` is called and another one thereafter.
The first one initializes the global variables with 0.
The second one assigns value to the global variables.
The third and forth call some functions like constructors to initialise even more
And the last one call the destructors of the global objects.

But how does this work and where do these symbols like `__bss_start__` or `__init_array_start` come from?
To understand this we have to understand how linking of a C program works.



Resources
---------

 * https://allthingsembedded.net/2019/01/03/arm-cortex-m-startup-code-for-c-and-c/ 
 * https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2018/12/06/understand_the_gnul-PTm6
 * https://github.com/JoeMerten/Stm32/blob/master/EgaTest/Cxx-F103-TraceNone/system/src/newlib/_startup.c
 * https://arm-software.github.io/CMSIS_5/Core/html/startup_c_pg.html
 * https://developer.arm.com/docs/dui0552/a/the-cortex-m3-instruction-set/cmsis-functions
 * https://blog.uvokchee.de/2019/07/arm-bare-metal-flags.html
 * https://interrupt.memfault.com/blog/zero-to-main-1
 * https://github.com/umanovskis/baremetal-arm
 * https://interrupt.memfault.com/how-to-write-linker-scripts-for-firmware
 * https://interrupt.memfault.com/boostrapping-libc-with-newlib
 * https://embeddedartistry.com/blog/2019/04/17/exploring-startup-implementations-newlib-arm/
