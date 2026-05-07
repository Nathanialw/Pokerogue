//
// Created by nathanial on 2/21/26.
//
#pragma once
#include "types.h"


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void InitPlayer(void);
void UpdatePlayerPosition(void);
Position GetPlayerPosition(void);
EntityId GetPlayerID(void);
Delta SetPlayerDelta(Delta newDelta);
EntityId CachePlayerCreatureData();
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
void DestroyEnemyCreature();
void DestroyPlayerCreature();
void CachePlayerData();
void PlacePlayerOnMap();
void PlayerInteractItemInCell();
void PlayerInteractObjectInCell();