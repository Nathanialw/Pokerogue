add_library(GameCore STATIC
        core.c
        camera.c
        collision.c
        pathing.c
        tilesets.c
        sound.c
        map.c
        graphics.c
        rendering.c

        animation.c
        animation_effects.c
        animation_skills_attack.c
        animation_spells_attack.c
        animation_items_attack.c
        animation_skills_struck.c
        animation_spells_struck.c
        animation_item_struck.c

        menu.c
        menu_main.c
        menu_battle.c
        ui.c

        effects.c
        effects_objects.c
        effects_spells.c
        effects_skills.c
        effects_items.c

        status_effects.c
        player.c
        battles.c
        entities.c
        stats.c
        ai.c

        memory_ram.c
        memory_rom.c
        game_state.c

        utils.h
        hash_map.c
        types.h
        constants.h
        enums.h
)

target_compile_definitions(GameCore PRIVATE ENABLE_DEBUG_OUTPUT ENABLE_ASSERTS)

target_link_libraries(GameCore
        PicoTypes
)

target_include_directories(GameCore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)


set_target_properties(GameCore PROPERTIES LINKER_LANGUAGE C)

