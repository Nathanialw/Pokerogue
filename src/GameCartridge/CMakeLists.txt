#uses no pico code written in C with minimal use of the standard library
#the entry point is called on the pico platform from an SPI call and injects the pico API functions as a parameter
#code can be reused by defining the API funcitons and calling the entry point with a struct of function pointers the code can call
#compiler g++ using Clion

#dummy main so it builds the binary
add_executable(GameCartridge
        main.c
)

target_compile_definitions(GameCore PRIVATE CARTRIDGE)


target_link_options(GameCartridge PRIVATE
        "-nostartfiles"
        "--specs=nosys.specs"
        "-Wl,--gc-sections"
        "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/output.map"
        "-T${CMAKE_CURRENT_SOURCE_DIR}/ram.ld"
)

target_link_libraries(GameCartridge
        "-Wl,--whole-archive"
        GameCore
        PicoTypes
        "-Wl,--no-whole-archive"
)


# ========================= POST BUILD - Create Cartridge ROM =========================
# After the normal GameCartridge executable target
add_custom_command(TARGET GameCartridge POST_BUILD
        COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/python/cartridge/make_cartridge.py"
)