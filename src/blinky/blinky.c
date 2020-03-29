/*
 * blinky.c
 *
 *  Created on: 19.03.2020
 *      Author: andreas
 */

#include "board.h"
#include "framework/hooks.h"

#define STEPS_PER_SECOND 1000
#define DUTY              100

unsigned init() {
    return STEPS_PER_SECOND;
}

void step(unsigned beat) {
    if ((beat % STEPS_PER_SECOND) < DUTY) {
        led_on();
    } else {
        led_off();
    }
}
