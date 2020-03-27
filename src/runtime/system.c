
#include "stm32f1xx.h"
#include <stdint.h>

#define HSE_VALUE 8000000U /* Default value of the External oscillator in Hz. */
#define HSI_VALUE 8000000U /* Default value of the Internal oscillator in Hz. */

#define VECT_TAB_OFFSET 0 /* Vector Table base offset field. This value must be a multiple of 0x200. */

unsigned system_core_clock = 16000000;

const int ahb_prescale_divisor[] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 6, 8, 12, 14, 16, 18};
const int apb_prescale_divisor[] =  {1, 1, 1, 1, 2, 4, 6, 8};

/*
 * Setup the microcontroller system
 */
void system_init() {
  /* Reset the RCC clock configuration to the default reset state (for debug purpose) */
  /* Set HSION bit */
  RCC->CR |= 0x00000001;

  /* Reset PLLON, CSSON, HSEBYP and HSEONand bits, keep HSERDY */
  RCC->CR &= 0xFEF0FFFF;

  /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
  RCC->CFGR &= 0xF8800000;
  
  /* Disable all interrupts and clear pending bits  */
  RCC->CIR = 0x009F0000;
    
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

    uint32_t pll_multiplier = (RCC->CFGR & RCC_CFGR_PLLMULL >> 18) +2;
    return pll_base * pll_multiplier;
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
  int prescaler = RCC->CFGR & RCC_CFGR_HPRE >> 4;
  system_core_clock /= ahb_prescale_divisor[prescaler];  
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
