/*
 * app.h
 *
 * Defines the prototypes for a standard embedded application.
 *
 *  Created on: 19.03.2020
 *      Author: andreas
 */

#ifndef _RUNTIME_CALLBACK_H_
#define _RUNTIME_CALLBACK_H_

/*
 * Setup has to set up the board and configure all peripherals so that is is useable for the application.
 */
void setup();

/*
 * Init initializes the application.
 * It must be independent from the hardware so that it can be executed in a simulation.
 * Everything that is hardware dependant has to got into `setup`.
 *
 * Init returns the amount of beats per second that it wants to have.
 */
unsigned init();

/*
 * Step executes one step of the application triggered by the system beat.
 */
void step(unsigned beat);

#endif /* _RUNTIME_CALLBACK_H_ */
