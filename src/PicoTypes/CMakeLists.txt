add_library(PicoTypes STATIC
        lib_types.h
        lib_enums.h
        lib_constants.h
)

target_include_directories(PicoTypes PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(PicoTypes PROPERTIES LINKER_LANGUAGE C)

