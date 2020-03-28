/*
 * Start a C program
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "system.h"

/*
 * main() is the entry point for applications.
 * There are no arguments because we are in an embedded environmet.
 */
extern int main(int argc, char **argv);

/*
 * The following addresses are provided by the linker.
 * Names are compatible with libc.
 */

/* Data segment */
extern char __etext;
extern char __data_start__;
extern char __data_end__;

/* BSS segment */
extern char __bss_start__;
extern char __bss_end__;

/* The start and end addresses of preinit/init/fini function array */
extern void (*__preinit_array_start)() __attribute__((weak));
extern void (*__preinit_array_end)() __attribute__((weak));
extern void (*__init_array_start)() __attribute__((weak));
extern void (*__init_array_end)() __attribute__((weak));
extern void (*__fini_array_start)() __attribute__((weak));
extern void (*__fini_array_end)() __attribute__((weak));

/*
 * Initialise the C runtime and run main.
 * 
 * This function is a replacement for the _start function of libc.
 * The libc implementation does not initialize the data segment
 * and is much to complicated when it comes to exit
 * which typically never happens in an embedded application.
 */
void _start() {
    /* clear BSS segment */
    memset(&__bss_start__, 0, &__bss_end__ - &__bss_start__);

    /* initialize data segment */
    memcpy(&__data_start__, &__etext, &__data_end__ - &__data_start__);

    /* run pre init and init functions */
    for (void (**f)() = &__preinit_array_start; f < &__init_array_start; f++) (*f)();
    for (void (**f)() = &__init_array_start; f < &__init_array_start; f++) (*f)();
    
    main(0, NULL);

    /* run fini functions in reveser order */
    for (void (**f)() = &__fini_array_end - 1; f >= &__fini_array_start; f--) (*f)();
}

/*
 * Start the system on reset event.
 */
void on_reset() {
    system_init();
    _start();
    system_reset();
}
