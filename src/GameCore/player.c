//
// Created by nathanial on 2/21/26.
//
#include "stdint.h"

#include "player.h"

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
void InitPlayer(void)
{
    g_run.player.currentBagSize = DEFAULT_BAG_SIZE;
    g_run.player.currentSpellbookSize = DEFAULT_SPELLBOOK_SIZE;

    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        g_run.player.partyID[i] = NO_ENTITY;

    for (uint8_t i = 0; i < MAX_BAG_SIZE; ++i)
        g_run.player.itemID[i] = NO_ENTITY;

    for (uint8_t i = 0; i < MAX_SPELLBOOK_SIZE; ++i)
        g_run.player.spellID[i] = NO_SPELL;

    g_run.player.spellID[0] = HEAL;
    g_run.player.spellID[1] = DESCEND;

    Position pos = FindOpenMapLocation(CREATURE);
    uint8_t x = pos.x;
    uint8_t y = pos.y;

    g_run.player.id = SpawnEntity(CREATURE, HUMAN, x, y, 0);
    g_run.creatures.speed[g_run.player.id].max = 99;
    g_run.creatures.speed[g_run.player.id].current = 15;


    EntityId e_id;
    e_id = SpawnEntity(CREATURE, AMMIT, x, y, 5);
    PlayerCaptureMonster(e_id);
    e_id = SpawnEntity(ITEM, HEALTH_POTION, x, y, 0);
    PlayerPickItem(e_id);

    for (uint16_t j = y - 5; j < y + 5; ++j)
        for (uint16_t i = x - 5; i < x + 5; ++i)
            SetFog(i, j, false);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
EntityId CachePlayerCreatureData()
{
    EntityId creature_idx = 0;
    //set player to beginning of the array
    if (GetPlayerID() != 0)
    {
    }
    creature_idx++;

    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
    {
        if (g_run.player.partyID[i] != NO_ENTITY && g_run.player.partyID[i] > creature_idx)
        {
            CopyCreature(g_run.player.partyID[i], creature_idx);
            g_run.player.partyID[i] = creature_idx;
            creature_idx++;
        }
    }

    //set player creatures to beginning of the array
    return creature_idx;
}

EntityId CachePlayerItemData()
{
    EntityId item_idx = 0;
    EntityId sorted_indexes[MAX_BAG_SIZE];
    SortEntityArray(sorted_indexes, g_run.player.itemID, MAX_BAG_SIZE);

    for (uint8_t i = 0; i < MAX_BAG_SIZE; ++i)
    {
        if (sorted_indexes[i] != NO_ENTITY && sorted_indexes[i] != item_idx)
        {
            CopyItem(g_run.player.itemID[i], item_idx);
            g_run.player.itemID[i] = item_idx;
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
EntityId PlayerCaptureMonster(EntityId e_id)
{
    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        if (g_run.player.partyID[i] == NO_ENTITY)
        {
            g_run.player.partyID[i] = CaptureMonster(e_id);
            g_run.player.cur_xp[i] = 0;
            g_run.player.tar_xp[i] = 100; //TODO: calculate needed xp to level
            return e_id;
        }

    return e_id;
}

/**********************************************************************************************************************/
/** Searches the bag to find an open slot to pick up an item
 *  ON SUCCESS - adds item id to the party array
*   ON FAIL - TODO - add a fail state (item cannot be picked up)
**********************************************************************************************************************/
EntityId PlayerPickItem(EntityId e_id)
{
    for (uint8_t i = 0; i < MAX_BAG_SIZE; ++i)
        if (g_run.player.itemID[i] == NO_ENTITY)
        {
            g_run.player.itemID[i] = PickItem(e_id);
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
Position GetPlayerPosition(void)
{
    return GetEntityPosition(CREATURE, g_run.player.id);
}

/**********************************************************************************************************************/
/**Sets player Position value to NewPosition value
 * stores the player delta value into the scroll value
 * clears the player delta value
**********************************************************************************************************************/
void UpdatePlayerPosition(void)
{
    Position pos = GetPlayerPosition();
    uint8_t x = pos.x + g_run.player.d.x;
    uint8_t y = pos.y + g_run.player.d.y;
    QueueObjectMovePosition(g_run.player.id, x, y);
    g_run.player.scroll.x = g_run.player.d.x;
    g_run.player.scroll.y = g_run.player.d.y;
    g_run.player.d.x = 0;
    g_run.player.d.y = 0;
}

/**********************************************************************************************************************/
/**Set the player movement delta
 * -1 or 0 or 1 for single cell movement
**********************************************************************************************************************/
Delta SetPlayerDelta(Delta newDelta)
{
    g_run.player.d = newDelta;
    return g_run.player.d;
}

/**********************************************************************************************************************/
/**Returns the player ID
**********************************************************************************************************************/
EntityId GetPlayerID(void)
{
    return g_run.player.id;
}

/**********************************************************************************************************************/
/**Returns the player creature type id
**********************************************************************************************************************/
uint8_t GetPlayerType(void)
{
    return GetCreatureType(g_run.player.id);
}

/**********************************************************************************************************************/
/**Returns the player's bag array
**********************************************************************************************************************/
EntityId* GetPlayerItemsIDs(void)
{
    return g_run.player.itemID;
}

/**********************************************************************************************************************/
/**Returns the player's party array
**********************************************************************************************************************/
EntityId* GetPlayerMonsterIDs(void)
{
    return g_run.player.partyID;
}

/**********************************************************************************************************************/
/**Checks if the given entity id is currently in the played party array
**********************************************************************************************************************/
bool IsInParty(EntityId id)
{
    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        if (g_run.player.partyID[i] == id)
            return true;
    return false;
}

/**********************************************************************************************************************/
/**Destroys the item entity
 * Sets the bag index of the item to NO_ITEM
**********************************************************************************************************************/
void ConsumeItem(uint8_t idx, EntityId e_id)
{
    DestroyItem(e_id);
    g_run.player.itemID[idx] = NO_ENTITY;
}

/**********************************************************************************************************************/
/**Checks if the player has lost the game (you have now lost the game)
 * ON SUCCESS -
 * ON FAIL -
**********************************************************************************************************************/
bool PlayerDefeated(void)
{
    if (CheckAlive(g_run.battleMode.playerMonsterID))
        return false;

    for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
    {
        uint8_t e_id = g_run.player.partyID[i];
        if (GetCreatureType(e_id) != NO_CREATURE && CheckAlive(e_id))
        {
            DEBUG("----set new active %d %d", e_id, GetCreatureType(e_id));
            g_run.battleMode.playerMonsterID = e_id;
            return false;
        }
    }

    return true;
}


/**********************************************************************************************************************/
/**
**********************************************************************************************************************/

void DestroyPlayerCreature()
{
    EntityId player_creature_id = g_run.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_run.battleMode.enemyMonsterID;
    GainXP(player_creature_id, ai_creature_id);
    DestroyCreature(ai_creature_id);
}

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/

void DestroyEnemyCreature()
{
    EntityId player_creature_id = g_run.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_run.battleMode.enemyMonsterID;
    GainXP(player_creature_id, ai_creature_id);
    DestroyCreature(ai_creature_id);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckBattleEnd(EntityId attackerID, EntityId defenderID)
{
    uint8_t hp = Int999GetCurrent(&g_run.creatures.hp[defenderID]);
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
    EntityId player_creature_id = g_run.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_run.battleMode.enemyMonsterID;
    return CheckBattleEnd(player_creature_id, ai_creature_id);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckEnemyAttackOutcome()
{
    EntityId player_creature_id = g_run.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_run.battleMode.enemyMonsterID;
    CheckBattleEnd(ai_creature_id, player_creature_id);
    return PlayerDefeated();
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void PlacePlayerOnMap()
{
    Position pos = FindOpenMapLocation(CREATURE);
    uint8_t x = pos.x;
    g_run.creatures.position[g_run.player.id].x = x;
    uint8_t y = pos.y;
    g_run.creatures.position[g_run.player.id].y = y;
}
