/*
 * app.h
 *
 * Defines the prototypes for a standard embedded application.
 *
 *  Created on: 19.03.2020
 *      Author: andreas
 */

#ifndef APP_APP_H_
#define APP_APP_H_

/*
 * Setup has to set up the board and configure all peripherals so that is is useable for the application.
 */
void setup(void);

/*
 * Init initializes the application.
 * It must be independent from the hardware so that it can be executed in a simulation.
 * Everything that is hardware dependant has to got into `setup`.
 *
 * Init returns the amount of beats per second that it wants to have.
 */
unsigned init(void);

/*
 * Step executes one step of the application triggered by the system beat.
 */
void step(int beat);

#endif /* APP_APP_H_ */
