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
extern uint32_t system_apb1_clock;

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

/*
 * Possible clock frequencies
 */
enum clock_frq {
  CLOCK_FRQ_16_MHZ,
  CLOCK_FRQ_24_MHZ,
  CLOCK_FRQ_32_MHZ,
  CLOCK_FRQ_40_MHZ,
  CLOCK_FRQ_48_MHZ,
  CLOCK_FRQ_56_MHZ,
  CLOCK_FRQ_64_MHZ,
  CLOCK_FRQ_72_MHZ
};

/*
 * Change clock frequency
 */
void system_clock_frequency(enum clock_frq frq);

#endif
