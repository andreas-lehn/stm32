/*
 * Startup the hardware and C runtime environment.
 * 
 * The startup function provided by this file can be called directly after a reset.
 * The steps performed are:
 * 
 *  1. Clear the BBS area
 *  2. Copy the data section
 *  3. Initialize the system (with a call of system_init)
 *  4. Run the preinit/init array (for C++ constructors)
 *  5. Call main
 *  6. Run the fini array (for C++ destructors)
 *  7. Reset the system (with a call of system_reset)
 */

#include <stdint.h>
#include <stddef.h>
#include "system.h"

/*
 * main() is the entry point for applications.
 * There are no arguments because we are in an embedded environmet.
 */
extern int main(int argc, char **argv);

/*
 * The followin addresses have to be provided by the linker and must be defined in linker script.
 */

/* Data segment */
extern int __etext;
extern int __data_start__;
extern int __data_end__;

/* BSS segment */
extern int __bss_start__;
extern int __bss_end__;

/* The start and end addresses of preinit/init/fini function array */
extern void (*__preinit_array_start)() __attribute__((weak));
extern void (*__preinit_array_end)() __attribute__((weak));
extern void (*__init_array_start)() __attribute__((weak));
extern void (*__init_array_end)() __attribute__((weak));
extern void (*__fini_array_start)() __attribute__((weak));
extern void (*__fini_array_end)() __attribute__((weak));

/*
 * Start initialise the C runtime, calls main and does the clean-up.
 * This function is sometime also called __main oder _start
 * when it is implemented by libc or newlib.
 */
static void start() {
    /* clear BSS segment */
    for (int *p = &__bss_start__; p < &__bss_end__; p++) *p = 0;

    /* initialize data segment */
    for (int *p = &__data_start__, *from = &__etext; p < &__data_end__; p++, from++) *p = *from;

    /* run pre init functions */
    for (void (**f)() = &__preinit_array_start; f < &__init_array_start; f++) (*f)();

    /* run init functions */
    for (void (**f)() = &__init_array_start; f < &__init_array_start; f++) (*f)();

    main(0, NULL);

    /* run fini functions in reveser order */
    for (void (**f)() = &__fini_array_start - 1; f >= &__fini_array_end; f--) (*f)();
}

/*
 * Start the system on reset event.
 */
void on_reset() {
    system_init();
    start();
    system_reset();
}
