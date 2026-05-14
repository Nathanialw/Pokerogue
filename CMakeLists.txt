cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (PICO)
    set(PICOTOOL_FETCH_FROM_GIT_PATH ${CMAKE_SOURCE_DIR}/vendor/pico-tools)
    set(PICOTOOL_FORCE_FETCH_FROM_GIT PICOTOOL_FETCH_FROM_GIT_PATH BOOL "Force fetch picotool from git")

    set(PICO_FLASH_SIZE_BYTES 4194304) # 4 * 1024 * 1024
#    set(PICO_MEMORY_LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/PicoGame/custom_memory.ld)
#    set(PICO_RAM_ORIGIN 0x20020000 CACHE STRING "")
#    set(PICO_RAM_LENGTH 0x20000 CACHE STRING "")   # 128K in hex

    set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/vendor/pico-sdk)
    include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

    set(PICO_EXTRAS_PATH ${CMAKE_SOURCE_DIR}/vendor/pico-extras)
    include(${PICO_EXTRAS_PATH}/pico_sdk_import.cmake)


    add_compile_definitions(PICO_FLASH_SIZE_BYTES=4194304) # 4 * 1024 * 1024
elseif (CARTRIDGE)
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_PROCESSOR arm)

    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

    set(CMAKE_AR arm-none-eabi-ar)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_OBJDUMP arm-none-eabi-objdump)

    # Important flags for bare metal
    set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-m0plus -mthumb -ffreestanding -fno-tree-loop-distribute-patterns -fno-builtin -nostartfiles")
    set(CMAKE_CXX_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}")

    # Tell CMake not to run full link tests during configuration
#    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
endif ()

project(PicoRogue C CXX ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 26)


if (PICO)
    pico_sdk_init()
    set(PICO_EXTRAS_ENABLE_AUDIO 1)
    add_subdirectory(${PICO_EXTRAS_PATH}/src/common/pico_util_buffer)
    add_subdirectory(${PICO_EXTRAS_PATH}/src/common/pico_audio)
    add_subdirectory(${PICO_EXTRAS_PATH}/src/rp2_common/pico_audio_i2s)
endif ()

add_subdirectory(src/PicoTypes)
add_subdirectory(src/GameCore)
add_subdirectory(src/tools/image_viewer)
add_subdirectory(src/tools/music_editor)

if (BUILD_TESTS)
    add_subdirectory(src/GameCoreTests)
elseif (SDL)
    add_subdirectory(src/SDLGame)
elseif (PICO)
    add_subdirectory(src/PicoGame)
elseif (CARTRIDGE)
    add_subdirectory(src/GameCartridge)
endif ()
