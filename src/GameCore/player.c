//
// Created by nathanial on 2/21/26.
//
#include "player.h"

#include "battles.h"
#include "map.h"
#include "memory_ram.h"
#include "entities.h"
#include "lib_debugging.h"

#include "utils.h"
#include "stats.h"

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/

EntityId PlayerCaptureMonster(EntityId id);
EntityId PlayerPickItem(EntityId id);


/**********************************************************************************************************************/
/** Initializes player data
 *  Creates default creatures for party
 *  Creates default items for party
 *  Places default spells in spellbook
 *  Sets player to a random empty cell on the map
 *  sets sight rango around player
**********************************************************************************************************************/
SET_MEMORY(".map.rodata")
static const char str_init_player_start[] = "InitPlayer start";

SET_MEMORY(".map.rodata")
static const char str_init_player_place[] = "Place player";

SET_MEMORY(".map.rodata")
static const char str_init_player_spawn[] = "spawn player";

SET_MEMORY(".map.rodata")
static const char str_init_player_party[] = "create party 1";

SET_MEMORY(".map.rodata")
static const char str_init_player_capture[] = "capture";

SET_MEMORY(".map.rodata")
static const char str_init_player_item[] = "create item";

SET_MEMORY(".map.rodata")
static const char str_init_player_picp_up[] = "InitPlayer pick up";

SET_MEMORY(".map.rodata")
static const char str_init_player_fog[] = "InitPlayer set fog";

SET_MEMORY(".map.rodata")
static const char str_init_player_DONE[] = "InitPlayer DONE";

SET_MEMORY(".map")
void InitPlayer(HardwareInterface hardware, MemoryInterface memory)
{
    hardware.Print(str_init_player_start);

    g_core.player.currentBagSize = DEFAULT_BAG_SIZE;
    g_core.player.currentSpellbookSize = DEFAULT_SPELLBOOK_SIZE;

    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        g_core.player.partyID[i] = NO_ENTITY;

    for (uint8_t i = 0; i < MAX_BAG_SIZE; ++i)
        g_core.player.itemID[i] = NO_ENTITY;

    for (uint8_t i = 0; i < MAX_SPELLBOOK_SIZE; ++i)
        g_core.player.spellID[i] = NO_SPELL;

    g_core.player.spellID[0] = HEAL;
    g_core.player.spellID[1] = DESCEND;

    hardware.Print(str_init_player_place);

    Position pos = FindOpenMapLocation(hardware, CREATURE);
    uint8_t x = pos.x;
    uint8_t y = pos.y;

    hardware.Print(str_init_player_spawn);

    g_core.player.id = SpawnEntity(hardware, memory, CREATURE, HUMAN, x, y, 0);

    g_core.creatures.speed[g_core.player.id].max = 99;
    g_core.creatures.speed[g_core.player.id].current = 15;

    hardware.Print(str_init_player_party);

    EntityId e_id;
    e_id = SpawnEntity(hardware, memory, CREATURE, LAMIA, x, y, 5);

    hardware.Print(str_init_player_capture);
    PlayerCaptureMonster(e_id);

    hardware.Print(str_init_player_item);
    e_id = SpawnEntity(hardware, memory, ITEM, HEALTH_POTION, x, y, 0);

    hardware.Print(str_init_player_picp_up);
    PlayerPickItem(e_id);


    hardware.Print(str_init_player_fog);
    for (uint16_t j = y - 5; j < y + 5; ++j)
        for (uint16_t i = x - 5; i < x + 5; ++i)
            SetFog(i, j, false);

    hardware.Print(str_init_player_DONE);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
EntityId CachePlayerCreatureData(HardwareInterface hardware)
{
    EntityId creature_idx = 0;
    //set player to beginning of the array
    if (GetPlayerID() != 0)
    {
    }
    creature_idx++;

    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
    {
        if (g_core.player.partyID[i] != NO_ENTITY && g_core.player.partyID[i] > creature_idx)
        {
            CopyCreature(hardware, g_core.player.partyID[i], creature_idx);
            g_core.player.partyID[i] = creature_idx;
            creature_idx++;
        }
    }

    //set player creatures to beginning of the array
    return creature_idx;
}

SET_MEMORY(".map")
EntityId CachePlayerItemData()
{
    EntityId item_idx = 0;
    EntityId sorted_indexes[MAX_BAG_SIZE];
    SortEntityArray(sorted_indexes, g_core.player.itemID, MAX_BAG_SIZE);

    for (uint8_t i = 0; i < MAX_BAG_SIZE; ++i)
    {
        if (sorted_indexes[i] != NO_ENTITY && sorted_indexes[i] != item_idx)
        {
            CopyItem(g_core.player.itemID[i], item_idx);
            g_core.player.itemID[i] = item_idx;
            item_idx++;
        }
    }

    //set player items to beginning of the array
    return item_idx;
}

/**********************************************************************************************************************/
/** Searches the party to find an open slot to capture a creature
 *  ON SUCCESS - adds creature id to the party array
 *             - sets xp for the creature
*   ON FAIL - TODO - add a fail state (creature cannot be captured)
**********************************************************************************************************************/
SET_MEMORY(".core")
EntityId PlayerCaptureMonster(EntityId e_id)
{
    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        if (g_core.player.partyID[i] == NO_ENTITY)
        {
            g_core.player.partyID[i] = CaptureMonster(e_id);
            g_core.player.cur_xp[i] = 0;
            g_core.player.tar_xp[i] = 100; //TODO: calculate needed xp to level
            return e_id;
        }

    return e_id;
}

/**********************************************************************************************************************/
/** Searches the bag to find an open slot to pick up an item
 *  ON SUCCESS - adds item id to the party array
*   ON FAIL - TODO - add a fail state (item cannot be picked up)
**********************************************************************************************************************/
SET_MEMORY(".core")
EntityId PlayerPickItem(EntityId e_id)
{
    if (e_id == NO_ENTITY) return e_id;
    for (uint8_t i = 0; i < MAX_BAG_SIZE; ++i)
        if (g_core.player.itemID[i] == NO_ENTITY)
        {
            g_core.player.itemID[i] = PickItem(e_id);
            return e_id;
        }

    return e_id;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
//TODO: sort items by type and alphabetical order
void SortItems(void)
{
}


/**********************************************************************************************************************/
/** Returns the player position as map cell index
**********************************************************************************************************************/
SET_MEMORY(".map")
Position GetPlayerPosition(void)
{
    return GetEntityPosition(CREATURE, g_core.player.id);
}

/**********************************************************************************************************************/
/**Sets player Position value to NewPosition value
 * stores the player delta value into the scroll value
 * clears the player delta value
**********************************************************************************************************************/
SET_MEMORY(".map")
void UpdatePlayerPosition(void)
{
    Position pos = GetPlayerPosition();
    uint8_t x = pos.x + g_core.player.d.x;
    uint8_t y = pos.y + g_core.player.d.y;
    QueueObjectMovePosition(g_core.player.id, x, y);
    g_core.player.scroll.x = g_core.player.d.x;
    g_core.player.scroll.y = g_core.player.d.y;
    g_core.player.d.x = 0;
    g_core.player.d.y = 0;
}

/**********************************************************************************************************************/
/**Set the player movement delta
 * -1 or 0 or 1 for single cell movement
**********************************************************************************************************************/
SET_MEMORY(".map")
Delta SetPlayerDelta(Delta newDelta)
{
    g_core.player.d = newDelta;
    return g_core.player.d;
}

/**********************************************************************************************************************/
/**Returns the player ID
**********************************************************************************************************************/
SET_MEMORY(".map")
EntityId GetPlayerID(void)
{
    return g_core.player.id;
}

/**********************************************************************************************************************/
/**Returns the player creature type id
**********************************************************************************************************************/
uint8_t GetPlayerType(void)
{
    return GetCreatureType(g_core.player.id);
}

/**********************************************************************************************************************/
/**Returns the player's bag array
**********************************************************************************************************************/
EntityId* GetPlayerItemsIDs(void)
{
    return g_core.player.itemID;
}

/**********************************************************************************************************************/
/**Returns the player's party array
**********************************************************************************************************************/
SET_MEMORY(".core")
EntityId* GetPlayerMonsterIDs(void)
{
    return g_core.player.partyID;
}

/**********************************************************************************************************************/
/**Checks if the given entity id is currently in the played party array
**********************************************************************************************************************/
bool IsInParty(EntityId id)
{
    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        if (g_core.player.partyID[i] == id)
            return true;
    return false;
}

/**********************************************************************************************************************/
/**Destroys the item entity
 * Sets the bag index of the item to NO_ITEM
**********************************************************************************************************************/
SET_MEMORY(".map")
void ConsumeItem(uint8_t idx, EntityId e_id)
{
    DestroyItem(e_id);
    g_core.player.itemID[idx] = NO_ENTITY;
}

/**********************************************************************************************************************/
/**Checks if the player has lost the game (you have now lost the game)
 * ON SUCCESS -
 * ON FAIL -
**********************************************************************************************************************/
bool PlayerDefeated(void)
{
    if (CheckAlive(g_core.battleMode.playerMonsterID))
        return false;

    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
    {
        uint8_t e_id = g_core.player.partyID[i];
        if (GetCreatureType(e_id) != NO_CREATURE && CheckAlive(e_id))
        {
            g_core.battleMode.playerMonsterID = e_id;
            return false;
        }
    }

    return true;
}


/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
SET_MEMORY(".core")
void DestroyPlayerCreature(HardwareInterface hardware)
{
    EntityId player_creature_id = g_core.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_core.battleMode.enemyMonsterID;
    GainXP(player_creature_id, ai_creature_id);
    DestroyCreature(hardware, ai_creature_id);
}

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
SET_MEMORY(".core")
void DestroyEnemyCreature(HardwareInterface hardware)
{
    EntityId player_creature_id = g_core.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_core.battleMode.enemyMonsterID;
    GainXP(player_creature_id, ai_creature_id);
    DestroyCreature(hardware, ai_creature_id);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckBattleEnd(EntityId attackerID, EntityId defenderID)
{
    uint8_t hp = Int999GetCurrent(&g_core.creatures.hp[defenderID]);
    if (hp == 0)
    {
        return false;
    }
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckPlayerAttackOutcome()
{
    EntityId player_creature_id = g_core.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_core.battleMode.enemyMonsterID;
    return CheckBattleEnd(player_creature_id, ai_creature_id);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckEnemyAttackOutcome()
{
    EntityId player_creature_id = g_core.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_core.battleMode.enemyMonsterID;
    CheckBattleEnd(ai_creature_id, player_creature_id);
    return PlayerDefeated();
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
void PlacePlayerOnMap(HardwareInterface hardware)
{
    Position pos = FindOpenMapLocation(hardware, CREATURE);
    g_core.creatures.position[g_core.player.id].x = pos.x;
    g_core.creatures.position[g_core.player.id].y = pos.y;
}

/**********************************************************************************************************************/
/*  interact with item in player's cell
/*  interact with object in player's cell
**********************************************************************************************************************/
SET_MEMORY(".map")
void PlayerInteractItemInCell()
{
    Position pos = GetPlayerPosition();
    EntityId item_id = CheckTileForEntity(ITEM, g_core.player.id, pos);
    PlayerPickItem(item_id);
}

SET_MEMORY(".map")
void PlayerInteractObjectInCell(MemoryInterface memory, HardwareInterface hardware)
{
    Position pos = GetPlayerPosition();
    EntityId object_id = CheckTileForEntity(OBJECT, g_core.player.id, pos);
    InteractObject(memory, hardware, object_id, g_core.player.id);
}
