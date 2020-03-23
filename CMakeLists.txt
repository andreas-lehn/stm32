cmake_minimum_required(VERSION 3.16)
project(STM32 C ASM)

include_directories(
    CMSIS/include
    CMSIS/Device/ST/STM32F1xx/Include
    #/usr/local/opt/arm-gcc-bin/arm-none-eabi/include
)

add_compile_options(
    -mcpu=cortex-m3
    -std=gnu11
    -g3
    -O0 
    -ffunction-sections 
    -fdata-sections 
    -Wall 
    -fstack-usage 
    -MMD
    -MP 
    -mfloat-abi=soft
    -mthumb
)

add_link_options(
    --specs=nano.specs
    -T /Users/andreas/STM32CubeIDE/stm32/blinky/STM32F103C8TX_FLASH.ld
    -static 
    -mfloat-abi=soft 
    -mthumb
    -lc
    -lm
)

add_executable(blinky.elf
    blinky/src/app/blinky.c
    blinky/src/board/board.c
    blinky/src/board/board.h
    blinky/src/runtime/hooks.h
    blinky/src/runtime/main.c
    blinky/src/runtime/startup_stm32f103c8tx.s
    blinky/src/runtime/syscalls.c
    blinky/src/runtime/sysmem.c
    blinky/src/runtime/system_stm32f1xx.c
)

target_include_directories(blinky.elf PUBLIC
    blinky/src
)

target_compile_definitions(blinky.elf PUBLIC
    STM32F103xB
)
