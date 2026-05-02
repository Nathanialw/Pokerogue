//
// Created by nathanial on 2/26/26.
//
#pragma once
#include <stdbool.h>
#include <data_constants.inc>
#include "lib_types.h"

/**********************************************************************************************************************/
/*
 *  This file holds the global constants values
 *  If only used in one file do try to move them to that file
 *
**********************************************************************************************************************/

// compile options
#define INCLUDE_BATTLERS

/**********************************************************************************************************************/
/** Function macros
**********************************************************************************************************************/
#define ASSERT_STRING_FITS(ARRAY, STRING) _Static_assert(sizeof(STRING) <= ALIGNED_SIZE, "String " #STRING " too long for " #ARRAY)
#define ASSERT_ALIGNED8(N) _Static_assert(((N) % 8) == 0, "ALIGNED_SIZE must be divisible by " #N)
#define ASSERT_ALIGNED16(N) _Static_assert(((N) % 16) == 0, "ALIGNED_SIZE must be divisible by " #N)
#define ASSERT_ALIGNED32(N) _Static_assert(((N) % 32) == 0, "ALIGNED_SIZE must be divisible by " #N)
#define GROWTH_PACK(a,d,m,s)  ( ((a)&0xF)<<12 | ((d)&0xF)<<8 | ((m)&0xF)<<4 | ((s)&0xF) )

/**********************************************************************************************************************/
/** ENUMS constants
**********************************************************************************************************************/
#define TYPE_COUNT 16
#define THEME_COUNT 16
// #define BIOME_COUNT 16
#define MAX_LEVELS 35


_Static_assert(ITEM_COUNT <= 256, "max must be 256 - one byte");
_Static_assert(ABILITY_COUNT <= 256, "max must be 256 - one byte");
_Static_assert(CREATURE_COUNT <= 256, "TOTAL MONSTERS MUST BE 256");
_Static_assert(SPELL_COUNT <= 256, "max must be 256 - one byte");
// _Static_assert(OBJECT_COUNT <= 256, "max must be 256 - one byte");

#define PALETTE_COUNT 64
#define CHARACTER_COUNT 256

/**********************************************************************************************************************/
/** STRING ARRAYS constants
 *  multiples of 16 for pico cache
**********************************************************************************************************************/
#define SMALL_STRINGS 16   // Cache line size - perfect for names/menu items
#define LARGE_STRINGS 128  // Plenty for descriptions (fits 1-2 sentences)


/**********************************************************************************************************************/
/**PLAYER constants
**********************************************************************************************************************/
#define MAX_PARTY_SIZE 6
#define MAX_SPELLBOOK_SIZE 8
#define DEFAULT_SPELLBOOK_SIZE 8
#define DEFAULT_BAG_SIZE 10
#define MAX_ITEM_SIZE 25
#define MAX_SPELL_SIZE 16    // spells not implemented yet, still a maybe
#define PERK_BAG_SIZE 0     // perks not implemented yet, still a maybe
#define MAX_BAG_SIZE (DEFAULT_BAG_SIZE + MAX_ITEM_SIZE + MAX_SPELL_SIZE + PERK_BAG_SIZE)


/**********************************************************************************************************************/
/**CAMERA constants
**********************************************************************************************************************/
#define CAM_OFFSET_X  (((VIEW_TW / 2) - 1))
#define CAM_OFFSET_Y  ((VIEW_TH / 2))
#define VIEW_TW 20
#define VIEW_TH 15
#define SCREEN_W 320
#define SCREEN_H 240

/**********************************************************************************************************************/
/** ENTITY constants
**********************************************************************************************************************/
#define ENTITY_COUNT 255
#define NO_ENTITY 255
#define SPR_W 16
#define SPR_H 16
#define MAX_ABILITIES 6
#define EMPTY_ID 0

/**********************************************************************************************************************/
/**BATTLE FRAMES constants
**********************************************************************************************************************/
#define BATTLER_AREA_H 112
#define BATTLER_AREA_W 160
#define BATTLER_TILES_W 7
#define BATTLER_TILES_H 7
#define BATTLER_OFFSET 24

#define RESOURCE_FRAME_W 160
#define RESOURCE_FRAME_H  64
#define RESOURCE_WIDTH (RESOURCE_FRAME_W-16)
#define RESOURCE_HEIGHT TEXT_W

#define DIALOGUE_H  64
_Static_assert(DIALOGUE_H + RESOURCE_FRAME_H + BATTLER_AREA_H <= 240, "cannot exceed screen height of 240");

#define PLAYER_BATTLER_FRAME    ((Rect_16){0, SCREEN_H-BATTLER_AREA_H-DIALOGUE_H, BATTLER_AREA_W, BATTLER_AREA_H})
#define ENEMY_BATTLER_FRAME     ((Rect_16){SCREEN_W-BATTLER_AREA_W, 0, BATTLER_AREA_W, BATTLER_AREA_H})
#define ENEMY_RESOURCE_FRAME    ((Rect_16){0, 0, RESOURCE_FRAME_W, RESOURCE_FRAME_H})
#define PLAYER_RESOURCE_FRAME   ((Rect_16){SCREEN_W-RESOURCE_FRAME_W, SCREEN_H-RESOURCE_FRAME_H-DIALOGUE_H, RESOURCE_FRAME_W, RESOURCE_FRAME_H})
#define DIALOGUE_BOX_FRAME      ((Rect_16){0, SCREEN_H-DIALOGUE_H, SCREEN_W, DIALOGUE_H})

/**********************************************************************************************************************/
/**MAP constants
**********************************************************************************************************************/
#define MAP_W 256
#define MAP_H 240

#define NUM_BIOME_CREATURES 64
#define NUM_THEME_CREATURES 64
#define BIOME_MONSTER_TYPES 8
#define THEME_MONSTER_TYPES 7
#define MONSTER_TYPES (BIOME_MONSTER_TYPES + THEME_MONSTER_TYPES + 1)// + one for player sprite
#define MAX_MAP_ITEM_TYPES 16
#define NUM_TILES 16
#define TILE_PIXELS (16*16)
#define TILE_W 16
#define TILE_H 16
#define TEXT_W 8
#define TEXT_H 8

/**********************************************************************************************************************/
/**MENUS constants
**********************************************************************************************************************/
#define FONT_OFFSET 32

#define SCREEN_TILE_W 20
#define SCREEN_TILE_H 15

#define MAIN_MENU_X 11
#define MAIN_MENU_Y 0
#define MAIN_MENU_W (SCREEN_TILE_W-11)
#define MAIN_MENU_H 15

#define BATTLE_MENU_X 0
#define BATTLE_MENU_Y 22
#define BATTLE_MENU_W 15
#define BATTLE_MENU_H 8

#define GLYPH_ROWS_8x8   8
#define MAX_MENU_SIZE (MAIN_MENU_H * 2)
#define MAX_MENU_DEPTH 3
#define BATTLE_MENU_SIZE 5

/**********************************************************************************************************************/
/** GRAPHICS constants
**********************************************************************************************************************/
// #define BUFFER_H (16)
// #define BUFFER_W (392)
// #define BUFFER_SIZE (BUFFER_W * BUFFER_H)

/**********************************************************************************************************************/
/**STAT MODIFIERS cosntants
**********************************************************************************************************************/
#define HASTED_PERCENT 25
#define SLOWED_PERCENT 25

/**********************************************************************************************************************/
/**SOUNDS constants
**********************************************************************************************************************/
#define GENERATED_MELODY_LENGTH 64
