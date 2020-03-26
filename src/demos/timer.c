/*
 * blinky-timer demonstrates three timer based approaches to blink an LED:
 * 
 *  1. Wait until a timer expires and switch the LED (PC13) on and off
 * 
 *  2. Switch a LED (PB8) on and off in an interupt service routine
 * 
 *  3. Switch a LED (PB9) with a PWM signal on and off
 */

#include <stm32f1xx.h>
#include <stddef.h>

#define PIN8  (1 << 8)
#define PIN9  (1 << 9)
#define PIN13 (1 << 13)

int main()  {
	/* Turn on clock for required peripherals */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN
                  | RCC_APB2ENR_IOPCEN;

    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    /* 
     * Each Pin of each GPIO Port is configured by by 4 bits.
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
     */

    /*
     * On Port B
     * 
     * PIN 8: LED IRQ switched => 6 = Output 2 MHz, open-drain
     * PIN 9: LED PWM switched => E = Output 2 MHz alternate function (timer 4, channel 4) open-drain
     */
    GPIOB->CRH = 0x444444E6;

    /*
     * On Port C
     * 
     * PIN 13: LED switched manually => 6 = Output open-drain, 2 MHz
     */
    GPIOC->CRH = 0x44644444;

    /*
     * The timer is running with a clock speed of 8 MHz.
     * With a prescale of 8000 our timer runs in a frequency of 1 kHz.
     * That mean a tick has a duration of 1 ms.
     */
    TIM4->PSC = 8000 - 1;

    /*
     * We want a timer udate every 1 s or 1000 ms. 
     * So 1000 is the value for the auto reload register.
     */
    TIM4->ARR = 1000 - 1;

    /*
     * We use channel 1 of timer 4 to switch the LED manually on
     * and channel 2 to switch it off.
     */
    TIM4->CCR1 = 100;
    TIM4->CCR2 = 600;

    /*
     * The interupt based LED is switch on with a timer update
     * and switched of by channel 3.
     */
    TIM4->CCR3 = 400;
    TIM4->DIER = TIM_DIER_UIE | TIM_DIER_CC3IE;
    NVIC_EnableIRQ(TIM4_IRQn);

    /* 
     * Channel 4 is used as channel for the PWM signal.
     */
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1;
    TIM4->CCER  |= TIM_CCER_CC4E;
    TIM4->CCR4  = 800;

    /* 
     * Enable the counter
     */
    TIM4->CR1 |= TIM_CR1_CEN;

    /*
     * main loop to switch on an off LED manually
     */
    while (1) {
        /* wait for timer4 channel 1 by pulling the status register */
        while (!(TIM4->SR & TIM_SR_CC1IF));

        /* reset interrupt flag and switch LED on */
        TIM4->SR &= ~TIM_SR_CC1IF;
        GPIOC->BRR = PIN13;
        
        /* wait for timer channel 2 */
        while (!(TIM4->SR & TIM_SR_CC2IF));

        /* reset interrupt flag and switch LED on */
        TIM4->SR &= ~TIM_SR_CC2IF;
        GPIOC->BSRR = PIN13;
    }
}

/*
 * Interrupt service routine for update event of timer 4:
 * switch LED on PB8 on.
 */
void on_timer4_update() {
    GPIOB->BRR = PIN8;
}

/*
 * Interrupt service routine for channel 3 compare event of timer 4:
 * switch LED on PB8 off.
 */
void on_timer4_channel3() {
    GPIOB->BSRR = PIN8;
}

/*
 * Entry structure for dispatch table associates an ISR with an event.
 */
struct dispatch_entry {
    int event;
    void (*isr)();
};

/*
 * Dispatch table for the event of timer 4: Update event and channel 3 event.
 */
const struct dispatch_entry timer4_dispatch_table[] = {
    { TIM_SR_UIF, on_timer4_update },
    { TIM_SR_CC3IF, on_timer4_channel3},
    { 0, NULL }
};

/*
 * ISR of timer 4 dispatches the event according to the dispatch table.
 */
void TIM4_IRQHandler() {
    for (int i = 0;  timer4_dispatch_table[i].event != 0; i++) {
        int evt = timer4_dispatch_table[i].event;
        if (TIM4->SR & evt) {
            TIM4->SR &= ~evt;
            timer4_dispatch_table[i].isr();
        }
    }
}
