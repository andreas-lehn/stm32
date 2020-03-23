set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})

set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft -ffunction-sections -fdata-sections")
#  -mcpu=cortex-m3      Specify the name of the target CPU
#  -mfloat-abi=soft     Specify if floating point hardware should be used.
#  -mthumb              Generate code for Thumb state.
#  -ffunction-sections  Place each function
#  -fdata-sections      or data item in its own section

set(CMAKE_C_FLAGS_DEBUG "-g3 -O0")
#    -g3                debug information level 3
#    -O0                No optimization (Optimization level 0)

set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
#    -O3                Optimization for speed (Optimization level 3)
#    -DNDEBUG           Set NDEBUG

set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
#    -Os                Optimization for size
#    -DNDEBUG           Set NDEBUG

set(CMAKE_C_FLAGS_RELWITHDBGINFO "-Og -g3 -DNDEBUG")
#    -Og                Optimization for debug (Optimization level 3)
#    -g3                Include debug info
#    -DNDEBUG           Set NDEBUG

set(CMAKE_LDFLAGS "-static -mfloat-abi=soft -mthumb -flto")
#    -static             # force static linking (no shared objects)
#    -mfloat-abi=soft    
#    -mthumb
#    --gc-sections        # remove unused sections. Does not work unfortunately...
#    -flto                # link time optimization

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
