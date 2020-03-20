/*
 * board.c
 *
 *  Created on: 20.03.2020
 *      Author: andreas
 */

#include "../app/board.h"

#include <stm32f1xx.h>

#include "../app/app.h"

#define PIN13 (1 << 13)

/*
 * Setup the board peripherals.
 */
void setup(void)  {

#ifndef NDEBUG
	/*
	 * IO-Port A is needed for debugging and flashing.
	 * So turn it on if debugging is wanted.
	 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
#endif

    /*
     * PIN 13 of IO-Port C is connected to the board internal LED.
     * This LED is used for the application
     */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH = 0x44144444;
}

/*
 * Switch LED on
 */
void led_on(void) {
	/*
	 * Switch is on the low side:
	 * LED is off if PIN is low.
	 */
    GPIOC->BRR = PIN13;
}

/*
 * Switch LED off
 */
void led_off(void) {
    GPIOC->BSRR = PIN13;
}