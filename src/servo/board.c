/*
 * Implementation of the hardware access functions of the servo application.
 */

#include "board.h"
#include "framework/hooks.h"
#include <stm32f1xx.h>

#define PIN0  (1 << 0)
#define PIN3  (1 << 3)
#define PIN5  (1 << 5)
#define PIN7  (1 << 7)
#define PIN8  (1 << 8)
#define PIN9  (1 << 9)
#define PIN13 (1 << 13)

#define SERVO_MID_DUTY 1500

/*
 * Setup the board peripherals.
 */
void setup()  {

	/* Turn on clock of rquired peripherals */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN
                  | RCC_APB2ENR_IOPBEN
                  | RCC_APB2ENR_IOPCEN;

    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    /* 
     * Each Pin of each Port is configured by by 4 bits.
     * So for each pin there is a configuration value range of 0 .. F
     * The bit coding is a little bit confusing.
     * The following list explains the meanung of each number:
     * 
     *  0   Input           analog
     *  1   Output 10 MZz   push-pull
     *  2   Output  2 MHz   push-pull
     *  3   Output 50 MHz   push-pull
     *  4   Input           floating (reset value)
     *  5   Output 10 MZz   open-drain
     *  6   Output  2 MHz   open-drain
     *  7   Output 50 MHz   open-drain
     *  8   Input           pull-up/pull-down
     *  9   Output 10 MZz   alternate push-pull
     *  A   Output  2 MHz   alternate push-pull
     *  B   Output 50 MHz   alternate push-pull
     *  C   Reserved
     *  D   Output 10 MZz   alternate open-drain
     *  E   Output  2 MHz   alternate open-drain
     *  F   Output 50 MHz   alternate open-drain
     * 
     * So just have a look in this table to know what is going on...
     */

    /*
     * On Port A
     * 
     * PIN 0: LED Position 0 => 6 = Output 2 MHz open-drain
     * PIN 3: Switch Pin 0   => 8 = Input pull-up/pull-down
     * PIN 5: Switch Pin 2   => 8 = Input pull-up/pull-down
     * PIN 7: LED Position 1 => 6 = Output 2 MHz open-drain
     */
    GPIOA->CRL = 0x64848446;

    /* Configure input pins as pull-up */
    GPIOA->ODR = PIN3 | PIN5;

    /*
     * On Port B
     * 
     * PIN 9: Servo Signal => A = Output 2 MHz alternate function (timer 4, channel 4) push-pull
     */
    GPIOB->CRH = 0x444444A4;

    /*
     * On Port C
     * 
     * PIN 13: LED Moving Servo => 6 = Output open-drain, 2 MHz
     */
    GPIOC->CRH = 0x44644444;

    /*
     * We the timer running with a clock speed of 8 MHz.
     * With a prescale of 8 our timer runs in a frequency of 1 MHz.
     * That mean a tick has a duration of 1 µs.
     */
    TIM4->PSC = 8 - 1;

    /*
     * Our PWM Signal has a frequency of 20 ms or 20000 µs. 
     * So we have to start every 20000 tick with the PWM signal.
     * This is the value for our auto reload register.
     */
    TIM4->ARR = 20000 - 1;

    /*
     * Timer 4 Channel 4 on PB9 configuration:
     * Initialize PWM mode and enable the channel
     */
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
    TIM4->CCER  |= TIM_CCER_CC4E;

    /*
     * The bring a servo to is middle position
     * it needs a signal that is 1.5 ms or 1500 µs long.
     * That is the inital value for our count and compare register.
     */
    TIM4->CCR4 = SERVO_MID_DUTY;

    /* Enable the counter */
    TIM4->CR1 |= TIM_CR1_CEN;
}

void moving_led_on() {
    GPIOC->BRR = PIN13;
}

void moving_led_off() {
    GPIOC->BSRR = PIN13;
}

void position_0_led_on() {
    GPIOA->BRR = PIN0;
}

void position_0_led_off() {
    GPIOA->BSRR = PIN0;
}

void position_1_led_on() {
    GPIOA->BRR = PIN7;
}

void position_1_led_off() {
    GPIOA->BSRR = PIN7;
}

int switch_position() {
    uint16_t input = GPIOA->IDR;
    uint16_t pin3 = input & PIN3;
    uint16_t pin5 = input & PIN5;
    if ( pin3 && !pin5) return 1;
    if (!pin3 &&  pin5) return 0;
    return -1;
}

void servo_position(int position) {
    TIM4->CCR4 = SERVO_MID_DUTY + position;
}
