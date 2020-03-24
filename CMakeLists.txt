cmake_minimum_required(VERSION 3.16)
project(STM32 C ASM)

include_directories(
    src
    src/CMSIS/include
    src/CMSIS/Device/ST/STM32F1xx/Include
    # ${CMAKE_SOURCE_DIR}
)

add_compile_options(
    -Wall               # show all warnings
    -std=gnu11          # specifies gnu11 as c language dialect.
)

add_executable(blinky.elf
    src/blinky/blinky.c
    src/blinky/board.c
    src/blinky/board.h
    src/runtime/hooks.h
    src/runtime/main.c
    src/runtime/startup_stm32f103c8tx.s
    src/runtime/system_stm32f1xx.c
)

target_compile_definitions(blinky.elf PUBLIC STM32F103xB)

target_link_options(blinky.elf PUBLIC
    -specs=nosys.specs # use libnosys as libc
    -T ${CMAKE_SOURCE_DIR}/src/runtime/STM32F103C8TX_FLASH.ld  # linker script file
)
