/*
 * blinky.c
 *
 *  Created on: 19.03.2020
 *      Author: andreas
 */

#include "runtime/hooks.h"
#include "board.h"

static struct servo {
    int current_position;
    int target_position;
    int end_position[2];
    int speed;
} servo;

void servo_init(struct servo * self, int pos0, int pos1) {
    self->current_position = 0;
    self->target_position = 0;
    self->end_position[0] = pos0;
    self->end_position[1] = pos1;
    self->speed = 1;
}

void servo_control(struct servo * self) {
    if (self->target_position > self->current_position) self->current_position += self->speed;
    if (self->target_position < self->current_position) self->current_position -= self->speed;
    //self->current_position = self->target_position;
}

static void update_leds(struct servo * servo) {
    if (servo->current_position == servo->end_position[0]) {
        position_0_led_on();
        moving_led_off();
        position_1_led_off();
    } else if (servo->current_position == servo->end_position[1]) {
        position_0_led_off();
        moving_led_off();
        position_1_led_on();
    } else {
        position_0_led_off();
        moving_led_on();
        position_1_led_off();
    }
}

#define BEATS_PER_SECOND 2000

#define END_POSITION_0  900
#define END_POSITION_1 -900

unsigned init() {
    servo_init(&servo, END_POSITION_0, END_POSITION_1);
    return BEATS_PER_SECOND;
}

void step(int beat) {
    int switch_pos = switch_position();
    if (switch_pos >= 0) servo.target_position = servo.end_position[switch_pos];
    servo_control(&servo);
    servo_position(servo.current_position);
    update_leds(&servo);
}
