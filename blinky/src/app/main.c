#include <stm32f1xx.h>

#include "../app/app.h"

static unsigned volatile system_beat = 0;

/*
 * Callback of the SysTick counter increments the system beat
 */
void SysTick_Handler(void) {
    system_beat++;
}

/*
 * waits (if necessary) for the next beat of current beat and returns it
 */
static unsigned next_beat(unsigned current_beat)  {
    while (system_beat == current_beat)  {
        __WFE(); // Power-Down until next Event/Interrupt
    }
    return system_beat;
}

/*
 * Standard main function an embedded application.
 * It sets up the board, initializes the application and steps throut it driven by the system beat.
 */
int main(void) {
    unsigned current_beat = 0;

    setup();

    unsigned beats_per_second = init();
    SysTick_Config(SystemCoreClock / beats_per_second);

    while (1) {
    	step(current_beat);
    	current_beat = next_beat(current_beat);
    }
    return 0;
}
