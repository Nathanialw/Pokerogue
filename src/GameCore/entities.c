//
// Created by nathanial on 2/20/26.
//

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "entities.h"

#include "battles.h"
#include "lib_debugging.h"
#include "map.h"
#include "utils.h"
#include "stats.h"

#include "memory_ram.h"
#include "memory_rom.h"
#include "player.h"


EntityId SpawnMonster(uint8_t monType, uint8_t x, uint8_t y, uint8_t l);
EntityId SpawnItem(uint8_t itmType, uint8_t x, uint8_t y, uint8_t l);
EntityId SpawnObject(uint8_t shrineType, uint8_t x, uint8_t y, uint8_t l);
typedef EntityId (*Spawn)(uint8_t, uint8_t, uint8_t, uint8_t);

/**********************************************************************************************************************/
/** point array for creating entities
**********************************************************************************************************************/
const Spawn spawn[TOTAL_SPAWNABLE_OBJECT_TYPES] = {SpawnMonster, SpawnItem, SpawnObject};

/**********************************************************************************************************************/
/**Takes in an entity ID
*   checks the queued positino for an entity
*   ON SUCCESS - returns the entity id of the entity in the new tile
*   ON FAIL - returns NO_OBJECT
**********************************************************************************************************************/
uint8_t CheckCollision(EntityId id)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    Position pos = g_run.creatures.newPosition[id];
    uint8_t creature_id = CheckTileForEntity(CREATURE, id, pos, g_run.creatures.position, ENTITY_COUNT);
    if (creature_id == NO_ENTITY)
        creature_id = CheckTileForEntity(CREATURE, id, pos, g_run.creatures.newPosition, ENTITY_COUNT);
    return creature_id;
};


/**********************************************************************************************************************/
/** Takes in the entity ID of the querying object, it's position, an array of positions to test against, the size of that array
 *  Returns the entity that is in the given cell
 *  Returns NO_OBJECT if none found
// TODO hashmap was a little pricey on memory, maybe a clever implementation would work, linear search for now
**********************************************************************************************************************/
uint8_t CheckTileForEntity(ObjectsTypes type, EntityId e_id, Position pos, Position* positions, uint8_t n)
{
    // ASSERT(e_id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    uint8_t* onMap = GetEntitiesOnMap(type);
    ASSERT(onMap, "CheckTileForEntity onMap array pointer is NULL!");

    for (uint16_t i = 0; i < n; ++i)
    {
        if (!GetBit(onMap, i)) continue;
        if (e_id != NO_ENTITY && i == e_id) continue;

        Position t_pos = positions[i];
        if (t_pos.x == pos.x && t_pos.y == pos.y)
            return i;
    }
    return NO_ENTITY;
}

/**********************************************************************************************************************/
/** saves the new position coords into the newPosition array for the gven entity id
**********************************************************************************************************************/
Position QueueObjectMovePosition(EntityId id, uint8_t x, uint8_t y)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    Position pos = {.x = x, .y = y};
    g_run.creatures.newPosition[id] = pos;
    return g_run.creatures.newPosition[id];
}

/**********************************************************************************************************************/
/**sets the current position as the newPosition value of the given entity id
**********************************************************************************************************************/
Position SetEntityPosition(ObjectsTypes type, EntityId id, uint8_t x, uint8_t y, uint8_t nx, uint8_t ny)
{
    Position pos = {.x = nx, .y = ny};
    if (type == CREATURE)
    {
        g_run.creatures.position[id] = pos;
        return g_run.creatures.position[id];
    }
    if (type == ITEM)
    {
        g_run.items.position[id] = pos;
        return g_run.items.position[id];
    }
    if (type == OBJECT)
    {
        g_run.objects.position[id] = pos;
        return g_run.objects.position[id];
    }

    ASSERT(type == CREATURE || type == ITEM || type == OBJECT, "SetEntityPosition %d invalid type", type);
    return pos;
}

/**********************************************************************************************************************/
/**Returns the position of a given entity ID
**********************************************************************************************************************/
Position GetEntityPosition(ObjectsTypes type, EntityId id)
{
    if (type == CREATURE)
        return g_run.creatures.position[id];
    if (type == ITEM)
        return g_run.items.position[id];
    if (type == OBJECT)
        return g_run.objects.position[id];
    return g_run.creatures.position[id];
}

/**********************************************************************************************************************/
/**Returns the array of entities on map status
**********************************************************************************************************************/
uint8_t* GetEntitiesOnMap(ObjectsTypes type)
{
    if (type == CREATURE)
        return g_run.creatures.onMap;
    if (type == ITEM)
        return g_run.items.onMap;
    if (type == OBJECT)
        return g_run.objects.onMap;

    ASSERT(type == CREATURE || type == ITEM || type == OBJECT, "GetEntityPositions %d invalid type", type);
    return NULL;
}

/**********************************************************************************************************************/
/**Returns the array of entities alive status
**********************************************************************************************************************/
BitFieldUint8* GetCreaturesAlive(void)
{
    return &g_run.creatures.alive;
}

/**********************************************************************************************************************/
/**Returns the array of entity speeds
**********************************************************************************************************************/
IntMax99* GetCreatureSpeeds(void)
{
    return g_run.creatures.speed;
}

/**********************************************************************************************************************/
/**Returns the array of entity positions
**********************************************************************************************************************/
Position* GetEntityPositions(ObjectsTypes type)
{
    if (type == CREATURE)
        return g_run.creatures.position;
    if (type == ITEM)
        return g_run.items.position;
    if (type == OBJECT)
        return g_run.objects.position;

    ASSERT(type == CREATURE || type == ITEM || type == OBJECT, "GetEntityPositions %d invalid type", type);
    return NULL;
}

/**********************************************************************************************************************/
/**Returns the array of entity NEW positions
**********************************************************************************************************************/
Position* GetEntityNewPositions(void)
{
    return g_run.creatures.newPosition;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given entity ID of type creature
*   TODO - get all values from the db data or generate them
**********************************************************************************************************************/
EntityId SpawnMonster(uint8_t type, uint8_t x, uint8_t y, uint8_t l)
{
    EntityId id = NO_ENTITY;
    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
        if (!GetBit(g_run.creatures.active, i))
        {
            id = i;
            SetBit(g_run.creatures.active, id, true);
            break;
        }

    Creature monType = (Creature)type;
    Position pos = {.x = x, .y = y};
    g_run.creatures.position[id] = pos;
    // g_run.objects.position_hash.
    g_run.creatures.level[id].value = l;
    g_run.creatures.types[id] = type;
    g_run.creatures.stats[id] = GetStats(monType, l);
    g_run.creatures.xp[id] = SetXPToLevel(id);
    g_run.creatures.hp[id] = GetHP(monType, l);
    g_run.creatures.mp[id] = GetMP(monType, l);
    g_run.creatures.senses[id].sight = 7;
    g_run.creatures.senses[id].sound = 7;
    g_run.creatures.senses[id].smell = 3;
    g_run.creatures.stealth[id].sight = 3;
    g_run.creatures.stealth[id].sound = 3;
    g_run.creatures.stealth[id].smell = 0;
    GetSkills(id, type);
    SetBit(g_run.creatures.alive, id, true);
    SetBit(g_run.creatures.onMap, id, true);
    g_run.creatures.speed[id].current = 0;
    g_run.creatures.speed[id].max = 40;
    return id;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given entity ID of type item
 *  //TODO: certain items will use metaData to store type of item, for example the spell ID of a spellbook
 *  //TODO: use 'l' to generate item of the appropriate level
**********************************************************************************************************************/
EntityId SpawnItem(uint8_t type, uint8_t x, uint8_t y, uint8_t l)
{
    EntityId id = NO_ENTITY;
    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
        if (!GetBit(g_run.items.active, i))
        {
            id = i;
            SetBit(g_run.items.active, id, true);
            break;
        }

    Position pos = {.x = x, .y = y};
    g_run.items.position[id] = pos;
    g_run.items.types[id] = type;
    SetBit(g_run.items.onMap, id, true);
    g_run.items.metaData[id].value = (l + 10) + (l * 5);
    return id;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given entity ID of type object
 *  TODO - cahnge to a generic object spawner, we wll have 255 object types, shirne will be one
**********************************************************************************************************************/
EntityId SpawnObject(uint8_t type, uint8_t x, uint8_t y, uint8_t l)
{
    EntityId id = NO_ENTITY;
    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
        if (!GetBit(g_run.creatures.active, i))
        {
            id = i;
            SetBit(g_run.creatures.active, id, true);
            break;
        }

    SetBit(g_run.objects.onMap, id, true);
    return id;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given the type of object, the id of that type, position and level
**********************************************************************************************************************/
EntityId SpawnEntity(ObjectsTypes type, uint8_t t, uint8_t x, uint8_t y, uint8_t l)
{
    return spawn[type](t, x, y, l);;
}

/**********************************************************************************************************************/
/** sets the creature position to 0,0
*   sets creature to false on the map array
*   returns the entity id of the creature
**********************************************************************************************************************/
EntityId CaptureMonster(EntityId id)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    SetBit(g_run.creatures.onMap, id, false);
    Position empty_pos = {.x = 0, .y = 0};
    g_run.creatures.position[id] = empty_pos;
    return id;
}

/**********************************************************************************************************************/
/** sets the item position to 0,0
*   sets item to false on the map array
*   returns the entity id of the item
**********************************************************************************************************************/
EntityId PickItem(EntityId id)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    Position empty_pos = {.x = 0, .y = 0};
    g_run.items.position[id] = empty_pos;
    SetBit(g_run.items.onMap, id, false);
    return id;
}

/**********************************************************************************************************************/
/**Reset all values of the given entity ID
**********************************************************************************************************************/
void DestroyCreature(EntityId id)
{
    Position empty_pos = {.x = 0, .y = 0};
    g_run.creatures.position[id] = empty_pos;
    SetBit(g_run.creatures.onMap, id, false);
    g_run.creatures.types[id] = NO_ENTITY;
    g_run.creatures.metaData[id].unused = NO_ENTITY;
    SetBit(g_run.creatures.alive, id, false);
    g_run.creatures.senses[id].sight = 0;
    g_run.creatures.senses[id].smell = 0;
    g_run.creatures.senses[id].sound = 0;
    g_run.creatures.stealth[id].sight = 0;
    g_run.creatures.stealth[id].sound = 0;
    g_run.creatures.stealth[id].smell = 0;
    g_run.creatures.stats[id].attack = 0;
    g_run.creatures.stats[id].defence = 0;
    g_run.creatures.stats[id].magic = 0;
    g_run.creatures.stats[id].speed = 0;
    memset(g_run.creatures.attacks[id], NO_ABILITY, 8);
    Int999SetCurrent(&g_run.creatures.hp[id], 0);
    Int999SetMax(&g_run.creatures.hp[id], 0);
    g_run.creatures.level[id].value = 0;
    SetBit(g_run.creatures.active, id, false);
}


void DestroyItem(EntityId id)
{
    Position empty_pos = {.x = 0, .y = 0};
    g_run.items.position[id] = empty_pos;
    SetBit(g_run.items.onMap, id, false);
    g_run.items.types[id] = NO_ENTITY;
    g_run.items.metaData[id].unused = NO_ENTITY;
    SetBit(g_run.items.active, id, false);
}


void DestroyObject(EntityId id)
{
    Position empty_pos = {.x = 0, .y = 0};
    g_run.objects.position[id] = empty_pos;
    SetBit(g_run.objects.onMap, id, false);
    g_run.objects.types[id] = NO_ENTITY;
    g_run.objects.metaData[id].unused = NO_ENTITY;
    SetBit(g_run.objects.active, id, false);
}

/**********************************************************************************************************************/
/**Copy all values of the given entity ID
**********************************************************************************************************************/
void CopyCreature(EntityId src_id, EntityId target_id)
{
    g_run.creatures.position[target_id] = g_run.creatures.position[src_id];
    g_run.creatures.types[target_id] = g_run.creatures.types[src_id];

    g_run.creatures.metaData[target_id].unused = g_run.creatures.metaData[src_id].unused;
    g_run.creatures.senses[target_id].sight = g_run.creatures.senses[src_id].sight;
    g_run.creatures.senses[target_id].smell = g_run.creatures.senses[src_id].smell;
    g_run.creatures.senses[target_id].sound = g_run.creatures.senses[src_id].sound;
    g_run.creatures.stealth[target_id].sight = g_run.creatures.stealth[src_id].sight;
    g_run.creatures.stealth[target_id].sound = g_run.creatures.stealth[src_id].sound;
    g_run.creatures.stealth[target_id].smell = g_run.creatures.stealth[src_id].smell;
    g_run.creatures.stats[target_id].attack = g_run.creatures.stats[src_id].attack;
    g_run.creatures.stats[target_id].defence = g_run.creatures.stats[src_id].defence;
    g_run.creatures.stats[target_id].magic = g_run.creatures.stats[src_id].magic;
    g_run.creatures.stats[target_id].speed = g_run.creatures.stats[src_id].speed;
    g_run.creatures.level[target_id].value = g_run.creatures.level[src_id].value;

    memset(g_run.creatures.attacks[target_id], NO_ABILITY, 8);
    Int999SetCurrent(&g_run.creatures.hp[target_id], 0);
    Int999SetMax(&g_run.creatures.hp[target_id], 0);

    SetBit(g_run.creatures.alive, target_id, GetBit(g_run.creatures.alive, src_id));
    SetBit(g_run.creatures.onMap, target_id, GetBit(g_run.creatures.onMap, src_id));
    SetBit(g_run.creatures.active, target_id, GetBit(g_run.creatures.active, src_id));
}


void CopyItem(EntityId src_id, EntityId target_id)
{
    g_run.items.position[target_id] = g_run.items.position[src_id];
    g_run.items.types[target_id] = g_run.items.types[src_id];
    g_run.items.metaData[target_id].unused = g_run.items.metaData[src_id].unused;

    SetBit(g_run.items.onMap, target_id, GetBit(g_run.items.onMap, src_id));
    SetBit(g_run.items.active, target_id, GetBit(g_run.items.active, src_id));
}


void CopyObject(EntityId src_id, EntityId target_id)
{
    g_run.items.position[target_id] = g_run.items.position[src_id];
    g_run.items.types[target_id] = g_run.items.types[src_id];
    g_run.items.metaData[target_id].unused = g_run.items.metaData[src_id].unused;

    SetBit(g_run.items.onMap, target_id, GetBit(g_run.items.onMap, src_id));
    SetBit(g_run.items.active, target_id, GetBit(g_run.items.active, src_id));
}


/**********************************************************************************************************************/
/** Reset all values of all entities on the map
 *  TODO: may add trainers later
**********************************************************************************************************************/
void ResetEntities(bool copyPlayer)
{
    uint16_t creature_start_idx = 0;
    uint16_t item_start_idx = 0;
    if (copyPlayer)
    {
        creature_start_idx = CachePlayerCreatureData();
        item_start_idx = CachePlayerItemData();
    }

    for (uint16_t i = creature_start_idx; i < ENTITY_COUNT; ++i)
        DestroyCreature(i);

    for (uint16_t i = item_start_idx; i < ENTITY_COUNT; ++i)
        DestroyItem(i);

    for (uint16_t i = 0; i < ENTITY_COUNT; ++i)
        DestroyObject(i);
}

/**********************************************************************************************************************/
/** Creates all the creatures on the map from the BIOME and THEME data
**********************************************************************************************************************/
void PopulateLevelCreatures()
{
    uint8_t creature_level = 1;
    for (uint8_t i = 0; i < NUM_BIOME_CREATURES; ++i)
    {
        uint8_t creature_type = 0 + rand() % BIOME_MONSTER_TYPES;
        const Creature creature = g_gameFlash.tileset.monsterGroups[g_run.biome][creature_type];
        const Position pos = FindOpenMapLocation(CREATURE);
        SpawnEntity(CREATURE, creature, pos.x, pos.y, creature_level);
    }

    for (uint8_t i = 0; i < NUM_THEME_CREATURES; ++i)
    {
        uint8_t creature_type = 0 + rand() % THEME_MONSTER_TYPES;
        const Creature creature = g_gameFlash.tileset.themeGroups[g_run.theme][creature_type];
        const Position pos = FindOpenMapLocation(CREATURE);
        SpawnEntity(CREATURE, creature, pos.x, pos.y, creature_level);
    }

    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
        if (GetBit(g_run.creatures.onMap, i) && GetBit(g_run.creatures.alive, i))
            g_run.creatures.newPosition[i] = g_run.creatures.position[i];
}

void PopulateLevelItems(void)
{
}

void PopulateLevelObjects(void)
{
}

/**********************************************************************************************************************/
/** Fill typeIDs[] with the types of the given e_ids[] array
 *  ON SUCCESS - Returns the type ID of the given entity ID
 *  ON FAIL - sets typeIDs to NULL and returns NULL
**********************************************************************************************************************/
const SmallStringArray* GetEntityTypes(uint8_t* typeIDs, const uint8_t* e_ids, ObjectsTypes type, uint8_t n)
{
    const SmallStringArray* text = NULL;

    switch (type)
    {
    case CREATURE:
        {
            text = g_gameFlash.text.names.monsters;
            for (uint8_t i = 0; i < n; ++i)
                typeIDs[i] = GetCreatureType(e_ids[i]);
            break;
        }
    case ITEM:
        {
            text = g_gameFlash.text.names.items;
            for (uint8_t i = 0; i < n; ++i)
                typeIDs[i] = GetItemType(e_ids[i]);
            break;
        }
    case OBJECT:
        {
            text = g_gameFlash.text.names.objects;
            for (uint8_t i = 0; i < n; ++i)
                typeIDs[i] = GetObjectType(e_ids[i]);
            break;
        }
    default:
        {
            typeIDs = NULL;
            return NULL;
        }
    }

    return text;
}


/**********************************************************************************************************************/
/** Returns the type ID of the given entity ID
**********************************************************************************************************************/
ItemTypes GetItemType(EntityId id)
{
    if (id == NO_ENTITY)
        return NO_ITEM;

    return g_run.items.types[id];
}


Object GetObjectType(EntityId id)
{
    if (id == NO_ENTITY)
        return NO_OBJECT;

    return g_run.objects.types[id];
}


Creature GetCreatureType(EntityId id)
{
    if (id == NO_ENTITY)
        return NO_CREATURE;

    return g_run.creatures.types[id];
}


/**********************************************************************************************************************/
/** Returns the array of active objects
**********************************************************************************************************************/
uint8_t* GetActiveObjects(void)
{
    return g_run.creatures.active;
}

/**********************************************************************************************************************/
/**Returns true if the given entity is Alive
**********************************************************************************************************************/
bool CheckAlive(EntityId id)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");
    return GetBit(g_run.creatures.active, id);
}

/**********************************************************************************************************************/
/** Checks whether is given entity ID can detect the target entity ID
 * ON SUCCESS - returns true
 * ON FAIL - returns false
**********************************************************************************************************************/
bool InDetectionRange(EntityId id, EntityId targetID)
{
    uint8_t smell = g_run.creatures.senses[id].smell;
    uint8_t smell_stealth = g_run.creatures.stealth[targetID].smell;

    if (smell > smell_stealth)
        return true;

    uint8_t sound = g_run.creatures.senses[id].sound;
    uint8_t sound_stealth = g_run.creatures.stealth[targetID].sound;

    if (sound > sound_stealth)
        return true;

    uint8_t sight = g_run.creatures.senses[id].sight;
    uint8_t sight_stealth = g_run.creatures.stealth[targetID].sight;

    if (sight > sight_stealth)
        return true;

    return false;
}

/**********************************************************************************************************************/
/**Returns true if the given entity is Alive
**********************************************************************************************************************/
bool CheckEntity(EntityId id)
{
    return false;
}
