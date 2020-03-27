cmake_minimum_required(VERSION 3.16)
project(STM32 C ASM)

include_directories(
    src
    lib/CMSIS/Device/ST/STM32F1xx/Include
    lib/CMSIS/include
)

add_compile_options(
    -Wall               # show all warnings
    -std=gnu11          # specifies gnu11 as c language dialect.
)

add_executable(blinky.elf
    src/blinky/blinky.c
    src/blinky/board.c
    src/blinky/board.h
    src/framework/hooks.h
    src/framework/main.c
    src/runtime/crt.c
    src/runtime/vector_table.c
    src/runtime/system.h
    src/runtime/system.c
)

target_compile_definitions(blinky.elf PUBLIC STM32F103xB)

target_link_options(blinky.elf PUBLIC
    -specs=nosys.specs # use libnosys as libc
    -nostartfiles
    -T ${CMAKE_SOURCE_DIR}/src/runtime/arm-gcc.ld
)

add_executable(timer-demo.elf
    src/demos/timer.c
    src/runtime/crt.c
    src/runtime/vector_table.c
    src/runtime/system.h
    src/runtime/system.c
)

target_compile_definitions(timer-demo.elf PUBLIC STM32F103xB)

target_link_options(timer-demo.elf PUBLIC
    -specs=nosys.specs # use libnosys as libc
    -nostartfiles
    -T ${CMAKE_SOURCE_DIR}/src/runtime/arm-gcc.ld
)

add_executable(servo.elf
    src/servo/servo.c
    src/servo/board.h
    src/servo/board.c
    src/framework/hooks.h
    src/framework/main.c
    src/runtime/crt.c
    src/runtime/vector_table.c
    src/runtime/system.h
    src/runtime/system.c
)

target_compile_definitions(servo.elf PUBLIC STM32F103xB)

target_link_options(servo.elf PUBLIC
    -specs=nosys.specs # use libnosys as libc
    -nostartfiles
    -T ${CMAKE_SOURCE_DIR}/src/runtime/arm-gcc.ld
)
