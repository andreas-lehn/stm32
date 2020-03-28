/*
 * Linker script for STM32
 */

MEMORY {
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
    ROM  (rx) : ORIGIN =  0x8000000, LENGTH = 64K
}

STACK_SIZE = 0x1000; /* 4K */

/* Highest address of the user mode stack is end of RAM */
_stack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS {
    .text : {
        . = ALIGN(4);
        KEEP ( * (.rodata.vector_table))
        * (.text .text.*)     
    } > ROM
 
    .data : {
        . = ALIGN(4);
        __data_start = .;
        * (.data .data.*)       
        __data_end = .; 
    } > RAM AT > ROM
    __data_load = LOADADDR(.data);

    .rodata : {
        . = ALIGN(4);
        * (.rodata .rodata.*)
        . = ALIGN(4);
    } > ROM

    .bss : {
        . = ALIGN(4);
        __bss_start__ = .;
        * (.bss .bss.*)
        * (COMMON)
        __bss_end__ = .;
    } > RAM
/*
    .ARM.extab : { 
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ROM
  
    .ARM : {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > ROM

    .ARM.attributes 0 : {
        *(.ARM.attributes)
    }
*/
    .stack (NOLOAD) : {
        . = ALIGN(8);
        . = . + STACK_SIZE;
    } > RAM   
}
