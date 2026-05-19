//
// Created by nathanial on 3/29/26.
//
#pragma once
#include "types.h"


/**********************************************************************************************************************/
/*
 *  All effects for items, objects, skills and spells are stored here as functions
*   Effect Functions make it easy to mix and match any effects to build unique actions
*
**********************************************************************************************************************/

/**********************************************************************************************************************/
/** ACCESSIBLE FOR TESTING
**********************************************************************************************************************/
#ifdef ENABLE_DEBUG_OUTPUT
uint16_t CalcModifier(MemoryInterface memory, EntityId attackerID, EntityId defenderID, Type attackType, uint16_t damage);
uint16_t CalcDamage(EntityId creatureID, uint16_t abilityPower);
uint16_t CalcHeal(EntityId creatureID, uint16_t abilityPower);
#endif

bool NoEffect();

/**********************************************************************************************************************/
/** DAMAGE EFFECTS
**********************************************************************************************************************/
void Attack(HardwareInterface hardware, MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData);
void InstantKill(HardwareInterface hardware, MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData);
bool QuickAttack(HardwareInterface hardware, MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData);
bool Parry(EntityId attackerID, EntityId defenderID, SkillData abilityData);
bool Summon(CreatureID creature);
void SacrificeHeal(HardwareInterface hardware, MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData);


/**********************************************************************************************************************/
/** RESOURCE EFFECTS
**********************************************************************************************************************/
bool Revive(EntityId e_id);
bool MakeInvulnerable(EntityId e_id);
bool HealTarget(EntityId e_id, uint16_t value);
bool RestoreMana(EntityId e_id, uint8_t value);
bool DrainMana(EntityId e_id, uint8_t value);
bool RestorePP(EntityId e_id, uint8_t value);

/**********************************************************************************************************************/
/** STATUS EFFECTS
**********************************************************************************************************************/
bool ApplyPoison(EntityId e_id, uint8_t duration);
bool ApplyCurse(EntityId e_id, uint8_t duration);
bool ApplyDisease(EntityId e_id, uint8_t duration);
bool ApplyParalyze(EntityId e_id, uint8_t duration);
bool ApplySleep(EntityId e_id, uint8_t duration);
bool ApplyFrozen(EntityId e_id, uint8_t duration);
bool ApplyFear(EntityId e_id, uint8_t duration);
bool ApplyBurn(EntityId e_id, uint8_t duration);
bool ApplySlow(EntityId e_id, uint8_t duration);
bool ApplyHaste(EntityId e_id, uint8_t duration);
bool RemovePoison(EntityId e_id);
bool RemoveCurse(EntityId e_id);
bool RemoveDisease(EntityId e_id);
bool RemoveParalyze(EntityId e_id);
bool RemoveSleep(EntityId e_id);
bool RemoveFrozen(EntityId e_id);
bool RemoveFear(EntityId e_id);
bool RemoveBurn(EntityId e_id);
bool RemoveSlow(EntityId e_id);
bool RemoveHaste(EntityId e_id);
void NextAttackFreezes();
bool FreezeAttackers(EntityId e_id, uint8_t duration);
bool PersistentPoisonCloud(uint8_t duration);

/**********************************************************************************************************************/
/** RESISTANCES
**********************************************************************************************************************/
bool RaiseAcidResistance(EntityId e_id);
bool RaiseFireResistance(EntityId e_id);
bool RaiseWaterResistance(EntityId e_id);
bool RaiseIceResistance(EntityId e_id);
bool RaiseMagicResistance(EntityId e_id);
bool RaiseAllResistance(EntityId e_id);
bool LowerAcidResistance(EntityId e_id);
bool LowerFireResistance(EntityId e_id);
bool LowerWaterResistance(EntityId e_id);
bool LowerIceResistance(EntityId e_id);
bool LowerMagicResistance(EntityId e_id);
bool FireEating(EntityId e_id);
bool WaterEating(EntityId e_id);
bool IceEating(EntityId e_id);
bool AcidEating(EntityId e_id);
bool LavaEating(EntityId e_id);


/**********************************************************************************************************************/
/** CREATURE STATS
**********************************************************************************************************************/
bool LowerAccuracy(EntityId e_id);
bool RaiseAccuracy(EntityId e_id);
bool RaiseStrength(EntityId e_id);
bool RaiseDefence(EntityId e_id);
bool RaiseMagic(EntityId e_id);
bool RaiseSpeed(EntityId e_id);
bool LowerSpeed(EntityId e_id);
bool LowerDefence(EntityId e_id);
bool LowerMagic(EntityId e_id);
bool LowerStrength(EntityId e_id);
bool RaiseBaseStrength(EntityId e_id);
bool RaiseBaseDefence(EntityId e_id);
bool RaiseBaseMagic(EntityId e_id);
bool RaiseBaseSpeed(EntityId e_id);
bool LowerBaseSpeed(EntityId e_id);
bool LowerBaseDefence(EntityId e_id);
bool LowerBaseMagic(EntityId e_id);
bool LowerBaseStrength(EntityId e_id);
bool DrainXP(EntityId e_id);
bool ResetTemporaryStatsMods(EntityId e_id);

/**********************************************************************************************************************/
/** PLAYER
**********************************************************************************************************************/
bool DiscoverItem(EntityId e_id);
bool DiscoverCreature(EntityId e_id);
bool DiscoverObject(EntityId e_id);
bool DiscoverSpell(Spell spell_id);
bool DiscoverSkill(Ability ability_id);
bool LearnSpell(EntityId e_id);
bool LearnSkill(EntityId e_id);
bool Capture(EntityId target_id, uint8_t successChance);
bool AbandonTeam(EntityId e_id);
bool Flee();
void DestroyPlayerItem(EntityId item_id);
void DestroyRandomPlayerItem();

/**********************************************************************************************************************/
/** MAP EFFECTS
**********************************************************************************************************************/
bool Repel(EntityId e_id, uint8_t duration);
bool Levitate(EntityId e_id, uint8_t duration);
bool Invisibility(EntityId e_id, uint8_t duration);
bool RevealMap();
bool Reposition(EntityId e_id, Position pos);

bool StatusGreaterLight(EntityId e_id, uint8_t duration);
bool StatusLesserLight(EntityId e_id, uint8_t duration);

bool WaterWalking(EntityId e_id, uint8_t duration);
bool WallWalking(EntityId e_id, uint8_t duration);
bool WaterBreathing(EntityId e_id, uint8_t duration);
bool XRayVision(EntityId e_id, uint8_t duration);
bool MapDescend(HardwareInterface hardware, MemoryInterface memory, EntityId e_id);
bool MapAscend(HardwareInterface hardware, MemoryInterface memory, EntityId e_id);
bool MapLateral(HardwareInterface hardware, MemoryInterface memory, EntityId e_id);
bool CreateItem(ItemTypes item);
bool CreateItemFood();
bool CreateItemCommon();
bool CreateItemMagic();
bool MapModifyTile(Position, TileType tileType);
