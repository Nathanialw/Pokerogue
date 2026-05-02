cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(PICO)
    set(PICOTOOL_FETCH_FROM_GIT_PATH ${CMAKE_SOURCE_DIR}/vendor/pico-tools)
    set(PICOTOOL_FORCE_FETCH_FROM_GIT PICOTOOL_FETCH_FROM_GIT_PATH BOOL "Force fetch picotool from git")

    set(PICO_FLASH_SIZE_BYTES 4194304) # 4 * 1024 * 1024

    set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/vendor/pico-sdk)
    include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

    set(PICO_EXTRAS_PATH ${CMAKE_SOURCE_DIR}/vendor/pico-extras)
    include(${PICO_EXTRAS_PATH}/pico_sdk_import.cmake)

    add_compile_definitions(PICO_FLASH_SIZE_BYTES=4194304) # 4 * 1024 * 1024
endif()


project(PicoRogue C CXX ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 26)

if(PICO)
    pico_sdk_init()
    set(PICO_EXTRAS_ENABLE_AUDIO 1)
    add_subdirectory(${PICO_EXTRAS_PATH}/src/common/pico_util_buffer)
    add_subdirectory(${PICO_EXTRAS_PATH}/src/common/pico_audio)
    add_subdirectory(${PICO_EXTRAS_PATH}/src/rp2_common/pico_audio_i2s)
endif()

add_subdirectory(src/GameCore)
add_subdirectory(src/tools/image_viewer)
add_subdirectory(src/tools/music_editor)
add_subdirectory(src/PicoTypes)

if(BUILD_TESTS)
    add_subdirectory(src/GameCoreTests)
elseif(SDL)
    add_subdirectory(src/SDLGame)
elseif(PICO)
    add_subdirectory(src/PicoGame)
endif()
