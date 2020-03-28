/*
 * Vector table for a STM32/Cortex-M3 processor.
 */

/*
 * Top of the stack address has to be provided by the linker.
 * It is defined in the linker script file.
 */
extern int _stack_top;

/*
 * Default Handler for unhandled exeptions/interrupts.
 */ 
static void default_handler() {
    while(1);
}

/*
 * Startup function is the first entry point a startup/after reset.
 */
void on_reset()           __attribute__ ((weak, alias("default_handler")));

/*
 * Exceptions
 */
void on_nmi()             __attribute__ ((weak, alias("default_handler")));
void on_hard_fault()      __attribute__ ((weak, alias("default_handler")));
void on_mem_mgnt_fault()  __attribute__ ((weak, alias("default_handler")));
void on_bus_fault()       __attribute__ ((weak, alias("default_handler")));
void on_usage_fault()     __attribute__ ((weak, alias("default_handler")));
void on_sv_call()         __attribute__ ((weak, alias("default_handler")));
void on_debug_mon()       __attribute__ ((weak, alias("default_handler")));
void on_pend_sv()         __attribute__ ((weak, alias("default_handler")));
void on_sys_tick()        __attribute__ ((weak, alias("default_handler")));

/*
 * Interrupts
 */
void on_win_watchdog()    __attribute__ ((weak, alias("default_handler")));
void on_pvd()             __attribute__ ((weak, alias("default_handler")));
void on_tamper()          __attribute__ ((weak, alias("default_handler")));
void on_rtc()             __attribute__ ((weak, alias("default_handler")));
void on_flash()           __attribute__ ((weak, alias("default_handler")));
void on_rcc()             __attribute__ ((weak, alias("default_handler")));
void on_ext_int0()        __attribute__ ((weak, alias("default_handler")));
void on_ext_int1()        __attribute__ ((weak, alias("default_handler")));
void on_ext_int2()        __attribute__ ((weak, alias("default_handler")));
void on_ext_int3()        __attribute__ ((weak, alias("default_handler")));
void on_ext_int4()        __attribute__ ((weak, alias("default_handler")));
void on_dma_channel1()    __attribute__ ((weak, alias("default_handler")));
void on_dma_channel2()    __attribute__ ((weak, alias("default_handler")));
void on_dma_channel3()    __attribute__ ((weak, alias("default_handler")));
void on_dma_channel4()    __attribute__ ((weak, alias("default_handler")));
void on_dma_channel5()    __attribute__ ((weak, alias("default_handler")));
void on_dma_channel6()    __attribute__ ((weak, alias("default_handler")));
void on_dma_channel7()    __attribute__ ((weak, alias("default_handler")));
void on_adc1_2()          __attribute__ ((weak, alias("default_handler")));
void on_usb_hp_can1_tx0() __attribute__ ((weak, alias("default_handler")));
void on_usb_lp_can1_rx0() __attribute__ ((weak, alias("default_handler")));
void on_can1_rx1()        __attribute__ ((weak, alias("default_handler")));
void on_can1_sce()        __attribute__ ((weak, alias("default_handler")));
void on_ext_int9_5()      __attribute__ ((weak, alias("default_handler")));
void on_timer1_brk()      __attribute__ ((weak, alias("default_handler")));
void on_timer1_up()       __attribute__ ((weak, alias("default_handler")));
void on_timer1_trg_com()  __attribute__ ((weak, alias("default_handler")));
void on_timer1_cc()       __attribute__ ((weak, alias("default_handler")));
void on_timer2()          __attribute__ ((weak, alias("default_handler")));
void on_timer3()          __attribute__ ((weak, alias("default_handler")));
void on_timer4()          __attribute__ ((weak, alias("default_handler")));
void on_i2c1_ev()         __attribute__ ((weak, alias("default_handler")));
void on_i2c1_er()         __attribute__ ((weak, alias("default_handler")));
void on_i2c2_ev()         __attribute__ ((weak, alias("default_handler")));
void on_i2c2_er()         __attribute__ ((weak, alias("default_handler")));
void on_spi1()            __attribute__ ((weak, alias("default_handler")));
void on_spi2()            __attribute__ ((weak, alias("default_handler")));
void on_usart1()          __attribute__ ((weak, alias("default_handler")));
void on_usart2()          __attribute__ ((weak, alias("default_handler")));
void on_usart3()          __attribute__ ((weak, alias("default_handler")));
void on_ext_int15_10()    __attribute__ ((weak, alias("default_handler")));
void on_rtc_alarm()       __attribute__ ((weak, alias("default_handler")));
void on_usb_wakeup()      __attribute__ ((weak, alias("default_handler")));

/*
 * Definition of the address vector.
 */
const void * const vector_table[] = {
    &_stack_top,
    on_reset,
    on_nmi,
    on_hard_fault,
    on_mem_mgnt_fault,
    on_bus_fault,
    on_usage_fault,
    0,
    0,
    0,
    0,
    on_sv_call,
    on_debug_mon,
    0,
    on_pend_sv,
    on_sys_tick,
    on_win_watchdog,
    on_pvd,
    on_tamper,
    on_rtc,
    on_flash,
    on_rcc,
    on_ext_int0,
    on_ext_int1,
    on_ext_int2,
    on_ext_int3,
    on_ext_int4,
    on_dma_channel1,
    on_dma_channel2,
    on_dma_channel3,
    on_dma_channel4,
    on_dma_channel5,
    on_dma_channel6,
    on_dma_channel7,
    on_adc1_2,
    on_usb_hp_can1_tx0,
    on_usb_lp_can1_rx0,
    on_can1_rx1,
    on_can1_sce,
    on_ext_int9_5,
    on_timer1_brk,
    on_timer1_up,
    on_timer1_trg_com,
    on_timer1_cc,
    on_timer2,
    on_timer3,
    on_timer4,
    on_i2c1_ev,
    on_i2c1_er,
    on_i2c2_ev,
    on_i2c2_er,
    on_spi1,
    on_spi2,
    on_usart1,
    on_usart2,
    on_usart3,
    on_ext_int15_10,
    on_rtc_alarm,
    on_usb_wakeup
};
