/*
 * clock72 switches the clock to 72 MHz.
 * It lights the green LED on success.ignal on and off
 */

#include <stm32f1xx.h>
#include <stddef.h>
#include "runtime/system.h"

#define PIN13 (1 << 13)

int main(int argc, char **argv)  {
	/* Turn on clock for required peripherals */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

    /*
     * On Port C:
     * PIN 13 = 6, Output open-drain, 2 MHz
     */
    GPIOC->BSRR = PIN13;
    GPIOC->CRH = 0x46644444;

    GPIOA->BSRR = 1;
    GPIOA->CRL = 0x44444446;

    system_clock_72();
    system_core_clock_update();
    int clock = system_core_clock;
    if (clock == 72000000) {
        GPIOC->BRR = PIN13;
    }

    GPIOA->BRR = 1;

    while(1);
}
