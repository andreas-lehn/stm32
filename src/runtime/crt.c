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
extern unsigned __etext;
extern unsigned __data_start__;
extern unsigned __data_end__;

/* BSS segment */
extern unsigned __bss_start__;
extern unsigned __bss_end__;

/* The start and end addresses of preinit/init/fini array */
extern void (*__preinit_array_start[])() __attribute__((weak));
extern void (*__preinit_array_end[])() __attribute__((weak));
extern void (*__init_array_start[])() __attribute__((weak));
extern void (*__init_array_end[])() __attribute__((weak));
extern void (*__fini_array_start[])() __attribute__((weak));
extern void (*__fini_array_end[])() __attribute__((weak));

/*
 * Copy data to the data section
 */
void copy_data(unsigned* from, unsigned* section_begin, unsigned* section_end) {
    for (unsigned int *p = section_begin; p < section_end; p++, from++) {
        *p = *from;
    }
}

/*
 * Clear bss section
 */
void clear_bss(unsigned int* section_begin, unsigned int* section_end) {
    for (unsigned int *p = section_begin; p < section_end; p++) {
        *p = 0;
    }
}

/*
 * Iterate over an array of void-void-functions and execute them.
 * These functions are mainly static constructors/destructors).
 */
void run_array(void (**start)(), void (**end)()) {
    for (void (**i)() = start; i < end; i++) {
        (*i)();
    }
}

/*
 * Same as run_array only in the reveser direction.
 */
void run_array_reverse(void (**start)(), void (**end)()) {
    for (void (**i)() = end - 1; i >= start; i--) {
        (*i)();
    }
}

/*
 * startup does all the work...
 */
static void __main() {
    clear_bss(&__bss_start__, &__bss_end__);
    copy_data(&__etext, &__data_start__, &__data_end__);
    run_array(__preinit_array_start, __preinit_array_end);
    run_array(__init_array_start, __init_array_end);

    main(0, NULL);

    run_array_reverse(__fini_array_start, __fini_array_end);
}

/*
 * on_reset starts the system.
 */
void on_reset() {
    system_init();
    __main();
    system_reset();
}
