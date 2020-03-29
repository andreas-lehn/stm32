#include "hooks.h"
#include "runtime/system.h"

static unsigned volatile system_beat = 0;

/*
 * Callback of the SysTick counter increments the system beat
 */
void on_sys_tick() {
    system_beat++;
}

/*
 * waits (if necessary) for the next beat of current beat and returns it
 */
static unsigned next_beat(unsigned current_beat)  {
    while (system_beat == current_beat)  {
        system_wait_for_event();
    }
    return system_beat;
}

/*
 * Standard main function an embedded application.
 * It sets up the board, initializes the application
 * and steps through it driven by the system beat.
 */
int main(int argc, char** argv) {
    unsigned current_beat = 0;

    system_core_clock_update();
    setup();

    unsigned beats_per_second = init();
    system_tick_config(system_core_clock / beats_per_second);

    while (1) {
    	step(current_beat);
    	current_beat = next_beat(current_beat);
    }
    return 0;
}
