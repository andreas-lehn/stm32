/*
 * blinky.c
 *
 *  Created on: 19.03.2020
 *      Author: andreas
 */

#include "../app/app.h"
#include "../app/board.h"

#define STEPS_PER_SECOND 1024
#define DUTY              128

unsigned init(void) {
    return STEPS_PER_SECOND;
}

void step(int beat) {
    if ((beat % STEPS_PER_SECOND) < DUTY) {
        led_on();
    } else {
        led_off();
    }
}
