//
// Created by nathanial on 2/26/26.
//
#pragma once
#include "types.h"
#include "enums.h"
#include "data_constants.inc"

#define TOTAL_MUSIC_NOTES 815
#define MUSIC_COUNT 1
#define TRAINER_COUNT 255

/**********************************************************************************************************************
*
*   ROM layout struct
*
**********************************************************************************************************************/

typedef struct
{
    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    struct
    {
        const Sprite items[ITEM_COUNT];
        const Sprite monsters[CREATURE_COUNT];
        const Sprite objects[CREATURE_COUNT];
        const Tile biomes[BIOME_COUNT][NUM_TILES];
    } sprites;

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    struct
    {
        const Creature monsterGroups[BIOME_COUNT][BIOME_MONSTER_TYPES];
        const Creature themeGroups[BIOME_COUNT][THEME_MONSTER_TYPES];
    } tileset;

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    struct
    {
        struct
        {
            const char main[MAIN_MENUS_SIZE][SMALL_STRINGS];
            const char options[OPTIONS_MENU_SIZE][SMALL_STRINGS];
            const char battleMenu[BATTLE_MENU_SIZE][SMALL_STRINGS];
        } menus;

        struct
        {
            const SmallStringArray items[ITEM_TEXT_ARRAY_SIZE];
            const SmallStringArray monsters[CREATURE_TEXT_ARRAY_SIZE];
            const SmallStringArray spells[SPELL_TEXT_ARRAY_SIZE];
            const SmallStringArray attacks[ABILITY_TEXT_ARRAY_SIZE];
            const SmallStringArray objects[OBJECT_TEXT_ARRAY_SIZE];
            const SmallStringArray trainers[OBJECT_TEXT_ARRAY_SIZE];

        } names;

        struct
        {
            const char items[ITEM_TEXT_ARRAY_SIZE][LARGE_STRINGS];
            const char monsters[CREATURE_TEXT_ARRAY_SIZE][LARGE_STRINGS];
            const char spells[SPELL_TEXT_ARRAY_SIZE][LARGE_STRINGS];
            const char attacks[ABILITY_TEXT_ARRAY_SIZE][LARGE_STRINGS];
            const char objects[OBJECT_TEXT_ARRAY_SIZE][LARGE_STRINGS];
            const char trainers[OBJECT_TEXT_ARRAY_SIZE][LARGE_STRINGS];
        } descriptions;

        const char types[TYPE_COUNT][SMALL_STRINGS];
    } text;

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    struct
    {
#ifdef INCLUDE_BATTLERS //quite large, keep disabled while iterating
        struct
        {
            const SpriteLayout frontLayout[CREATURE_COUNT];
            const SpriteLayout backLayout[CREATURE_COUNT];
            const uint8_t front[SPRITE_CREATURE_FRONT_BYTES];
            const uint8_t back[SPRITE_CREATURE_BACK_BYTES];
        } battlers;

        const SpriteLayout itemLayout[ITEM_COUNT];
        const uint8_t items[SPRITE_ITEM_BYTES];

        const SpriteLayout skillLayout[ABILITY_COUNT];
        const uint8_t skills[SPRITE_SKILL_BYTES];

        const SpriteLayout spellLayout[SPELL_COUNT];
        const uint8_t spells[SPRITE_SPELL_BYTES];

        const SpriteLayout objectLayout[OBJECT_COUNT];
        const uint8_t objects[SPRITE_OBJECT_BYTES];

        const SpriteLayout trainerLayout[TRAINER_COUNT];
        const uint8_t trainers[TRAINER_COUNT];

        const uint8_t font8x8[CHARACTER_COUNT * 8];
        const uint16_t font16x16[CHARACTER_COUNT * 32];
#endif
    } spriteData;

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    struct
    {
        struct
        {
            const MusicData musicData[MUSIC_COUNT]; // maybe one piece for each biome, when we how much space we are looking at
            const Note songs[TOTAL_MUSIC_NOTES];
        } music;

        struct
        {
            const Note creatures[CREATURE_COUNT]; //probably not Notes, we will see though
            const Note spells[SPELL_COUNT]; //probably not Notes, we will see though
            const Note attacks[ABILITY_COUNT]; //probably not Notes, we will see though
            const Note menus[10];
        } effects;
    } sounds;

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    struct
    {
        const int8_t typeEffects[TYPE_COUNT * TYPE_COUNT];
        const MonsterType creatureTypes[CREATURE_COUNT];
        const StatsRange creatureStats[CREATURE_COUNT];
        const BitFieldUint8 learnableSkills[CREATURE_COUNT];
        const CreatureSkillLearnLevels levelUpSkills[CREATURE_COUNT];

        const SkillData abilityData[ABILITY_COUNT];
        const SpellData spellData[SPELL_COUNT];
        const ItemData itemData[ITEM_COUNT];
        const ObjectData objectData[OBJECT_COUNT];
    } gameData;


    struct
    {
        const Battle_Animation itemsAttack[ITEM_COUNT];
        const Battle_Animation itemsStruck[ITEM_COUNT];
        const Battle_Animation spellsAttack[SPELL_COUNT];
        const Battle_Animation spellsStruck[SPELL_COUNT];
        const Battle_Animation skillsAttack[ABILITY_COUNT];
        const Battle_Animation skillsStruck[ABILITY_COUNT];
    } animation;

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    struct
    {
        const SkillEffect abilityFunctions[ABILITY_COUNT];
        const SpellEffect spellFunctions[SPELL_COUNT];
        const ItemEffect itemFunctions[ITEM_COUNT];
        const ObjectEffect objectFunctions[OBJECT_COUNT];
    } funcs;

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    const uint16_t GetColor[PALETTE_COUNT];
    const uint16_t GetColor256[256];

    /**********************************************************************************************************************/
    /*
    **********************************************************************************************************************/
    const int32_t noteFreqFixed[128];
} GameFlash;


extern const GameFlash g_gameFlash;


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
ASSERT_ALIGNED16(SPRITE_CREATURE_FRONT_BYTES);
ASSERT_ALIGNED16(SPRITE_CREATURE_BACK_BYTES);
ASSERT_ALIGNED16(SMALL_STRINGS);
ASSERT_ALIGNED16(LARGE_STRINGS);
_Static_assert(ITEM_COUNT <= 256, "MAX_ITEMS must Be less than or equal to 256");
_Static_assert(OBJECT_COUNT <= 256, "MAX_ITEMS must Be less than or equal to 256");
_Static_assert(CREATURE_COUNT <= 256, "CREATURE_COUNT must be less than or equal to 256");
_Static_assert(SPELL_COUNT <= 256, "TOTAL_SPELLS must be less than or equal to 256");
_Static_assert(ABILITY_COUNT <= 256, "TOTAL_ATTACKS must be less than or equal to 256");
_Static_assert(ENTITY_COUNT <= 256, "TOTAL_ATTACKS must be less than or equal to 256");
