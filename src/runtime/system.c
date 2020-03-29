#include "system.h"
#include "stm32f1xx.h"
#include <stdint.h>

#define HSE_VALUE 8000000U /* Default value of the External oscillator in Hz. */
#define HSI_VALUE 8000000U /* Default value of the Internal oscillator in Hz. */

#define VECT_TAB_OFFSET 0 /* Vector Table base offset field. This value must be a multiple of 0x200. */

uint32_t system_core_clock = 8000000;
uint32_t system_apb1_clock = 8000000;

static const int ahb_prescale_divisor[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 6, 8, 12, 14, 16, 18};
static const int apb_prescale_divisor[] =  {1, 1, 1, 1, 2, 4, 6, 8};

/*
 * Setup the microcontroller system
 * 
 * There are three clock sources that can be used to drive the system clock (SYSCLK):
 *  1. HSI oscilator clock (high speed internal clock signal)
 *  2. HSE oscilator clock (high speed external clock signal)
 *  3. PLL clock
 * 
 * The system clock is used to drive the AHB frequency
 * which can be adjusted by prescalers.
 * The maximum frequency of AHB is 72 MHz.
 * 
 * The AHB frequency is used to drive the APB1 and APB2 frequency.
 * Again these frequencies can be configured with prescalers.
 * The maximum frequency of APB1 is 36 MHz and of APB2 it is 72 MHz.
 * 
 * This setup routine sets up the mirco controller to use external 8 MHz oscillator clock (HSE) 
 * as source for the system clock.
 * It scales this frequency up to 16 MHz.
 * 16 MHz is also used for APB2.
 * APB1 uses half the frequency which results in 8 MHz.
 * 
 * The timer frequencies are set to the hardware frequency.
 * They are not doubled by their prescaler.
 */
void system_init() {
    /* The reset value of control register RCC-CR is 0x000 XX83. That means:
     *
     *  PLL ready        PLLRDY = No
     *  PLL on           PLLON  = No
     *  CSS on           CSSON  = No
     *  HSE bypass       HSEBY  = No
     *  HSE on           HSEON  = No
     *  HSI calibration  HSICAL = undefined
     *  HSI trimming    HSITRIM = 16
     *  HSI ready       HSIRDY  = Yes
     *  HSI on          HSION   = Yes
     */

    /* Switch on HSE and Clock Security*/
    RCC->CR |= RCC_CR_HSEON | RCC_CR_CSSON;

    /*
     * The reset value of the clock configuration register is 0x0000 0000.
     * That means:
     * 
     *  Micro controller clock output:    MCO = No clock
     *  USB prescaler:                 USBPRE = Divided by 1.5
     *  PLL multiplication factor      PLLMUL = input clock x 2
     *  HSE divider for PLL          PLLXTPRE = not divided
     *  PLL entry clock source         PLLSRC = HSI / 2
     *  ADC prescaler                  ADCPRE = PCLK / 2
     *  APB high speed prescaler        PPRE2 = HCLK not divided
     *  APB low speed prescaler         PPRE2 = HCLK not divided
     *  AHB prescaler                    HPRE = SYSCLK not divided
     *  System clock switch status        SWS = HSI oscillator used
     *  System clock switch                SW = HSI selected
     */

    /* Set HSE as source for system clock => 8 MHz */
    RCC->CFGR |= RCC_CFGR_SW_HSE;

    /* Vector Table Relocation in Internal FLASH. */
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; 
}

/*
 * Calculate system PLL clock
 */
static unsigned system_pll_clock() {
    unsigned pll_base = 0;

    uint32_t pll_source = RCC->CFGR & RCC_CFGR_PLLSRC;
    if (pll_source == 0) {
        pll_base = (HSI_VALUE / 2);
    } else {
        pll_base = HSE_VALUE;
        /* HSE selected as PLL clock entry */
        if ((RCC->CFGR & RCC_CFGR_PLLXTPRE) != (uint32_t) RESET) {
            pll_base = (HSE_VALUE / 2);
        }
    }

    uint32_t pll_multiplier = (RCC->CFGR & RCC_CFGR_PLLMULL) >> RCC_CFGR_PLLMULL_Pos;
    return pll_base * (pll_multiplier + 2);
}

/*
 * Update system core clock variable according to Clock Register Values.
 * The core clock is HCLK.
 */
void system_core_clock_update()
{
    /* Get SYSCLK source */
    switch (RCC->CFGR & RCC_CFGR_SWS)
    {
        case 0x00U:  /* HSI used as system clock */
            system_core_clock = HSI_VALUE;
            break;
        case 0x04U:  /* HSE used as system clock */
            system_core_clock = HSE_VALUE;
            break;
        case 0x08U:  /* PLL used as system clock */
            system_core_clock = system_pll_clock();
            break;
        default:
            system_core_clock = HSI_VALUE;
            break;
    }

    /* Compute HCLK clock frequency */
    int prescaler = (RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;
    system_core_clock /= ahb_prescale_divisor[prescaler];
    prescaler = (RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;
    system_apb1_clock = system_core_clock / apb_prescale_divisor[prescaler];
}

void system_tick_config(uint32_t ticks) {
    SysTick_Config(ticks);
}

/*
 * Wait for an event/interrupt
 */
void system_wait_for_event() {
    __WFE();
}

/*
 * Resets the system
 */
void system_reset() {
    __NVIC_SystemReset();
}

/*
 * table of clock and flash latency parameters.
 */
const static struct clock_param {
    uint32_t pllmul;
    uint32_t ppre1;
    uint32_t flash_latency;
} clock_param_table[] = {
    { RCC_CFGR_PLLMULL2, RCC_CFGR_PPRE1_DIV1, 0},
    { RCC_CFGR_PLLMULL3, RCC_CFGR_PPRE1_DIV1, 0},
    { RCC_CFGR_PLLMULL4, RCC_CFGR_PPRE1_DIV1, FLASH_ACR_LATENCY_0},
    { RCC_CFGR_PLLMULL5, RCC_CFGR_PPRE1_DIV2, FLASH_ACR_LATENCY_0},
    { RCC_CFGR_PLLMULL6, RCC_CFGR_PPRE1_DIV2, FLASH_ACR_LATENCY_0},
    { RCC_CFGR_PLLMULL7, RCC_CFGR_PPRE1_DIV2, FLASH_ACR_LATENCY_1},
    { RCC_CFGR_PLLMULL8, RCC_CFGR_PPRE1_DIV2, FLASH_ACR_LATENCY_1},
    { RCC_CFGR_PLLMULL9, RCC_CFGR_PPRE1_DIV2, FLASH_ACR_LATENCY_1},
};

/*
 * Switch clock frequency
 */
void system_clock_frequency(enum clock_frq frq) {
    RCC->CFGR &= ~RCC_CFGR_SW; /* switch to HSI */
    while ((RCC->CFGR & RCC_CFGR_SWS) != 0);

    RCC->CR &= ~RCC_CR_PLLON; /* disable PLL to change parameters */
    
    FLASH->ACR = (FLASH->ACR & FLASH_ACR_LATENCY) | clock_param_table[frq].flash_latency;
    RCC->CFGR = RCC_CFGR_PLLSRC 
              | clock_param_table[frq].pllmul
              | clock_param_table[frq].ppre1;

    RCC->CR |= RCC_CR_PLLON; /* enable PLL again */
    while(!(RCC->CR & RCC_CR_PLLRDY));

    RCC->CFGR |= RCC_CFGR_SW_PLL; /* switch PLL on */

    system_core_clock_update();
}
