//
// Created by nathanial on 2/20/26.
//
#pragma once
#include "types.h"
#include "stdbool.h"



/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
uint8_t CheckCollision(EntityId id);
uint8_t CheckTileForEntity(ObjectsTypes type, EntityId e_id, Position pos, Position* positions, uint8_t n);

EntityId SpawnEntity(ObjectsTypes type, uint8_t t, uint8_t x, uint8_t y, uint8_t l);
EntityId PickItem(EntityId id);
EntityId CaptureMonster(EntityId id);
void DestroyCreature(EntityId i);
void DestroyItem(EntityId id);
void DestroyObject(EntityId id);
void CopyCreature(EntityId src_id, EntityId target_id);
void CopyItem(EntityId src_id, EntityId target_id);
void CopyObject(EntityId src_id, EntityId target_id);
void PopulateLevelCreatures(void);
void PopulateLevelItems(void);
void PopulateLevelObjects(void);


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
IntMax99* GetCreatureSpeeds(void);
Position* GetEntityPositions(ObjectsTypes type);
Position* GetEntityNewPositions(void);
uint8_t* GetEntitiesOnMap(ObjectsTypes type);
BitFieldUint8* GetCreaturesAlive(void);
uint8_t* GetActiveObjects(void);

const SmallStringArray* GetEntityTypes(uint8_t* typeIDs, const uint8_t* e_ids, ObjectsTypes type, uint8_t n);
ItemTypes GetItemType(EntityId id);
Object GetObjectType(EntityId id);
Creature GetCreatureType(EntityId id);


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
Position GetEntityPosition(ObjectsTypes type, EntityId id);
uint8_t GetTilesetIdx(uint8_t id);
Position SetEntityPosition(ObjectsTypes type, EntityId id, uint8_t x, uint8_t y, uint8_t nx, uint8_t ny);
Position QueueObjectMovePosition(EntityId id, uint8_t x, uint8_t y);
void ResetEntities(bool copyPlayer);
void PopulateLevel(void);
bool CheckAlive(EntityId id);
bool InDetectionRange(EntityId id, EntityId targetID);
