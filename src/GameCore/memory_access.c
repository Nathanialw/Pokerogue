//
// Created by nathanial on 5/15/26.
//

#include "memory_access.h"

#include <stdlib.h>

#include "lib_decl.h"
#include "lib_debugging.h"

#include "memory_ram.h"
#include "memory_constants.inc"

#include "enums.h"
#include "memory_rom.h"
#include "types.h"

// #define MEMORY_PRINT

#if defined(MEMORY_PRINT)
SET_MEMORY(".core.data")
static const char str_spawn_creature_type[] = "0x%2x,";

SET_MEMORY(".core.data")
static const char begin[] = "begin\n";

SET_MEMORY(".core.data")
static const char new_line[] = "\n";
#endif


/**********************************************************************************************************************/
/*      ENTITIES
**********************************************************************************************************************/
SET_MEMORY(".map")
SkillLearnLevel Flash_GetSkill(MemoryInterface memory, CreatureSkillLearnLevels c, Type creatureType, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.levelUpSkills[creatureType][index];
#else
    const uint8_t length = sizeof(SkillLearnLevel);
    memory.GetRom(GAME_DATA_LEVEL_UP_SKILLS_POSITION + (length * index), c.bytes, length);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, c.bytes[i]);
    memory.Print(new_line);
#endif
    return c.c[index];
#endif
}


SET_MEMORY(".map")
uint8_t Flash_GetStatGrowth(MemoryInterface memory, Creature type)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.creatureStats[type].growth;
#else
    const uint8_t length = 1;
    char struct_bytes[length];
    memory.GetRom(GAME_DATA_CREATURE_STATS_POSITION + type, struct_bytes, length);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, struct_bytes[i]);
    memory.Print(new_line);
    return struct_bytes[0];
#endif
#endif
}

SET_MEMORY(".map")
Creature Flash_GetBiomeCreature(MemoryInterface memory, uint8_t biome, Creature index)
{
#ifdef STANDALONE
    return g_gameFlash.tileset.monsterGroups[biome][index];
#else
    const uint8_t length = 1;
    char struct_bytes[length];
    memory.GetRom(CHAR_SPRITES_BIOMES_POSITION + index, struct_bytes, length);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, struct_bytes[i]);
    memory.Print(new_line);
    return struct_bytes[0];
#endif
#endif
}

SET_MEMORY(".map")
Creature Flash_GetThemeCreature(MemoryInterface memory, uint8_t theme, Creature index)
{
#ifdef STANDALONE
    return g_gameFlash.tileset.themeGroups[theme][creature_type];
#else
    const uint8_t length = 1;
    char struct_bytes[length];
    memory.GetRom(TILESET_THEME_POSITION + index, struct_bytes, length);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, struct_bytes[i]);
    memory.Print(new_line);
    return struct_bytes[0];
#endif
#endif
}


SET_MEMORY(".core")
void Flash_GetCreatureStatsRange(MemoryInterface memory, StatsRange* stats, Creature creature_type)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.creatureStats[creature_type];
#else
    const uint8_t length = 10;
    memory.GetRom(TILESET_THEME_POSITION + (creature_type * length), stats->bytes, length);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, stats->bytes[i]);
    memory.Print(new_line);
#endif
#endif
}


SET_MEMORY(".core")
MonsterType Flash_GetType(MemoryInterface memory, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.creatureTypes[index];
#else

    uint8_t bytes[1] = {0};
    memory.GetRom(STRINGS_TYPES_POSITION + index, bytes, 1);
    MonsterType mt = {.typeA = bytes[0] << 4, .typeB = bytes[0]};

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < 1; i++)
        memory.Print(str_spawn_creature_type, mt.typeA);
    memory.Print(new_line);
    return mt;
#endif
#endif
}

SET_MEMORY(".core")
int8_t Flash_GetTypeEffects(MemoryInterface memory, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.typeEffects[index];
#else
    uint8_t bytes[1] = {0};
    memory.GetRom(TILESET_THEME_POSITION + index, bytes, 1);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < 1; i++)
        memory.Print(str_spawn_creature_type, bytes[0]);
    memory.Print(new_line);
    return bytes[0];
#endif
#endif
}

SET_MEMORY(".core")
SkillData Flash_GetSkillData(MemoryInterface memory, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.abilityData[index];
#else
    SkillData skillData = {0};
    memory.GetRom(GAME_DATA_ABILITY_POSITION + index, skillData.bytes, sizeof(SkillData));

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < sizeof(SkillData); i++)
        memory.Print(str_spawn_creature_type, skillData.bytes[i]);
    memory.Print(new_line);
    return skillData;
#endif
#endif
}

SET_MEMORY(".core")
SpellData Flash_GetSpellData(MemoryInterface memory, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.spellData[index];
#else
    SpellData spell_data = {0};

#if defined(MEMORY_PRINT)
    memory.GetRom(GAME_DATA_SPELL_POSITION + index, spell_data.bytes, sizeof(SpellData));
    for (uint8_t i = 0; i < sizeof(SpellData); i++)
        memory.Print(str_spawn_creature_type, spell_data.bytes[i]);
    memory.Print(new_line);
    return spell_data;
#endif
#endif
}

SET_MEMORY(".core")
ItemData Flash_GetItemData(MemoryInterface memory, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.itemData[index];
#else
    ItemData item_data = {0};

#if defined(MEMORY_PRINT)
    memory.GetRom(GAME_DATA_ITEM_POSITION + index, item_data.bytes, sizeof(ItemData));
    for (uint8_t i = 0; i < sizeof(ItemData); i++)
        memory.Print(str_spawn_creature_type, item_data.bytes[i]);
    memory.Print(new_line);
    return item_data;
#endif
#endif
}

SET_MEMORY(".core")
ObjectData Flash_GetObjectData(MemoryInterface memory, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.gameData.objectData[index];
#else
    ObjectData object_data = {0};
    memory.GetRom(GAME_DATA_OBJECT_POSITION + index, object_data.bytes, sizeof(ObjectData));

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < sizeof(Tile); i++)
        memory.Print(str_spawn_creature_type);
    memory.Print(new_line);
    return object_data;
#endif
#endif
}

/**********************************************************************************************************************/
/**     MAP
**********************************************************************************************************************/
SET_MEMORY(".map")
void Flash_GetBiomeTile(MemoryInterface memory, Tile* tile, uint8_t biomeType, uint8_t tile_id)
{
#ifdef STANDALONE
    return g_gameFlash.sprites.biomes[biomeType][tile_id];
#else
    memory.GetRom(CHAR_SPRITES_BIOMES_POSITION + (g_core.biome * (NUM_TILES * sizeof(Tile))) + (tile_id * sizeof(Tile)), tile->bytes, sizeof(Tile));

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < sizeof(Tile); i++)
        memory.Print(str_spawn_creature_type, tile->bytes[i]);
    memory.Print(new_line);
#endif
#endif
}


/**********************************************************************************************************************/
/*      SPRITES
**********************************************************************************************************************/
//TODO:
SET_MEMORY(".map")
void Flash_GetSpriteMetadata(MemoryInterface memory, Sprite* sprite, ObjectsTypes type, uint8_t index)
{
#ifdef STANDALONE
    if (type == ITEM)
    {
        return g_gameFlash.sprites.items[type];
    }
    else if (type == CREATURE)
    {
        return g_gameFlash.sprites.monsters[type];
    }
    else if (type == OBJECT)
    {
        return g_gameFlash.sprites.objects[type];
    }
#else
    sprite->glyph_index = 33;
    sprite->fg = 10;
#endif
}

SET_MEMORY(".core")
void Flash_GetSpriteLayout(MemoryInterface memory, SpriteLayout spriteLayout, uint8_t index, ObjectsTypes type, bool front)
{
#ifdef STANDALONE
    if (type == ITEM)
        return &g_gameFlash.spriteData.itemLayout[index];
    else if (type == OBJECT)
        return &g_gameFlash.spriteData.objectLayout[index];
    else if (type == SPELL)
        return &g_gameFlash.spriteData.spellLayout[index];
    else if (type == SKILL)
        return &g_gameFlash.spriteData.skillLayout[index];
#else

    if (type == ITEM)
        memory.GetRom(SPRITE_ITEMS_LAYOUT_POSITION, spriteLayout.bytes, sizeof(SpriteLayout));
    else if (type == OBJECT)
        memory.GetRom(SPRITE_OBJECTS_LAYOUT_POSITION, spriteLayout.bytes, sizeof(SpriteLayout));
    else if (type == SPELL)
        memory.GetRom(SPRITE_SPELLS_LAYOUT_POSITION, spriteLayout.bytes, sizeof(SpriteLayout));
    else if (type == SKILL)
        memory.GetRom(SPRITE_SKILLS_LAYOUT_POSITION, spriteLayout.bytes, sizeof(SpriteLayout));
    else if (type == CREATURE)
    {
        if (front)
            memory.GetRom(SPRITE_BATTLER_LAYOUT_FRONT_POSITION, spriteLayout.bytes, sizeof(SpriteLayout));
        else
            memory.GetRom(SPRITE_BATTLER_LAYOUT_BACK_POSITION, spriteLayout.bytes, sizeof(SpriteLayout));
    }

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < sizeof(SpriteLayout); i++)
        memory.Print(str_spawn_creature_type, spriteLayout.bytes[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetSprite(MemoryInterface memory, uint8_t* sprite, uint16_t length, ObjectsTypes type, bool front)
{
#ifdef STANDALONE
    if (type == ITEM)
        return g_gameFlash.spriteData.itemSprites;
    else if (type == OBJECT)
        return g_gameFlash.spriteData.objectSprites;
    else if (type == SPELL)
        return g_gameFlash.spriteData.spells;
    else if (type == SKILL)
        return g_gameFlash.spriteData.skills;
#else
    if (type == ITEM)
        memory.GetRom(SPRITE_ITEMS_POSITION, sprite, length);
    else if (type == OBJECT)
        memory.GetRom(SPRITE_OBJECTS_POSITION, sprite, length);
    else if (type == SPELL)
        memory.GetRom(SPRITE_SPELLS_POSITION, sprite, length);
    else if (type == SKILL)
        memory.GetRom(SPRITE_SKILLS_POSITION, sprite, length);
    else if (type == CREATURE)
    {
        if (front)
            memory.GetRom(SPRITE_BATTLER_FRONT_POSITION, sprite, length);
        else
            memory.GetRom(SPRITE_BATTLER_BACK_POSITION, sprite, length);
    }


#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, sprite[i]);
    memory.Print(new_line);
#endif
#endif
}

//TODO:
SET_MEMORY(".battle")
SpriteLayout Flash_GetBattlerLayout(MemoryInterface memory, uint8_t creatureType, bool front)
{
#ifdef STANDALONE
    if (front)
        return g_gameFlash.spriteData.battlers.frontLayout[creatureType];
    else
        return g_gameFlash.spriteData.battlers.backLayout[creatureType];
#else
    SpriteLayout l = {0};
    return l;
#endif
}

//TODO:
SET_MEMORY(".battle")
const uint8_t* Flash_GetBattlerArray(MemoryInterface memory, bool front)
{
#ifdef STANDALONE
    if (front)
        return g_gameFlash.spriteData.battlers.front;
    else
        return g_gameFlash.spriteData.battlers.back;
#else
    return NULL;
#endif
}


/**********************************************************************************************************************/
/*      ANIMATIONS
**********************************************************************************************************************/
// TODO:
SET_MEMORY(".battle")
void Flash_BattlerAttackAnimation(MemoryInterface memory, EntityId id, ObjectsTypes type, bool player)
{
#ifdef STANDALONE
    if (type == ITEM)
        g_gameFlash.animation.itemsAttack[id](player);
    else if (type == SKILL)
        g_gameFlash.animation.skillsAttack[id](player);
    else if (type == SPELL)
        g_gameFlash.animation.spellsAttack[id](player);
#else
    if (type == ITEM)
        itemsAttack[id](player);
    else if (type == SKILL)
        skillsAttack[id](player);
    else if (type == SPELL)
        spellsAttack[id](player);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < sizeof(ObjectsTypes); i++)
        memory.Print(str_spawn_creature_type);
    memory.Print(new_line);
#endif
#endif
}

// TODO:
SET_MEMORY(".battle")
void Flash_BattlerStruckAnimation(MemoryInterface memory, EntityId id, ObjectsTypes type, bool player)
{
#ifdef STANDALONE
    if (type == ITEM)
        g_gameFlash.animation.itemsStruck[id](player);
    else if (type == SKILL)
        g_gameFlash.animation.skillsStruck[id](player);
    else if (type == SPELL)
        g_gameFlash.animation.spellsStruck[id](player);
#else
    if (type == ITEM)
        itemsStruck[id](player);
    else if (type == SKILL)
        skillsStruck[id](player);
    else if (type == SPELL)
        spellsStruck[id](player);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < sizeof(ObjectsTypes); i++)
        memory.Print(str_spawn_creature_type);
    memory.Print(new_line);
#endif
#endif
}


/**********************************************************************************************************************/
/*      TEXT
**********************************************************************************************************************/
SET_MEMORY(".core")
void Flash_GetFontChar8x8(MemoryInterface memory, uint8_t* glyph, uint8_t index)
{
#ifdef STANDALONE
    for (uint16_t i = 0; i < 8; i++)
    {
        glyph[i] = g_gameFlash.spriteData.font8x8[(index * 8) + i];
    }
#else
    const uint8_t length = 8;
    memory.GetRom(FONT_8X8_POSITION + (length * index), glyph, length);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, glyph[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetFontChar16x16(MemoryInterface memory, uint8_t* glyph, uint8_t index)
{
#ifdef STANDALONE
    for (uint16_t i = 0; i < 16; i++)
    {
        glyph[i] = g_gameFlash.spriteData.font16x16[(index * 16) + i];
    }
#else
    const uint16_t length = 32;
    memory.GetRom(FONT_16X16_POSITION + (length * index), glyph, length);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < length; i++)
        memory.Print(str_spawn_creature_type, glyph[i]);
    memory.Print(new_line);
#endif
#endif
}


/**********************************************************************************************************************/
/*      Color
**********************************************************************************************************************/
SET_MEMORY(".core")
uint16_t Flash_GetColor(MemoryInterface memory, uint8_t color)
{
#ifdef STANDALONE
    // return Flash_GetColor(memory, PAL_OFF_WHITE_GRAY);
#else
    uint8_t color_value[2];
    memory.GetRom(COLORS_256_POSITION + (2 * (color)), color_value, 2);
    return ((uint16_t)color_value[0] << 8) | color_value[1];

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < 2; i++)
        memory.Print(str_spawn_creature_type, color_value[i]);
    memory.Print(new_line);
#endif
#endif
}


/**********************************************************************************************************************/
/*      Text
**********************************************************************************************************************/

/**********************************************************************************************************************/
/*  for fixed length string strings in an array
 *  use length * index to get the offset
**********************************************************************************************************************/
// UI
SET_MEMORY(".map")
void Flash_GetOptionText(MemoryInterface memory, uint8_t* textBuffer, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.menus.options[index];
#else
    memory.GetRom(STRINGS_MENU_OPTIONS_POSITION + (SMALL_STRINGS * index), textBuffer, SMALL_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, textBuffer[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetMenuText(MemoryInterface memory, uint8_t* textBuffer, uint8_t index)
{
#ifdef STANDALONE
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
    {
        textBuffer[i] = g_gameFlash.text.menus.main[index][i];
    }
#else
    memory.GetRom(STRINGS_MENU_MAIN_POSITION + (SMALL_STRINGS * index), textBuffer, SMALL_STRINGS);


#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, textBuffer[i]);
    memory.Print(new_line);
#endif
#endif
}

/**********************************************************************************************************************/
/** Returns a pointer to the char array for the given index of the battle menu
 *  Used for printing the menu options to the screen
**********************************************************************************************************************/

SET_MEMORY(".core")
void Flash_GetBattleMenuList(MemoryInterface memory, uint8_t* text, uint8_t idx)
{
#ifdef STANDALONE
    return g_gameFlash.text.menus.battleMenu[idx];
#else
    memory.GetRom(STRINGS_MENU_BATTLE_POSITION + (SMALL_STRINGS * idx), text, SMALL_STRINGS);


#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}


// Game Data
SET_MEMORY(".core")
void Flash_GetSpellbookText(MemoryInterface memory, char* textBuffer, uint8_t index)
{
#ifdef STANDALONE
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        textBuffer[i] = g_gameFlash.text.names.spells[index][i];
#else
    Flash_GetSpellName(memory, textBuffer, index);
#endif
}

SET_MEMORY(".core")
void Flash_GetCreatureDescription(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.monsters[index];
#else
    memory.GetRom(STRINGS_DESCRIPTIONS_CREATURES_POSITION + (LARGE_STRINGS * index), text, LARGE_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < LARGE_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}


SET_MEMORY(".core")
void Flash_GetCreatureName(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.monsters[index];
#else
    memory.GetRom(STRINGS_NAMES_CREATURES_POSITION + (SMALL_STRINGS * index), text, SMALL_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}


SET_MEMORY(".core")
void Flash_GetObjectDescription(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.objects[index];
#else
    memory.GetRom(STRINGS_DESCRIPTIONS_OBJECTS_POSITION + (LARGE_STRINGS * index), text, LARGE_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < LARGE_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetObjectName(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.objects[index];
#else
    memory.GetRom(STRINGS_NAMES_OBJECTS_POSITION + (SMALL_STRINGS * index), text, SMALL_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}


SET_MEMORY(".core")
void Flash_GetItemDescription(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.items[index];
#else
    memory.GetRom(STRINGS_DESCRIPTIONS_ITEMS_POSITION + (LARGE_STRINGS * index), text, LARGE_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < LARGE_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetItemName(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.items[index];
#else
    memory.GetRom(STRINGS_NAMES_ITEMS_POSITION + (SMALL_STRINGS * index), text, SMALL_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}


SET_MEMORY(".core")
void Flash_GetSpellDescription(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.spells[index];
#else
    memory.GetRom(STRINGS_DESCRIPTIONS_SPELLS_POSITION + (LARGE_STRINGS * index), text, LARGE_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < LARGE_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetSpellName(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.spells[index];
#else
    memory.GetRom(STRINGS_NAMES_SPELLS_POSITION + (SMALL_STRINGS * index), text, SMALL_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetSkillDescription(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.attacks[index];
#else
    memory.GetRom(STRINGS_DESCRIPTIONS_SKILLS_POSITION + (LARGE_STRINGS * index), text, LARGE_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < LARGE_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}

SET_MEMORY(".core")
void Flash_GetSkillName(MemoryInterface memory, char* text, uint8_t index)
{
#ifdef STANDALONE
    return g_gameFlash.text.descriptions.attacks[index];
#else
    memory.GetRom(STRINGS_NAMES_SKILLS_POSITION + (SMALL_STRINGS * index), text, SMALL_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}


SET_MEMORY(".core")
void Flash_GetTypeName(MemoryInterface memory, char* text, uint8_t typeIndex)
{
#ifdef STANDALONE
    return g_gameFlash.text.types[typeIndex];
#else
    memory.GetRom(STRINGS_TYPES_POSITION + (SMALL_STRINGS * typeIndex), text, SMALL_STRINGS);

#if defined(MEMORY_PRINT)
    for (uint8_t i = 0; i < SMALL_STRINGS; i++)
        memory.Print(str_spawn_creature_type, text[i]);
    memory.Print(new_line);
#endif
#endif
}


/**********************************************************************************************************************/
/**     FUNCTIONS
**********************************************************************************************************************/
SET_MEMORY(".core")
bool Flash_GetSkillEffect(MemoryInterface memory, uint8_t skillType, EntityId id, EntityId target_id, SkillData skillData)
{
#ifdef STANDALONE
    return g_gameFlash.funcs.abilityFunctions[skillType](id, target_id, skillData);
#else
    return abilityFunctions[skillType](id, target_id, skillData);
#endif
}

SET_MEMORY(".core")
bool Flash_GetSpellEffect(HardwareInterface hardware, MemoryInterface memory, uint8_t spellType, EntityId id, EntityId target_id, SpellData spellData)
{
#ifdef STANDALONE
    return g_gameFlash.funcs.spellFunctions[spellType](id, target_id, spellData);
#else
    return spellFunctions[spellType](hardware, memory, id, target_id, spellData);
#endif
}

SET_MEMORY(".core")
bool Flash_GetItemEffect(MemoryInterface memory, uint8_t itemType, EntityId item_id, EntityId id, ItemData itemData)
{
#ifdef STANDALONE
    return g_gameFlash.funcs.itemFunctions[itemType](item_id, id, itemData);
#else
    return itemFunctions[itemType](item_id, id, itemData);
#endif
}

SET_MEMORY(".map")
bool Flash_GetObjectEffect(MemoryInterface memory, HardwareInterface hardware, uint8_t objectType, EntityId object_id, EntityId target_id, ObjectData objectData)
{
#ifdef STANDALONE
    return g_gameFlash.funcs.objectFunctions[objectType](object_id, target_id, objectData);
#else
    return objectFunctions[objectType](hardware, object_id, target_id, objectData);
#endif
}
