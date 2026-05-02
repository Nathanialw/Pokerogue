//
// Created by nathanial on 2/22/26.
//
#pragma once
#include "stdint.h"
#include <stdbool.h>
#include "constants.h"
#include "enums.h"

/**********************************************************************************************************************
*
/**********************************************************************************************************************/

typedef uint8_t EntityId;
typedef uint16_t Pixel;
typedef uint8_t CreatureID;
typedef uint8_t SpellId;
typedef uint16_t TileSet[NUM_TILES * TILE_PIXELS];
typedef uint16_t Glyph16x96s[49];
typedef uint16_t Glyph16x96[(16 * 96)];
typedef char StatusLine[10];
typedef Ability Attacks[8];
typedef char SmallStringArray[SMALL_STRINGS];

/**********************************************************************************************************************/
/**Bitfield types
**********************************************************************************************************************/
/**********************************************************************************************************************/
/** assumes 255 entities
**********************************************************************************************************************/
typedef uint8_t BitFieldUint8[32];
typedef uint16_t BitFieldUint16[16];
typedef uint32_t BitFieldUint32[8];

/**********************************************************************************************************************
*   Creature skills
**********************************************************************************************************************/
typedef struct
{
    uint8_t skillID;
    uint8_t level;
} SkillLearnLevel;

typedef SkillLearnLevel CreatureSkillLearnLevels[16];

/**********************************************************************************************************************
*   MENUS
**********************************************************************************************************************/
typedef bool (*SubMenu)(bool update);
typedef uint16_t MainMenuPtr[MAIN_MENU_W];


/**********************************************************************************************************************/
/**  Stores an array of 64 uin16_t pixels
*   can be accessed as a linear array or as a 2d array 8x8
**********************************************************************************************************************/
typedef union
{
    uint16_t pixels[TILE_PIXELS / 4];
    uint16_t pixels_2d[TILE_W / 4][TILE_H / 4];
} Glyph8x8;

_Static_assert(sizeof(Glyph8x8) == 128, "Glyph8x8 must be 128 bytes");

/**********************************************************************************************************************/
/**  Stores an array of 256 uin16_t pixels
*   can be accessed as a linear array or as a 2d array 16x16
**********************************************************************************************************************/
typedef union
{
    uint16_t pixels[TILE_PIXELS];
    uint16_t pixels_2d[TILE_W][TILE_H];
} Glyph16x16;

_Static_assert(sizeof(Glyph16x16) == 512, "Glyph16x16 must be 512 bytes");


/**********************************************************************************************************************/
/**  stores entity action data imported from the DB
**********************************************************************************************************************/
typedef struct
{
    uint8_t power;
    uint8_t pp;
    uint8_t type : 4;
    //TODO: bits unused
    uint8_t _pad : 4;
} SpellData;

_Static_assert(sizeof(SpellData) == 3, "SpellData must be 3 bytes");

typedef struct
{
    uint8_t power;
    uint8_t manaCost;
    uint8_t type : 4;
    //TODO: bits unused
    uint8_t _pad : 4;
    //hit chance?
    //crit chance?
} SkillData;

_Static_assert(sizeof(SkillData) == 3, "SkillData must be 3 bytes");

typedef struct
{
    union
    {
        uint8_t chance;
        uint8_t power;
    };

    uint8_t type : 4;
    uint8_t level : 4;
} ItemData;

typedef struct
{
    union
    {
        uint8_t chance;
        uint8_t power;
    };

    uint8_t type : 4;
    uint8_t level : 4;
} ObjectData;

_Static_assert(sizeof(ItemData) == 2, "ItemData must be 2 bytes");


typedef bool (*SkillEffect)(EntityId attackerID, EntityId defenderID, SkillData skillData);
typedef bool (*ItemEffect)(EntityId item_id, EntityId e_id, ItemData itemData);
typedef bool (*SpellEffect)(EntityId partyID, EntityId enemyID, SpellData spellData);
typedef bool (*ObjectEffect)(EntityId partyID, EntityId enemyID, ObjectData spellData);


/**********************************************************************************************************************/
/**Store 2 integers each max 255
**********************************************************************************************************************/
typedef struct
{
    uint8_t x;
    uint8_t y;
} Position;

_Static_assert(sizeof(Position) == 2, "ObjectType must be 2 bytes");

/**********************************************************************************************************************/
/**Stores the type id of a game object
**********************************************************************************************************************/
typedef union
{
    uint8_t unused;
    uint8_t CreatureID;
    uint8_t SpellId;
    uint8_t AbilityId;
    uint8_t ItemId;
    uint8_t value;
} ObjectType;

_Static_assert(sizeof(ObjectType) == 1, "ObjectType must be 1 byte");

/**********************************************************************************************************************/
/**  Stores 2 integers each with the max value of 999
*   Used for resources where current/max and max <= 999
*   4 additional bits for flags
**********************************************************************************************************************/
typedef struct
{
    uint8_t current_low; // Lower 8 bits of current HP
    uint8_t max_low; // Lower 8 bits of max HP
    uint8_t current_high : 2; // [curr_hi:2][max_hi:2][flags:4]
    uint8_t max_high : 2; // [curr_hi:2][max_hi:2][flags:4]
    //TODO: bits unused
    uint8_t flags : 4; // Raw 4 bits
} IntMax999;

_Static_assert(sizeof(IntMax999) == 3, "IntMax999 must be 3 bytes");

/**********************************************************************************************************************/
/**  Stores 2 integers each with the max value of 99
*   Used for resources where current/max and max <= 99
*   2 additional bits for flags
**********************************************************************************************************************/
typedef struct
{
    uint16_t current : 7; // max 128
    uint16_t max : 7; // max 128
    //TODO: bits unused
    bool negativeEffect : 1; // 1 bits for flag
    bool positiveEffect : 1; // 1 bits for flag
} IntMax99;

_Static_assert(sizeof(IntMax99) == 2, "IntMax99 must be 2 bytes");

/**********************************************************************************************************************/
/**  the first 4 bits hold the value < 16
*   the remaining 4 bits hold flags
*   you can have a max 16 value while using the remaining bit as flags
**********************************************************************************************************************/
typedef struct
{
    uint8_t current : 4; // max 15
    uint8_t greater : 4;
} Int16;

_Static_assert(sizeof(Int16) == 1, "Int16 must be 1 byte");

/**********************************************************************************************************************/
/**  the first 5 bits hold the value < 32
*   the remaining 3 bits hold flags
*   you can have a max 32 value while using the remaining bit as flags
**********************************************************************************************************************/
typedef struct
{
    uint8_t current : 5; // max 31
    uint8_t greater : 3;
} Int32;

_Static_assert(sizeof(Int32) == 1, "Int32 must be 1 byte");

/**********************************************************************************************************************/
/**  the first 6 bits hold the value < 64
*   the remaining 2 bits hold flags
*   max 64 value while using the remaining bit as flags
**********************************************************************************************************************/
typedef struct
{
    uint8_t current : 6; // max 63
    uint8_t greater : 2;
} Int64;

_Static_assert(sizeof(Int64) == 1, "Int64 must be 1 byte");

/**********************************************************************************************************************/
/**  the top 7 bits hold the value < 128 - meant for max 99 applications
*   the bottom bit holds a flag
*   max 99 value while using the remaining bit as a flag
**********************************************************************************************************************/
typedef struct
{
    uint8_t value : 7; // max 127
    uint8_t greater : 1;
} Int99;

_Static_assert(sizeof(Int99) == 1, "Int99 must be 1 byte");

/**********************************************************************************************************************/
/** holds the metadata of each object in the sprite arrays
**********************************************************************************************************************/
typedef struct
{
    uint32_t idx;
    uint16_t palette[16];
    uint8_t emptyIndexes[7];
} SpriteLayout;

_Static_assert(sizeof(SpriteLayout) == 44, "SpriteLayout must be 44 bytes");

/**********************************************************************************************************************/
/** holds 2 2-byte unsigned integers
**********************************************************************************************************************/
typedef struct
{
    uint16_t x;
    uint16_t y;
} Vec_16;

_Static_assert(sizeof(Vec_16) == 4, "Vec_16 must be 4 bytes");
/**********************************************************************************************************************/
/** holds 2 1-byte unsigned integers
**********************************************************************************************************************/
typedef struct
{
    uint8_t x;
    uint8_t y;
} Vec_8;

_Static_assert(sizeof(Vec_8) == 2, "Vec_8 must be 2 bytes");

/**********************************************************************************************************************/
/** holds 2 1-byte signed integers
**********************************************************************************************************************/
typedef struct
{
    int8_t x;
    int8_t y;
} Vec_8_Signed;

_Static_assert(sizeof(Vec_8_Signed) == 2, "Vec_8_Signed must be 2 bytes");

/**********************************************************************************************************************/
/** defines a rectangle each 1-byte value max 255
**********************************************************************************************************************/
typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} Rect_8;

_Static_assert(sizeof(Rect_8) == 4, "Rect_8 must be 4 bytes");

/**********************************************************************************************************************/
/** 16 types of tiles, each have a move limiting effect
/** ie no_effect, wall, water, lava, acid, etc
**********************************************************************************************************************/
typedef struct
{
    uint8_t u : 4;
    uint8_t d : 4;
    uint8_t r : 4;
    uint8_t l : 4;
} Node;

_Static_assert(sizeof(Node) == 2, "Node must be 2 bytes");


/**********************************************************************************************************************/
/** This camera struct assumes the max size of the map is 255 x 255
*  defines a rectangle
*       position values max <65536
*       position values max <256
**********************************************************************************************************************/
typedef struct Camera
{
    uint8_t x;
    uint8_t y;
    uint16_t w;
    uint16_t h;
} Camera;

_Static_assert(sizeof(Camera) == 6, "Camera must be 6 bytes");


/**********************************************************************************************************************/
/** Detection values for creatures packed into 1 byte
 *  Separates out sounds, sight and smell into discrete values
 *      sight value max <8
 *      sound value max <8
 *      smell value max <4
**********************************************************************************************************************/
typedef struct Senses
{
    uint8_t sight : 3;
    uint8_t sound : 3;
    uint8_t smell : 2;
} Senses;

_Static_assert(sizeof(Senses) == 1, "Senses must be 1 byte");

/**********************************************************************************************************************/
/** Generated stat values for each creature
 *  Grows with levels and can be modified by items/spells/skill
 *  each value is 1 byte - max <256
**********************************************************************************************************************/
typedef struct Stats
{
    uint8_t attack;
    uint8_t defence;
    uint8_t speed;
    uint8_t magic;
} Stats;

_Static_assert(sizeof(Stats) == 4, "Stats must be 4 bytes");

/**********************************************************************************************************************/
/** Min and max values of stats
/** Growth per level data for a creature
**********************************************************************************************************************/
typedef struct StatsRange
{
    Stats min;
    Stats max;
    uint16_t growth;
} StatsRange;

_Static_assert(sizeof(StatsRange) == 10, "StatsRange must be 10 bytes");

/**********************************************************************************************************************/
/** stores the 2 type values each monster has
 *  assumes 16 typ values
 *  each value uses 4 bits
**********************************************************************************************************************/
typedef struct MonsterType
{
    uint8_t typeA : 4;
    uint8_t typeB : 4;
} MonsterType;

_Static_assert(sizeof(MonsterType) == 1, "MonsterType must be 1 byte");


/**********************************************************************************************************************/
/** index into the font array to retrieve the glyph data
 *  color value of the glyph
**********************************************************************************************************************/
typedef struct
{
    uint8_t glyph_index;
    uint8_t fg : 6;
    //TODO: bits unused - possibly palette data
    uint8_t _pad : 2; //future use
} Sprite;

_Static_assert(sizeof(Sprite) == 2, "Sprite must be 2 bytes");


/**********************************************************************************************************************/
/** index into the font array to retrieve the glyph data
 *  color value of the background
 *  color value of the glyph
**********************************************************************************************************************/
typedef struct
{
    uint8_t glyph_index;
    uint16_t bg : 6;
    uint16_t fg : 6;
    //TODO: bits unused - possibly palette data
    uint16_t _pad : 4;
} __attribute__((packed)) Tile;

_Static_assert(sizeof(Tile) == 3, "Sprite must be 3 bytes");


typedef bool (*Battle_Animation)(bool onAttacker);
