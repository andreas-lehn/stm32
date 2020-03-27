/*
 * system provides the file to initialize the system.
 */

#ifndef RUNTIME_SYSTEM_H
#define RUNTIME_SYSTEM_H

#include <stdint.h>

/*
 * Initialize the system.
 */
void system_init();

/*
 * Reset of the system.
 */
void system_reset();

/*
 * System core clock frequency
 */
extern uint32_t system_core_clock;

/*
 * Updates the system core clock
 */
void system_core_clock_update();

/*
 * Configuration of system ticks
 */
void system_tick_config(uint32_t ticks);

/*
 * Wait for an event/interrupt
 */
void system_wait_for_event();

#endif
