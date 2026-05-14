add_executable(PicoRogue
        main.c
        init.c

        ili9341.c
        input.c
        sounds.c
        hardware.c

        cartridge.c
        cartridge_rom.c
        cartridge_save.c

        memory_ram.c
        memory_rom.c
        memory_psram.c

        pico_constants.h
)


target_link_libraries(PicoRogue
        PicoTypes
        GameCore
        pico_stdlib
        hardware_spi
        hardware_gpio
        hardware_adc
        hardware_pio
        hardware_i2c
        hardware_pwm
        pico_audio_i2s
        pico_audio
        pico_multicore
)



target_compile_definitions(PicoRogue PRIVATE ENABLE_DEBUG_OUTPUT ENABLE_ASSERTS)
target_compile_definitions(PicoTypes PRIVATE ENABLE_DEBUG_OUTPUT ENABLE_ASSERTS)
#target_compile_definitions(GameCore PRIVATE ENABLE_DEBUG_OUTPUT ENABLE_ASSERTS)


target_include_directories(PicoRogue PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)


pico_set_linker_script(PicoRogue ${CMAKE_CURRENT_LIST_DIR}/memmap_default.ld)
pico_set_linker_script(PicoRogue ${CMAKE_CURRENT_LIST_DIR}/memmap_default.ld)
pico_enable_stdio_usb(PicoRogue 1)
pico_enable_stdio_uart(PicoRogue 0)

# And update target_link_options:
target_link_options(PicoRogue PRIVATE
        -Wl,--defsym=__flash_size=4M
        -Wl,--defsym=__flash_binary_end=0x101A5858
        -Wl,--gc-sections
        -Wl,--strip-all
#        -T${CMAKE_CURRENT_SOURCE_DIR}/memmap_default.ld
#        -Wl,-T ${CMAKE_CURRENT_SOURCE_DIR}/custom_memory.ld
)


pico_add_extra_outputs(PicoRogue)
