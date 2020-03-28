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
extern char __data_load;
extern char __data_start;
extern char __data_end;

/* BSS segment */
extern char __bss_start__;
extern char __bss_end__;

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
    memcpy(&__data_start, &__data_load, &__data_end - &__data_start);
    
    main(0, NULL);
}

/*
 * Start the system on reset event.
 */
void on_reset() {
    system_init();
    _start();
    system_reset();
}
