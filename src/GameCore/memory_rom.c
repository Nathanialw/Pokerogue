//
// Created by nathanial on 2/26/26.
//

#include "memory_rom.h"
#include "types.h"
#include "enums.h"
#include "inc/decl_skills.inc"
#include "inc/decl_items.inc"
#include "inc/decl_spells.inc"
#include "inc/decl_objects.inc"
#include "inc/decl_animation_items_attack.inc"
#include "inc/decl_animation_skills_attack.inc"
#include "inc/decl_animation_spells_attack.inc"
#include "inc/decl_animation_items_struck.inc"
#include "inc/decl_animation_skills_struck.inc"
#include "inc/decl_animation_spells_struck.inc"
#include "lib_memory.h"


SET_MEMORY(".sprites_objects")
const GameFlash g_gameFlash =
{
    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .sprites = {
        .items =
        {
#include "inc/map_sprites_items.inc"
        },
        .monsters =
        {
#include "inc/map_sprites_creatures.inc"
        },
        .objects =
        {
#include "inc/map_sprites_objects.inc"
        },
        .biomes =
        {
#include "inc/map_sprite_biome_tiles.inc"
        },
    },

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .tileset = {
        .monsterGroups =
        {
#include "inc/data_tileset_monsterGroups.inc"

        },
        .themeGroups =
        {
#include "inc/data_tileset_themeGroups.inc"
        }
    },

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .text =
    {
        .menus =
        {
#include "inc/text_menus.inc"
        },

        .names =
        {
            .items =
            {
#include "inc/text_names_items.inc"
            },
            .monsters =
            {
#include "inc/text_names_creatures.inc"
            },
            .trainers =
            {
#include "inc/text_names_trainers.inc"
            },
            .spells =
            {
#include "inc/text_names_spells.inc"
            },
            .attacks =
            {
#include "inc/text_names_skills.inc"
            },
            .objects =
            {
#include "inc/text_names_objects.inc"
            },
        },
        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .descriptions =
        {
            .items =
            {
#include "inc/text_desc_items.inc"
            },
            .monsters =
            {
#include "inc/text_desc_creatures.inc"
            },
            .trainers =
            {
#include "inc/text_desc_trainers.inc"
            },
            .spells =
            {
#include "inc/text_desc_spells.inc"
            },
            .attacks =
            {
#include "inc/text_desc_skills.inc"
            },
            .objects =
            {
#include "inc/text_desc_objects.inc"
            },
        },

        .types =
        {
#include "inc/text_types.inc"
        }
    },

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .spriteData =
    {
#ifdef INCLUDE_BATTLERS
        .battlers = {
            .backLayout =
            {
#include "inc/sprite_creature_back_layout.inc"
            },
            .back =
            {
#include "inc/sprite_creature_back.inc"
            },

            .frontLayout =
            {
#include "inc/sprite_creature_front_layout.inc"
            },
            .front =
            {
#include "inc/sprite_creature_front.inc"
            },
        },

        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .itemLayout =
        {
#include "inc/sprite_item_layout.inc"
        },
        .items =
        {
#include "inc/sprite_item.inc"
        },

        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .skillLayout =
        {
#include "inc/sprite_skill_layout.inc"
        },
        .skills =
        {
#include "inc/sprite_skill.inc"
        },

        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .spellLayout =
        {
#include "inc/sprite_spell_layout.inc"
        },
        .spells =
        {
#include "inc/sprite_spell.inc"
        },

        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .objectLayout =
        {
#include "inc/sprite_object_layout.inc"
        },
        .objects =
        {
#include "inc/sprite_object.inc"
        },

        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .trainerLayout =
        {
#include "inc/sprite_trainer_layout.inc"
        },
        .trainers =
        {
#include "inc/sprite_trainer.inc"
        },
#endif


        /**********************************************************************************************************************/
        /*  map Sprites
        **********************************************************************************************************************/
        .creatureSprites =
        {
#include "inc/sprite_map_creatures.inc"
        },
        .trainerSprites =
        {
#include "inc/sprite_map_trainers.inc"
        },
        .objectSprites =
        {
#include "inc/sprite_map_objects.inc"
        },
        .itemSprites =
        {
#include "inc/sprite_map_items.inc"
        },
        .tileSprites =
        {
#include "inc/sprite_map_tiles.inc"
        },

        /**********************************************************************************************************************/
        /* text font data
        **********************************************************************************************************************/
        .font8x8 =
        {
#include "inc/data_font8x8.inc"

        },

        .font16x16 =
        {
#include "inc/data_font16x16.inc"
        },
    },

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .sounds =
    {
        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .music =
        {
            .musicData =
            {
#include "inc/sounds_music_data.inc"
            },
            .songs =
            {
#include "inc/sounds_music.inc"
            },
        },
        /**********************************************************************************************************************/
        /*
        **********************************************************************************************************************/
        .effects =
        {
            .creatures =
            {
#include "inc/sounds_creatures.inc"
            },
            .spells =
            {
#include "inc/sounds_spells.inc"
            },
            .attacks =
            {
#include "inc/sounds_skills.inc"
            },
            .menus =
            {
#include "inc/sounds_menu.inc"
            }
        }
    },
    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .gameData =
    {
        .typeEffects =
        {
#include "inc/data_type_effects.inc"

        },
        .creatureTypes =
        {
#include "inc/data_types_creatures.inc"

        },
        .creatureStats =
        {
#include "inc/data_creature_stats.inc"
        },
        .learnableSkills =
        {
#include "inc/data_creature_skills.inc"
        },
        .levelUpSkills =
        {
#include "inc/data_creature_level_up_skills.inc"
        },

        .abilityData =
        {
#include "inc/data_skills.inc"
        },
        .spellData =
        {
#include "inc/data_spells.inc"
        },
        .itemData =
        {
#include "inc/data_items.inc"
        },
        .objectData =
        {
#include "inc/data_objects.inc"
        },
    },
    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .animation =
    {
        .itemsAttack =
        {
#include "inc/funcs_animation_items_attack.inc"
        },
        .itemsStruck =
        {
#include "inc/funcs_animation_items_struck.inc"
        },
        .spellsAttack =
        {
#include "inc/funcs_animation_spells_attack.inc"
        },
        .spellsStruck =
        {
#include "inc/funcs_animation_spells_struck.inc"
        },
        .skillsAttack =
        {
#include "inc/funcs_animation_skills_attack.inc"
        },
        .skillsStruck =
        {
#include "inc/funcs_animation_skills_struck.inc"
        },
    },

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .funcs =
    {
        .abilityFunctions =
        {
#include "inc/funcs_skills.inc"
        },
        .spellFunctions =
        {
#include "inc/funcs_spells.inc"
        },
        .itemFunctions =
        {
#include "inc/funcs_items.inc"
        },
        .objectFunctions =
        {
#include "inc/funcs_objects.inc"
        },
    },
    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    .GetColor =
    {
#include "inc/data_colors16.inc"

    },
    .GetColor256 =
    {
#include "inc/data_colors256.inc"
    },

    .noteFreqFixed =
    {
        /** 0 */ 818, 866, 918, 972, 1030, 1091, 1156, 1225, 1298, 1375, 1457, 1543,
        /*12 */ 1635, 1732, 1835, 1945, 2060, 2183, 2312, 2450, 2596, 2750, 2914, 3087,
        /*24 */ 3270, 3465, 3671, 3889, 4120, 4365, 4625, 4900, 5191, 5500, 5827, 6174,
        /*36 */ 6541, 6930, 7342, 7778, 8241, 8731, 9250, 9800, 10383, 11000, 11654, 12347,
        /*48 */ 13081, 13859, 14683, 15556, 16481, 17461, 18500, 19600, 20765, 22000, 23308, 24694,
        /*60 */ 26163, 27718, 29366, 31113, 32963, 34923, 36999, 39200, 41530, 44000, 46616, 49388,
        /*72 */ 52325, 55437, 58733, 62225, 65925, 69846, 73999, 78399, 83061, 88000, 93233, 98777,
        /*84 */ 104650, 110873, 117466, 124451, 131851, 139691, 147998, 156798, 166122, 176000, 186466, 197553,
        /*96 */ 209300, 221746, 234932, 248902, 263702, 279383, 295996, 313596, 332244, 352000, 372931, 395107,
        /*108*/ 418601, 443492, 469863, 497803, 527404, 558765, 591991, 627193, 664488, 704000, 745862, 790213,
        /*120*/ 837202, 886984, 939727, 995606, 1054808, 1117530, 1183982, 1254385
    }
};
