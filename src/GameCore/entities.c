//
// Created by nathanial on 2/20/26.
//
#include "entities.h"

#include "lib_types.h"
#include "lib_debugging.h"

#include "battles.h"
#include "map.h"
#include "memory_access.h"
#include "utils.h"
#include "stats.h"

#include "memory_ram.h"
#include "memory_rom.h"
#include "player.h"


EntityId SpawnMonster(HardwareInterface hardware, MemoryInterface memory, uint8_t monType, uint8_t x, uint8_t y, uint8_t l);
EntityId SpawnItem(HardwareInterface hardware, MemoryInterface memory, uint8_t itmType, uint8_t x, uint8_t y, uint8_t l);
EntityId SpawnObject(HardwareInterface hardware, MemoryInterface memory, uint8_t shrineType, uint8_t x, uint8_t y, uint8_t l);
typedef EntityId (*Spawn)(HardwareInterface hardware, MemoryInterface memory, uint8_t, uint8_t, uint8_t, uint8_t);

/**********************************************************************************************************************/
/** point array for creating entities
**********************************************************************************************************************/
SET_MEMORY(".map.rodata")
const Spawn spawn[TOTAL_SPAWNABLE_OBJECT_TYPES] = {SpawnMonster, SpawnItem, SpawnObject};

/**********************************************************************************************************************/
/**Takes in an entity ID
*   checks the queued positino for an entity
*   ON SUCCESS - returns the entity id of the entity in the new tile
*   ON FAIL - returns NO_OBJECT
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t CheckCollision(EntityId id)
{
    Position pos = g_core.creatures.newPosition[id];
    uint8_t creature_id = CheckTileForEntity(CREATURE, id, pos);
    if (creature_id == NO_ENTITY)
        creature_id = CheckTileForEntity(CREATURE, id, pos);
    return creature_id;
};


/**********************************************************************************************************************/
/** Takes in the entity ID of the querying object, it's position, an array of positions to test against, the size of that array
 *  Returns the entity that is in the given cell
 *  Returns NO_OBJECT if none found
// TODO hashmap was a little pricey on memory, maybe a clever implementation would work, linear search for now
**********************************************************************************************************************/
SET_MEMORY(".map")
EntityId CheckTile(ObjectsTypes type, EntityId e_id, Position pos, Position* positions, uint8_t n)
{
    uint8_t* onMap = GetEntitiesOnMap(type);

    for (uint16_t i = 0; i < n; ++i)
    {
        if (!GetBit(onMap, i)) continue;
        if (type == CREATURE && e_id != NO_ENTITY && i == e_id) continue;

        Position t_pos = positions[i];
        if (t_pos.x == pos.x && t_pos.y == pos.y)
            return i;
    }
    return NO_ENTITY;
}

SET_MEMORY(".map")
EntityId CheckTileForEntity(ObjectsTypes type, EntityId e_id, Position pos)
{
    // ASSERT(e_id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    if (type == CREATURE)
    {
        return CheckTile(type, e_id, pos, g_core.creatures.position, g_core.creatures.total);
    }

    if (type == ITEM)
    {
        return CheckTile(type, e_id, pos, g_core.items.position, g_core.items.total);
    }

    if (type == OBJECT)
    {
        return CheckTile(type, e_id, pos, g_core.objects.position, g_core.objects.total);
    }

    ASSERT(type == CREATURE || type == ITEM || type == OBJECT, "CheckTileForEntity %d invalid type", type);
    return NO_ENTITY;
}

/**********************************************************************************************************************/
/** saves the new position coords into the newPosition array for the gven entity id
**********************************************************************************************************************/
SET_MEMORY(".map")
Position QueueObjectMovePosition(EntityId id, uint8_t x, uint8_t y)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    Position pos = {.x = x, .y = y};
    g_core.creatures.newPosition[id] = pos;
    return g_core.creatures.newPosition[id];
}

/**********************************************************************************************************************/
/**sets the current position as the newPosition value of the given entity id
**********************************************************************************************************************/
SET_MEMORY(".map")
Position SetEntityPosition(ObjectsTypes type, EntityId id, uint8_t x, uint8_t y, uint8_t nx, uint8_t ny)
{
    Position pos = {.x = nx, .y = ny};
    if (type == CREATURE)
    {
        g_core.creatures.position[id] = pos;
        return g_core.creatures.position[id];
    }
    if (type == ITEM)
    {
        g_core.items.position[id] = pos;
        return g_core.items.position[id];
    }
    if (type == OBJECT)
    {
        g_core.objects.position[id] = pos;
        return g_core.objects.position[id];
    }

    ASSERT(type == CREATURE || type == ITEM || type == OBJECT, "SetEntityPosition %d invalid type", type);
    return pos;
}

/**********************************************************************************************************************/
/**Returns the position of a given entity ID
**********************************************************************************************************************/
SET_MEMORY(".map")
Position GetEntityPosition(ObjectsTypes type, EntityId id)
{
    if (type == CREATURE)
        return g_core.creatures.position[id];
    if (type == ITEM)
        return g_core.items.position[id];
    if (type == OBJECT)
        return g_core.objects.position[id];
    return g_core.creatures.position[id];
}

/**********************************************************************************************************************/
/**Returns the array of entities on map status
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t* GetEntitiesOnMap(ObjectsTypes type)
{
    if (type == CREATURE)
        return g_core.creatures.onMap;
    if (type == ITEM)
        return g_core.items.onMap;
    if (type == OBJECT)
        return g_core.objects.onMap;

    ASSERT(type == CREATURE || type == ITEM || type == OBJECT, "GetEntityPositions %d invalid type", type);
    return NULL;
}

/**********************************************************************************************************************/
/**Returns the array of entities alive status
**********************************************************************************************************************/
BitFieldUint8* GetCreaturesAlive(void)
{
    return &g_core.creatures.alive;
}

/**********************************************************************************************************************/
/**Returns the array of entity speeds
**********************************************************************************************************************/
SET_MEMORY(".map")
IntMax99* GetCreatureSpeeds(void)
{
    return g_core.creatures.speed;
}

/**********************************************************************************************************************/
/**Returns the array of entity positions
**********************************************************************************************************************/
SET_MEMORY(".map")
Position* GetEntityPositions(ObjectsTypes type)
{
    if (type == CREATURE)
        return g_core.creatures.position;
    if (type == ITEM)
        return g_core.items.position;
    if (type == OBJECT)
        return g_core.objects.position;

    ASSERT(type == CREATURE || type == ITEM || type == OBJECT, "GetEntityPositions %d invalid type", type);
    return NULL;
}

/**********************************************************************************************************************/
/**Returns the array of entity NEW positions
**********************************************************************************************************************/
SET_MEMORY(".map")
Position* GetEntityNewPositions(void)
{
    return g_core.creatures.newPosition;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given entity ID of type creature
*   TODO - get all values from the db data or generate them
**********************************************************************************************************************/
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_start[] = "SpawnMonster start\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_reset[] = "SpawnMonster reset\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_stats[] = "SpawnMonster stats\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_level[] = "SpawnMonster level\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_hp[] = "SpawnMonster hp\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_mp[] = "SpawnMonster mp\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_type[] = "SpawnMonster type\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_skills[] = "SpawnMonster skills\n";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_done[] = "SpawnMonster done\n";


SET_MEMORY(".map")
EntityId SpawnMonster(HardwareInterface hardware, MemoryInterface memory, uint8_t type, uint8_t x, uint8_t y, uint8_t l)
{
    hardware.Print(str_spawn_creature_start);

    EntityId id = NO_ENTITY;
    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
        if (!GetBit(g_core.creatures.active, i))
        {
            id = i;
            SetBit(g_core.creatures.active, id, true);
            break;
        }

    hardware.Print(str_spawn_creature_reset);

    Creature monType = (Creature)type;
    Position pos = {.x = x, .y = y};
    g_core.creatures.position[id] = pos;
    g_core.creatures.level[id].value = l;
    g_core.creatures.types[id] = type;

    hardware.Print(str_spawn_creature_stats);

    GetStats(hardware, memory, &g_core.creatures.stats[id], monType, l);

    hardware.Print(str_spawn_creature_level);

    SetXPToLevel(id, g_core.creatures.xp[id]);

    hardware.Print(str_spawn_creature_hp);

    g_core.creatures.hp[id] = GetHP(monType, l);

    hardware.Print(str_spawn_creature_mp);

    g_core.creatures.mp[id] = GetMP(monType, l);

    hardware.Print(str_spawn_creature_type);

    g_core.creatures.senses[id].sight = 7;
    g_core.creatures.senses[id].sound = 7;
    g_core.creatures.senses[id].smell = 3;
    g_core.creatures.stealth[id].sight = 3;
    g_core.creatures.stealth[id].sound = 3;
    g_core.creatures.stealth[id].smell = 0;

    hardware.Print(str_spawn_creature_skills);

    GetSkills(memory, id, type);
    SetBit(g_core.creatures.alive, id, true);
    SetBit(g_core.creatures.onMap, id, true);
    g_core.creatures.speed[id].current = 0;
    g_core.creatures.speed[id].max = 40;
    g_core.creatures.total++;

    hardware.Print(str_spawn_creature_done);

    return id;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given entity ID of type item
 *  //TODO: certain items will use metaData to store type of item, for example the spell ID of a spellbook
 *  //TODO: use 'l' to generate item of the appropriate level
**********************************************************************************************************************/
SET_MEMORY(".map")
EntityId SpawnItem(HardwareInterface hardware, MemoryInterface memory, uint8_t type, uint8_t x, uint8_t y, uint8_t l)
{
    EntityId id = NO_ENTITY;
    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
        if (!GetBit(g_core.items.active, i))
        {
            id = i;
            SetBit(g_core.items.active, id, true);
            break;
        }

    Position pos = {.x = x, .y = y};
    g_core.items.position[id] = pos;
    g_core.items.types[id] = type;
    SetBit(g_core.items.onMap, id, true);
    g_core.items.metaData[id].value = (l + 10) + (l * 5);
    g_core.items.total++;
    return id;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given entity ID of type object
 *  TODO - cahnge to a generic object spawner, we wll have 255 object types, shirne will be one
**********************************************************************************************************************/
SET_MEMORY(".map")
EntityId SpawnObject(HardwareInterface hardware, MemoryInterface memory, uint8_t type, uint8_t x, uint8_t y, uint8_t l)
{
    EntityId id = NO_ENTITY;
    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
        if (!GetBit(g_core.objects.active, i))
        {
            id = i;
            SetBit(g_core.objects.active, id, true);
            break;
        }

    SetBit(g_core.objects.onMap, id, true);
    Position pos = {.x = x, .y = y};
    g_core.objects.position[id] = pos;
    g_core.objects.types[id] = type;
    g_core.objects.metaData[id].value = (l + 10) + (l * 5);
    g_core.objects.total++;
    return id;
}

/**********************************************************************************************************************/
/** Sets initial data values of a given the type of object, the id of that type, position and level
**********************************************************************************************************************/
SET_MEMORY(".map")
EntityId SpawnEntity(HardwareInterface hardware, MemoryInterface memory, ObjectsTypes type, uint8_t t, uint8_t x, uint8_t y, uint8_t l)
{
    return spawn[type](hardware, memory, t, x, y, l);;
}

/**********************************************************************************************************************/
/** sets the creature position to 0,0
*   sets creature to false on the map array
*   returns the entity id of the creature
**********************************************************************************************************************/
SET_MEMORY(".core")
EntityId CaptureMonster(EntityId id)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    SetBit(g_core.creatures.onMap, id, false);
    Position empty_pos = {.x = 0, .y = 0};
    g_core.creatures.position[id] = empty_pos;
    return id;
}

/**********************************************************************************************************************/
/** sets the item position to 0,0
*   sets item to false on the map array
*   returns the entity id of the item
**********************************************************************************************************************/
SET_MEMORY(".core")
EntityId PickItem(EntityId id)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");

    Position empty_pos = {.x = 0, .y = 0};
    g_core.items.position[id] = empty_pos;
    SetBit(g_core.items.onMap, id, false);
    return id;
}

/**********************************************************************************************************************/
/**Reset all values of the given entity ID
**********************************************************************************************************************/
SET_MEMORY(".map")
void DestroyCreature(HardwareInterface hardware, EntityId id)
{
    Position empty_pos = {.x = 0, .y = 0};
    g_core.creatures.position[id] = empty_pos;
    SetBit(g_core.creatures.onMap, id, false);
    g_core.creatures.types[id] = NO_ENTITY;
    g_core.creatures.metaData[id].unused = NO_ENTITY;
    SetBit(g_core.creatures.alive, id, false);
    g_core.creatures.senses[id].sight = 0;
    g_core.creatures.senses[id].smell = 0;
    g_core.creatures.senses[id].sound = 0;
    g_core.creatures.stealth[id].sight = 0;
    g_core.creatures.stealth[id].sound = 0;
    g_core.creatures.stealth[id].smell = 0;
    g_core.creatures.stats[id].attack = 0;
    g_core.creatures.stats[id].defence = 0;
    g_core.creatures.stats[id].magic = 0;
    g_core.creatures.stats[id].speed = 0;
    hardware.MemSet(g_core.creatures.attacks[id], NO_ABILITY, 8);
    Int999SetCurrent(&g_core.creatures.hp[id], 0);
    Int999SetMax(&g_core.creatures.hp[id], 0);
    g_core.creatures.level[id].value = 0;
    SetBit(g_core.creatures.active, id, false);
}

SET_MEMORY(".map")
void DestroyItem(EntityId id)
{
    Position empty_pos = {.x = 0, .y = 0};
    g_core.items.position[id] = empty_pos;
    SetBit(g_core.items.onMap, id, false);
    g_core.items.types[id] = NO_ENTITY;
    g_core.items.metaData[id].unused = NO_ENTITY;
    SetBit(g_core.items.active, id, false);
}


SET_MEMORY(".map")
void DestroyObject(EntityId id)
{
    Position empty_pos = {.x = 0, .y = 0};
    g_core.objects.position[id] = empty_pos;
    SetBit(g_core.objects.onMap, id, false);
    g_core.objects.types[id] = NO_ENTITY;
    g_core.objects.metaData[id].unused = NO_ENTITY;
    SetBit(g_core.objects.active, id, false);
}

/**********************************************************************************************************************/
/**Copy all values of the given entity ID
**********************************************************************************************************************/
SET_MEMORY(".map")
void CopyCreature(HardwareInterface hardware, EntityId src_id, EntityId target_id)
{
    g_core.creatures.position[target_id] = g_core.creatures.position[src_id];
    g_core.creatures.types[target_id] = g_core.creatures.types[src_id];

    g_core.creatures.metaData[target_id].unused = g_core.creatures.metaData[src_id].unused;
    g_core.creatures.senses[target_id].sight = g_core.creatures.senses[src_id].sight;
    g_core.creatures.senses[target_id].smell = g_core.creatures.senses[src_id].smell;
    g_core.creatures.senses[target_id].sound = g_core.creatures.senses[src_id].sound;
    g_core.creatures.stealth[target_id].sight = g_core.creatures.stealth[src_id].sight;
    g_core.creatures.stealth[target_id].sound = g_core.creatures.stealth[src_id].sound;
    g_core.creatures.stealth[target_id].smell = g_core.creatures.stealth[src_id].smell;
    g_core.creatures.stats[target_id].attack = g_core.creatures.stats[src_id].attack;
    g_core.creatures.stats[target_id].defence = g_core.creatures.stats[src_id].defence;
    g_core.creatures.stats[target_id].magic = g_core.creatures.stats[src_id].magic;
    g_core.creatures.stats[target_id].speed = g_core.creatures.stats[src_id].speed;
    g_core.creatures.level[target_id].value = g_core.creatures.level[src_id].value;

    hardware.MemSet(g_core.creatures.attacks[target_id], NO_ABILITY, 8);
    Int999SetCurrent(&g_core.creatures.hp[target_id], 0);
    Int999SetMax(&g_core.creatures.hp[target_id], 0);

    SetBit(g_core.creatures.alive, target_id, GetBit(g_core.creatures.alive, src_id));
    SetBit(g_core.creatures.onMap, target_id, GetBit(g_core.creatures.onMap, src_id));
    SetBit(g_core.creatures.active, target_id, GetBit(g_core.creatures.active, src_id));
}


SET_MEMORY(".map")
void CopyItem(EntityId src_id, EntityId target_id)
{
    g_core.items.position[target_id] = g_core.items.position[src_id];
    g_core.items.types[target_id] = g_core.items.types[src_id];
    g_core.items.metaData[target_id].unused = g_core.items.metaData[src_id].unused;

    SetBit(g_core.items.onMap, target_id, GetBit(g_core.items.onMap, src_id));
    SetBit(g_core.items.active, target_id, GetBit(g_core.items.active, src_id));
}


void CopyObject(EntityId src_id, EntityId target_id)
{
    g_core.items.position[target_id] = g_core.items.position[src_id];
    g_core.items.types[target_id] = g_core.items.types[src_id];
    g_core.items.metaData[target_id].unused = g_core.items.metaData[src_id].unused;

    SetBit(g_core.items.onMap, target_id, GetBit(g_core.items.onMap, src_id));
    SetBit(g_core.items.active, target_id, GetBit(g_core.items.active, src_id));
}


/**********************************************************************************************************************/
/** Reset all values of all entities on the map
 *  TODO: may add trainers later
**********************************************************************************************************************/
SET_MEMORY(".map")
void ResetEntities(HardwareInterface hardware, MemoryInterface memory, bool copyPlayer)
{
    uint16_t creature_start_idx = 0;
    uint16_t item_start_idx = 0;
    if (copyPlayer)
    {
        creature_start_idx = CachePlayerCreatureData(hardware);
        item_start_idx = CachePlayerItemData();
    }

    for (uint16_t i = creature_start_idx; i < ENTITY_COUNT; ++i)
        DestroyCreature(hardware, i);

    for (uint16_t i = item_start_idx; i < ENTITY_COUNT; ++i)
        DestroyItem(i);

    for (uint16_t i = 0; i < ENTITY_COUNT; ++i)
        DestroyObject(i);

    g_core.creatures.total = 0;
    g_core.items.total = 0;
    g_core.objects.total = 0;
}

/**********************************************************************************************************************/
/** Creates all the creatures on the map from the BIOME and THEME data
**********************************************************************************************************************/
SET_MEMORY(".map.rodata")
static const char str_map_pop_place_player[] = "PopulateLevelCreatures start";
SET_MEMORY(".map.rodata")
static const char str_map_pop_set_fog[] = "PopulateLevelCreatures biome";
SET_MEMORY(".map.rodata")
static const char str_map_pop_camera_init[] = "PopulateLevelCreatures theme";
SET_MEMORY(".map.rodata")
static const char str_map_pop_camera_set[] = "PopulateLevelCreatures DONE";

SET_MEMORY(".map")
void PopulateLevelCreatures(HardwareInterface hardware, MemoryInterface memory)
{
    uint8_t creature_level = g_core.floor;
    for (uint8_t i = 0; i < NUM_BIOME_CREATURES; ++i)
    {
        uint8_t index = hardware.GetRandom_uint8_t(0, BIOME_MONSTER_TYPES);
        const Creature creature = Flash_GetBiomeCreature(memory, g_core.biome, index);
        const Position pos = FindOpenMapLocation(hardware, CREATURE);
        SpawnEntity(hardware, memory, CREATURE, creature, pos.x, pos.y, creature_level);
    }


    for (uint8_t i = 0; i < NUM_THEME_CREATURES; ++i)
    {
        uint8_t index = hardware.GetRandom_uint8_t(0, THEME_MONSTER_TYPES);
        const Creature creature = Flash_GetThemeCreature(memory, g_core.theme, index);
        const Position pos = FindOpenMapLocation(hardware, CREATURE);
        SpawnEntity(hardware, memory, CREATURE, creature, pos.x, pos.y, creature_level);
    }

    for (uint8_t i = 0; i < g_core.creatures.total; ++i)
        if (GetBit(g_core.creatures.onMap, i) && GetBit(g_core.creatures.alive, i))
            g_core.creatures.newPosition[i] = g_core.creatures.position[i];
}

SET_MEMORY(".map")
void PopulateLevelItems(HardwareInterface hardware, MemoryInterface memory)
{
    uint8_t creature_level = 1;
    for (uint8_t i = 0; i < NUM_MAP_ITEMS; ++i)
    {
        const ItemTypes item_type = hardware.GetRandom_uint8_t(0, ITEM_COUNT);
        const Position pos = FindOpenMapLocation(hardware, ITEM);
        SpawnEntity(hardware, memory, ITEM, item_type, pos.x, pos.y, creature_level);
    }
}

SET_MEMORY(".map")
void PopulateLevelObjects(HardwareInterface hardware, MemoryInterface memory)
{
    ASSERT(g_run.objects.total == 0, "Objects already populated! %d", g_run.objects.total);
    uint8_t creature_level = 1;
    for (uint8_t i = 0; i < NUM_MAP_OBJECTS; ++i)
    {
        const Object object_type = hardware.GetRandom_uint8_t(0, OBJECT_COUNT);
        const Position pos = FindOpenMapLocation(hardware, OBJECT);
        SpawnEntity(hardware, memory, OBJECT, object_type, pos.x, pos.y, creature_level);
    }
}

/**********************************************************************************************************************/
/** Fill typeIDs[] with the types of the given e_ids[] array
 *  ON SUCCESS - Returns the type ID of the given entity ID
 *  ON FAIL - sets typeIDs to NULL and returns NULL
**********************************************************************************************************************/
SET_MEMORY(".map")
void GetEntityTypes(MemoryInterface memory, uint8_t* typeIDs, const uint8_t* e_ids, ObjectsTypes type, uint8_t n, uint8_t offset)
{
    SmallStringArray text[MAX_MENU_SIZE];
    uint8_t i = offset;

    switch (type)
    {
    case CREATURE:
        {
            while (i < n)
            {
                typeIDs[i] = GetCreatureType(e_ids[i]);
                Flash_GetCreatureName(memory, g_core.menu.text[i], typeIDs[i]);
                i++;
            }
            g_core.menu.text[i][0] = '\0';
            break;
        }
    case ITEM:
        {
            while (i < n)
            {
                typeIDs[i] = GetItemType(e_ids[i]);
                Flash_GetItemName(memory, g_core.menu.text[i], typeIDs[i]);
                i++;
            }
            g_core.menu.text[i][0] = '\0';
            break;
        }
    case OBJECT:
        {
            while (i < n)
            {
                typeIDs[i] = GetObjectType(e_ids[i]);
                Flash_GetObjectName(memory, g_core.menu.text[i], typeIDs[i]);
                i++;
            }
            g_core.menu.text[i][0] = '\0';
            break;
        }
    default:
        {
            typeIDs = NULL;
        }
    }
}


/**********************************************************************************************************************/
/** Returns the type ID of the given entity ID
**********************************************************************************************************************/
ItemTypes GetItemType(EntityId id)
{
    if (id == NO_ENTITY)
        return NO_ITEM;

    return g_core.items.types[id];
}


SET_MEMORY(".map")
Object GetObjectType(EntityId id)
{
    if (id == NO_ENTITY)
        return NO_OBJECT;

    return g_core.objects.types[id];
}


SET_MEMORY(".core")
Creature GetCreatureType(EntityId id)
{
    if (id == NO_ENTITY)
        return NO_CREATURE;

    return g_core.creatures.types[id];
}


/**********************************************************************************************************************/
/** Returns the array of active objects
**********************************************************************************************************************/
uint8_t* GetActiveObjects(void)
{
    return g_core.creatures.active;
}

/**********************************************************************************************************************/
/**Returns true if the given entity is Alive
**********************************************************************************************************************/
bool CheckAlive(EntityId id)
{
    ASSERT(id != NO_CREATURE, "ID is NO_CREATURE it is invalid!");
    return GetBit(g_core.creatures.active, id);
}

/**********************************************************************************************************************/
/** Checks whether is given entity ID can detect the target entity ID
 * ON SUCCESS - returns true
 * ON FAIL - returns false
**********************************************************************************************************************/
SET_MEMORY(".map")
bool InDetectionRange(EntityId id, EntityId targetID)
{
    uint8_t smell = g_core.creatures.senses[id].smell;
    uint8_t smell_stealth = g_core.creatures.stealth[targetID].smell;

    if (smell > smell_stealth)
        return true;

    uint8_t sound = g_core.creatures.senses[id].sound;
    uint8_t sound_stealth = g_core.creatures.stealth[targetID].sound;

    if (sound > sound_stealth)
        return true;

    uint8_t sight = g_core.creatures.senses[id].sight;
    uint8_t sight_stealth = g_core.creatures.stealth[targetID].sight;

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
