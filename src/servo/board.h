/*
 * This is the interface to the hardware of the servo application
 */

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * The moving_led indicates that the servo is moving
 */
void moving_led_on();
void moving_led_off();

/*
 * The position LED indicate that the servo has reached one of its end positions.
 */
void position_0_led_on();
void position_0_led_off();
void position_1_led_on();
void position_1_led_off();

/*
 * The switch_position delivers the position of the switch.
 * The position is 0 or 1, if the switch is in on of its positions.
 * It is -1 if it is somewhere inbetween.
 */
int switch_position();

/*
 * Sets the position of the servo.
 * Range allowed +/-1000
 */
void servo_position(int position);

#endif /* _BOARD_H_ */
