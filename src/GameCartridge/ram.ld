/*
1MB	    1,048,576	0x00100000	0x0FFFFF (1,048,575)
2MB	    2,097,152	0x00200000	0x1FFFFF
4MB	    4,194,304	0x00400000	0x3FFFFF
8MB	    8,388,608	0x00800000	0x7FFFFF
16MB	16,777,216	0x01000000	0xFFFFFF (16,777,215)
16MB	16,777,216	0x02000000	0xFFFFFF
32MB	16,777,216	0x04000000	0xFFFFFF
64MB	16,777,216	0x08000000	0xFFFFFF
128MB	16,777,216	0x10000000	0xFFFFFF
256MB	16,777,216	0x20000000	0xFFFFFF
*/

/*16MB is used right now 16777216*/
MEMORY
{
    RAM (rwx) : ORIGIN = 0x20010000, LENGTH = 4M
}

SECTIONS
{
    /* Start at address 0 */
    . = 0x000000;

    .core 0x20000000 : AT(0x0000000)   /* example cartridge address */
    {
        KEEP(*(.core))
        KEEP(*(.core.text))    /* code */
        KEEP(*(.core.rodata))  /* read‑only data (strings, const) */
        KEEP(*(.core.data))    /* initialised data (if any) */
        KEEP(*(.core.bss))     /* zero‑initialised data (if any) */
        . = ALIGN(4);
    }

    .splash 0x20010000 : AT(0x020000)
    {
        KEEP(*(.splash_entry))   /* must come first */
        KEEP(*(.splash.text))    /* code */
        KEEP(*(.splash.rodata))  /* read‑only data (strings, const) */
        KEEP(*(.splash.data))    /* initialised data (if any) */
        KEEP(*(.splash.bss))     /* zero‑initialised data (if any) */
        KEEP(*(.splash))
        . = ALIGN(4);
    }

    .map 0x20010000 : AT(0x030000)
    {
        KEEP(*(.map_entry))
        KEEP(*(.map.text))    /* code */
        KEEP(*(.map.rodata))  /* read‑only data (strings, const) */
        KEEP(*(.map.data))    /* initialised data (if any) */
        KEEP(*(.map.bss))     /* zero‑initialised data (if any) */
        KEEP(*(.map))
        . = ALIGN(4);
    }

    .map_gen 0x20010000 : AT(0x040000)
    {
        KEEP(*(.map_gen_entry))
        KEEP(*(.map_gen.text))    /* code */
        KEEP(*(.map_gen.rodata))  /* read‑only data (strings, const) */
        KEEP(*(.map_gen.data))    /* initialised data (if any) */
        KEEP(*(.map_gen.bss))     /* zero‑initialised data (if any) */
        KEEP(*(.map_gen))
        . = ALIGN(4);
    }

    /* Battle / Map Overlays */
    .battle 0x20010000 : AT(0x050000)
    {
        KEEP(*(.battle_entry))
        KEEP(*(.battle.text))    /* code */
        KEEP(*(.battle.rodata))  /* read‑only data (strings, const) */
        KEEP(*(.battle.data))    /* initialised data (if any) */
        KEEP(*(.battle.bss))     /* zero‑initialised data (if any) */
        KEEP(*(.battle))
        . = ALIGN(4);
    }

    /* ========================================== */
    /*     FORCE ABSOLUTE POSITIONS FROM HERE     */
    /* ========================================== */

    . = 0x1500000;

    /* ========================================== */
    /*     MAP SPRITE DATA                        */
    /* ========================================== */
    .char_sprites_items              0x1500000 : { KEEP(*(.char_sprites_items))      . = ALIGN(4); }
    .char_sprites_monsters           0x1510000 : { KEEP(*(.char_sprites_monsters))      . = ALIGN(4); }
    .char_sprites_objects            0x1520000 : { KEEP(*(.char_sprites_objects))      . = ALIGN(4); }
    .char_sprites_biomes             0x1530000 : { KEEP(*(.char_sprites_biomes))      . = ALIGN(4); }

    /* ========================================== */
    /*     TIELSETS                               */
    /* ========================================== */
    .tileset_theme                   0x1600000 : { KEEP(*(.tileset_theme))      . = ALIGN(4); }
    .tileset_creatures               0x1610000 : { KEEP(*(.tileset_creatures))      . = ALIGN(4); }

    /* ========================================== */
    /*     STRINGS                                */
    /* ========================================== */
    .strings_menu_main               0x1700000 : { KEEP(*(.strings_menu_main))     . = ALIGN(4); }
    .strings_menu_battle             0x1710000 : { KEEP(*(.strings_menu_battle))     . = ALIGN(4); }
    .strings_menu_options            0x1720000 : { KEEP(*(.strings_menu_options))     . = ALIGN(4); }
    .strings_names_items             0x1730000 : { KEEP(*(.strings_names_items))     . = ALIGN(4); }
    .strings_names_creatures         0x1740000 : { KEEP(*(.strings_names_creatures))  . = ALIGN(4); }
    .strings_names_spells            0x1750000 : { KEEP(*(.strings_names_spells))    . = ALIGN(4); }
    .strings_names_skills            0x1760000 : { KEEP(*(.strings_names_skills))   . = ALIGN(4); }
    .strings_names_objects           0x1770000 : { KEEP(*(.strings_names_objects))   . = ALIGN(4); }
    .strings_names_trainers          0x1780000 : { KEEP(*(.strings_names_trainers))  . = ALIGN(4); }
    .strings_descriptions_items      0x1790000 : { KEEP(*(.strings_descriptions_items))     . = ALIGN(4); }
    .strings_descriptions_creatures  0x1800000 : { KEEP(*(.strings_descriptions_creatures))  . = ALIGN(4); }
    .strings_descriptions_spells     0x1810000 : { KEEP(*(.strings_descriptions_spells))    . = ALIGN(4); }
    .strings_descriptions_skills     0x1820000 : { KEEP(*(.strings_descriptions_skills))   . = ALIGN(4); }
    .strings_descriptions_objects    0x1830000 : { KEEP(*(.strings_descriptions_objects))   . = ALIGN(4); }
    .strings_descriptions_trainers   0x1840000 : { KEEP(*(.strings_descriptions_trainers))  . = ALIGN(4); }
    .strings_types                   0x1850000 : { KEEP(*(.strings_types))  . = ALIGN(4); }

    /* ========================================== */
    /*     GRAPHICS                               */
    /* ========================================== */
    .sprite_battler_layout_back      0x1860000 : { KEEP(*(.sprite_battler_layout_back))  . = ALIGN(4); }
    .sprite_battler_layout_front     0x1870000 : { KEEP(*(.sprite_battler_layout_front))  . = ALIGN(4); }
    .sprite_battler_back             0x1880000 : { KEEP(*(.sprite_battler_back))  . = ALIGN(4); }
    .sprite_battler_front            0x19500000 : { KEEP(*(.sprite_battler_front))  . = ALIGN(4); }
    .sprite_items_layout             0x2040000 : { KEEP(*(.sprite_items_layout))  . = ALIGN(4); }
    .sprite_creatures_layout         0x2050000 : { KEEP(*(.sprite_creatures_layout))  . = ALIGN(4); }
    .sprite_spells_layout            0x2060000 : { KEEP(*(.sprite_spells_layout))  . = ALIGN(4); }
    .sprite_skills_layout            0x2070000 : { KEEP(*(.sprite_skills_layout))  . = ALIGN(4); }
    .sprite_objects_layout           0x2080000 : { KEEP(*(.sprite_objects_layout))  . = ALIGN(4); }
    .sprite_trainers_layout          0x2090000 : { KEEP(*(.sprite_trainers_layout))  . = ALIGN(4); }
    .sprite_items                    0x2000000 : { KEEP(*(.sprite_items))  . = ALIGN(4); }
    .sprite_creatures                0x2100000 : { KEEP(*(.sprite_creatures))  . = ALIGN(4); }
    .sprite_spells                   0x2200000 : { KEEP(*(.sprite_spells))  . = ALIGN(4); }
    .sprite_skills                   0x2300000 : { KEEP(*(.sprite_skills))  . = ALIGN(4); }
    .sprite_objects                  0x2400000 : { KEEP(*(.sprite_objects))  . = ALIGN(4); }
    .sprite_trainers                 0x2500000 : { KEEP(*(.sprite_trainers))  . = ALIGN(4); }
    .sprite_map_creature             0x2600000 : { KEEP(*(.sprite_map_creature))  . = ALIGN(4); }
    .sprite_map_item                 0x2700000 : { KEEP(*(.sprite_map_item))  . = ALIGN(4); }
    .sprite_map_object               0x2800000 : { KEEP(*(.sprite_map_object))  . = ALIGN(4); }
    .sprite_map_trainer              0x2900000 : { KEEP(*(.sprite_map_trainer))  . = ALIGN(4); }
    .sprite_map_tile                 0x3000000 : { KEEP(*(.sprite_map_tile))  . = ALIGN(4); }
    .sprite_map_creature_metaData    0x3100000 : { KEEP(*(.sprite_map_creature_metaData))  . = ALIGN(4); }
    .sprite_map_item_metaData        0x3200000 : { KEEP(*(.sprite_map_item_metaData))  . = ALIGN(4); }
    .sprite_map_object_metaData      0x3300000 : { KEEP(*(.sprite_map_object_metaData))  . = ALIGN(4); }
    .sprite_map_trainer_metaData     0x3400000 : { KEEP(*(.sprite_map_trainer_metaData))  . = ALIGN(4); }
    .sprite_map_tile_metaData        0x3500000 : { KEEP(*(.sprite_map_tile_metaData))  . = ALIGN(4); }

    /* ========================================== */
    /*     FONTS                                  */
    /* ========================================== */
    .font_8x8                        0x3600000 : { KEEP(*(.font_8x8))  . = ALIGN(4); }
    .font_16x16                      0x3700000 : { KEEP(*(.font_16x16))  . = ALIGN(4); }

    /* ========================================== */
    /*      GAME DATA                             */
    /* ========================================== */
    .game_data_type_effects          0x4500000 : { KEEP(*(.game_data_type_effects))  . = ALIGN(4); }
    .game_data_creature_types        0x4600000 : { KEEP(*(.game_data_creature_types))  . = ALIGN(4); }
    .game_data_creature_stats        0x4700000 : { KEEP(*(.game_data_creature_stats))  . = ALIGN(4); }
    .game_data_learnable_skills      0x4800000 : { KEEP(*(.game_data_learnable_skills))  . = ALIGN(4); }
    .game_data_level_up_skills       0x4900000 : { KEEP(*(.game_data_level_up_skills))  . = ALIGN(4); }
    .game_data_ability               0x5000000 : { KEEP(*(.game_data_ability))  . = ALIGN(4); }
    .game_data_spell                 0x5100000 : { KEEP(*(.game_data_spell))  . = ALIGN(4); }
    .game_data_item                  0x5200000 : { KEEP(*(.game_data_item))  . = ALIGN(4); }
    .game_data_object                0x5300000 : { KEEP(*(.game_data_object))  . = ALIGN(4); }

    /* ========================================== */
    /*      COLORS                                */
    /* ========================================== */
    .colors_16                       0x6000000 : { KEEP(*(.colors_16))  . = ALIGN(4); }
    .colors_256                      0x6100000 : { KEEP(*(.colors_256))  . = ALIGN(4); }
    /* ========================================== */
    /*      SOUNDS                                */
    /* ========================================== */
    .sounds_music_data               0x6200000 : { KEEP(*(.sounds_music_data))  . = ALIGN(4); }
    .sounds_music_notes              0x6300000 : { KEEP(*(.sounds_music_notes))  . = ALIGN(4); }
    .sounds_creatures                0x6400000 : { KEEP(*(.sounds_creatures))  . = ALIGN(4); }
    .sounds_spells                   0x6500000 : { KEEP(*(.sounds_spells))  . = ALIGN(4); }
    .sounds_skills                   0x6600000 : { KEEP(*(.sounds_skills))  . = ALIGN(4); }
    .sounds_menus                    0x6700000 : { KEEP(*(.sounds_menus))  . = ALIGN(4); }
    .sounds_notes                    0x6800000 : { KEEP(*(.sounds_notes))  . = ALIGN(4); }


    . = 0x7000000;


    /* This is the critical one */
/*    . = 0x650000;*/
/*    .sprites_objects    0x900000 : { KEEP(*(.sprites_objects))    . = ALIGN(4); }*/

    __rom_end = .;
    end = .;
}
