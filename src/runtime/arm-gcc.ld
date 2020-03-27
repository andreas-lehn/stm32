/*
 * Linker script for STM32
 */

MEMORY {
    RAM    (xrw)    : ORIGIN = 0x20000000,  LENGTH = 20K
    FLASH   (rx)    : ORIGIN = 0x8000000,   LENGTH = 64K
}

/* Highest address of the user mode stack is end of RAM */
_stack_top = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
    .text : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        *(.text)      
        *(.text*)     
        KEEP(*(.init))
        KEEP(*(.fini))
        *(.rodata)
        *(.rodata*)
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    .ARM.extab : { 
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH
  
    .ARM : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    __etext = LOADADDR(.data);

    .data : {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data)        
        *(.data*)       
        . = ALIGN(4);
        __data_end__ = .; 
    } >RAM AT> FLASH

    .bss : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } >RAM

    .ARM.attributes 0 : {
        *(.ARM.attributes)
    }
}
