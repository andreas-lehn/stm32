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

    struct frq_entry {
        enum clock_frq frq;
        uint32_t clock;
        uint32_t apb1;
    } frq_table[] = {
        { CLOCK_FRQ_16_MHZ, 16000000, 16000000},
        { CLOCK_FRQ_24_MHZ, 24000000, 24000000},
        { CLOCK_FRQ_32_MHZ, 32000000, 32000000},
        { CLOCK_FRQ_40_MHZ, 40000000, 20000000},
        { CLOCK_FRQ_48_MHZ, 48000000, 24000000},
        { CLOCK_FRQ_56_MHZ, 56000000, 28000000},
        { CLOCK_FRQ_64_MHZ, 64000000, 32000000},
        { CLOCK_FRQ_72_MHZ, 72000000, 36000000}
    };

    int success = 1;
    for (int i = 0; i <= CLOCK_FRQ_72_MHZ; i++) {
        system_clock_frequency(frq_table[i].frq);
        int clock = system_core_clock;
        int apb1  = system_apb1_clock;
        success = success && (clock == frq_table[i].clock) && (apb1 == frq_table[i].apb1);
    }
    if (success) {
        GPIOC->BRR = PIN13;
    } else {
        GPIOA->BRR = 1;
    }

    while(1);
}
