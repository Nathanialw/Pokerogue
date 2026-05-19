//
// Created by nathanial on 5/15/26.
//
#pragma once
#include <stdint.h>

#include "lib_decl.h"

#include "enums.h"
#include "types.h"

// ENTITIES
SkillLearnLevel Flash_GetSkill(MemoryInterface memory, CreatureSkillLearnLevels c, Type creatureType, uint8_t index);
uint8_t Flash_GetStatGrowth(MemoryInterface memory, Creature type);
Creature Flash_GetBiomeCreature(MemoryInterface memory, uint8_t biome, Creature index);
Creature Flash_GetThemeCreature(MemoryInterface memory, uint8_t theme, Creature index);
void Flash_GetCreatureStatsRange(MemoryInterface memory, StatsRange* stats, Creature creature_type);
MonsterType Flash_GetType(MemoryInterface memory, uint8_t index);
int8_t Flash_GetTypeEffects(MemoryInterface memory, uint8_t index);
SkillData Flash_GetSkillData(MemoryInterface memory, uint8_t index);
SpellData Flash_GetSpellData(MemoryInterface memory, uint8_t index);
ItemData Flash_GetItemData(MemoryInterface memory, uint8_t index);
ObjectData Flash_GetObjectData(MemoryInterface memory, uint8_t index);

//MAP
void Flash_GetBiomeTile(MemoryInterface memory, Tile* tile, uint8_t biomeType, uint8_t tile_id);

//GRAPHICS
void Flash_GetSpriteMetadata(MemoryInterface memory, Sprite* sprite, ObjectsTypes type, uint8_t index);
void Flash_GetSpriteLayout(MemoryInterface memory, SpriteLayout spriteLayout, uint8_t index, ObjectsTypes type, bool front);
void Flash_GetSprite(MemoryInterface memory, uint8_t* sprite, uint16_t length, ObjectsTypes type, bool front);
SpriteLayout Flash_GetBattlerLayout(MemoryInterface memory, uint8_t creatureType, bool front);
const uint8_t* Flash_GetBattlerArray(MemoryInterface memory, bool front);

//ANIMATION
void Flash_BattlerAttackAnimation(MemoryInterface memory, EntityId id, ObjectsTypes type, bool player);
void Flash_BattlerStruckAnimation(MemoryInterface memory, EntityId id, ObjectsTypes type, bool player);

///FONTS
void Flash_GetFontChar8x8(MemoryInterface memory, uint8_t* glyph, uint8_t index);
void Flash_GetFontChar16x16(MemoryInterface memory, uint8_t* glyph, uint8_t index);

///COLORS
uint16_t Flash_GetColor(MemoryInterface memory, uint8_t color);

///TEXT
//ui
void Flash_GetOptionText(MemoryInterface memory, uint8_t* textBuffer, uint8_t index);
void Flash_GetMenuText(MemoryInterface memory, uint8_t* textBuffer, uint8_t index);
void Flash_GetBattleMenuList(MemoryInterface memory, uint8_t* text, uint8_t idx);
//entities

void Flash_GetSpellbookText(MemoryInterface memory, char* textBuffer, uint8_t index);
void Flash_GetCreatureName(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetCreatureDescription(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetObjectName(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetObjectDescription(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetItemName(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetItemDescription(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetSpellName(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetSpellDescription(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetSkillName(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetSkillDescription(MemoryInterface memory, char* text, uint8_t index);
void Flash_GetTypeName(MemoryInterface memory, char* text, uint8_t typeIndex);

//FUNCTIONS
bool Flash_GetSkillEffect(MemoryInterface memory, uint8_t index, EntityId id, EntityId target_id, SkillData skillData);
bool Flash_GetSpellEffect(HardwareInterface hardware, MemoryInterface memory, uint8_t index, EntityId id, EntityId target_id, SpellData spellData);
bool Flash_GetItemEffect(MemoryInterface memory, uint8_t index, EntityId item_id, EntityId id, ItemData itemData);
bool Flash_GetObjectEffect(MemoryInterface memory, HardwareInterface hardware, uint8_t index, EntityId object_id, EntityId target_id, ObjectData objectData);
