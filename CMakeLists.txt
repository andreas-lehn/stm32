cmake_minimum_required(VERSION 3.16)
project(STM32 C ASM)

include_directories(
    CMSIS/include
    CMSIS/Device/ST/STM32F1xx/Include
    ${CMAKE_SOURCE_DIR}
)

add_compile_options(
    -mcpu=cortex-m3     # specify the name of the target CPU
    #-std=gnu11          # specifies gnu11 as c language dialect.
    -g3                 # debug information level 3
    -O0                 # No optimization (Optimization level 0)
    -ffunction-sections # place each function
    -fdata-sections     # or data item in its own section
    -Wall               # show all warnings
    # -fstack-usage       # ?
    -mfloat-abi=soft    # Specify if floating point hardware should be used.
    -mthumb             #Generate code for Thumb state.
)

add_link_options(
    # -specs=nano.specs  # use newlib instead of libc.
    -T ${CMAKE_SOURCE_DIR}/STM32F103C8TX_FLASH.ld # linker script file
    -static             # force static linking (no shared objects)
    -mfloat-abi=soft    
    -mthumb
    #-lc                 # link library c
    #-lm                 # link library m
    #--gc-sections        # remove unused sections
    -flto                # link time optimization
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

target_link_options(blinky.elf PUBLIC
    -specs=nosys.specs # use libnosys as libc.
)

target_include_directories(blinky.elf PUBLIC
    
)

target_compile_definitions(blinky.elf PUBLIC
    STM32F103xB
)
