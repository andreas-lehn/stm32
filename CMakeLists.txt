cmake_minimum_required(VERSION 3.16)
project(STM32 C ASM)

include_directories(
    CMSIS/include
    CMSIS/Device/ST/STM32F1xx/Include
    ${CMAKE_SOURCE_DIR}
)

add_compile_options(
    -Wall               # show all warnings
    -std=gnu11          # specifies gnu11 as c language dialect.
)

add_executable(blinky.elf
    blinky/blinky.c
    blinky/board.c
    blinky/board.h
    runtime/hooks.h
    runtime/main.c
    runtime/startup_stm32f103c8tx.s
    runtime/system_stm32f1xx.c
)

target_compile_definitions(blinky.elf PUBLIC STM32F103xB)

target_link_options(blinky.elf PUBLIC
    -specs=nosys.specs # use libnosys as libc
    -T ${CMAKE_SOURCE_DIR}/runtime/STM32F103C8TX_FLASH.ld  # linker script file
)
