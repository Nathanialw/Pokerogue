//
// Created by nathanial on 2/21/26.
//
#pragma once
#include "types.h"


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void InitPlayer(HardwareInterface hardware, MemoryInterface memory);
void UpdatePlayerPosition(void);
Position GetPlayerPosition(void);
EntityId GetPlayerID(void);
Delta SetPlayerDelta(Delta newDelta);
EntityId CachePlayerCreatureData(HardwareInterface hardware);
EntityId CachePlayerItemData();

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void ConsumeItem(uint8_t idx, EntityId e_id);
EntityId* GetPlayerItemsIDs(void);
EntityId* GetPlayerMonsterIDs(void);
uint8_t GetPlayerType(void);
bool PlayerDefeated(void);
bool IsInParty(EntityId id);


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckPlayerAttackOutcome();
bool CheckEnemyAttackOutcome();
void DestroyEnemyCreature(HardwareInterface hardware);
void DestroyPlayerCreature(HardwareInterface hardware);
void PlacePlayerOnMap(HardwareInterface hardware);
void PlayerInteractItemInCell();
void PlayerInteractObjectInCell(MemoryInterface memory, HardwareInterface hardware);