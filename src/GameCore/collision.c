//
// Created by nathanial on 2/21/26.
//

#include "collision.h"

#include "lib_debugging.h"
#include "memory_ram.h"
#include "menu_battle.h"

/**********************************************************************************************************************/
/*
 *  This file handles map tile interaction logic
 *
**********************************************************************************************************************/
typedef bool (*Interaction)(EntityId, uint8_t x, uint8_t y);


bool NoInteraction(EntityId id, uint8_t x, uint8_t y);
bool Wall(EntityId id, uint8_t x, uint8_t y);
bool Foliage(EntityId id, uint8_t x, uint8_t y);
bool Water(EntityId id, uint8_t x, uint8_t y);
bool Lava(EntityId id, uint8_t x, uint8_t y);
bool Acid(EntityId id, uint8_t x, uint8_t y);


const Interaction TileCollision[NUM_TILES] =
{
    NoInteraction,
    Wall,
    Wall,
    Foliage,
    NoInteraction,
    NoInteraction,
    NoInteraction,
    NoInteraction,
    NoInteraction,
    NoInteraction,
    NoInteraction,
    NoInteraction,
    NoInteraction,
    Water,
    Acid,
    Lava,
};

/**********************************************************************************************************************/
/** Space is empty, free to move into
**********************************************************************************************************************/
bool NoInteraction(EntityId id, uint8_t x, uint8_t y)
{
    return true;
}

/**********************************************************************************************************************/
/** Space is blocked, no movement
**********************************************************************************************************************/
bool Wall(EntityId id, uint8_t x, uint8_t y)
{
    // check current tile, cancel movement

    if (id == g_run.player.id)
    {
        g_run.player.scroll.x = 0;
        g_run.player.scroll.y = 0;
    }


    return false;
}

/**********************************************************************************************************************/
/** Space is blocked, no movement
**********************************************************************************************************************/
bool Foliage(EntityId id, uint8_t x, uint8_t y)
{
    // slow move speed, move speed not implemented yet
    return true;
}

/**********************************************************************************************************************/
/** Space is water
 *  TODO: add drift mechanics
 *  TODO: add drowning mechanics
**********************************************************************************************************************/
bool Water(EntityId id, uint8_t x, uint8_t y)
{
    // check for damage and position drift
    return true;
}

/**********************************************************************************************************************/
/** Space is lava
 *  TODO: add drift mechanics
 *  TODO: add flame damage mechanics
**********************************************************************************************************************/
bool Lava(EntityId id, uint8_t x, uint8_t y)
{
    // check for damage and position drift
    return true;
}

/**********************************************************************************************************************/
/** Space is acid
 *  TODO: add drift mechanics
 *  TODO: add Toxic damage mechanics
 *  TODO: add item melting mechanics
**********************************************************************************************************************/
bool Acid(EntityId id, uint8_t x, uint8_t y)
{
    // check for damage, item melting and position drift
    return true;
}

/**********************************************************************************************************************/
/** Main tile interaction entry point
**********************************************************************************************************************/
bool CheckInteraction(uint8_t tile, EntityId id, uint8_t x, uint8_t y)
{
    return TileCollision[tile](id, x, y);
}

/**********************************************************************************************************************/
/** Triggers the battle state
**********************************************************************************************************************/
void ObjectCollision(EntityId id)
{
    g_run.battleMode.playerMonsterID = g_run.player.partyID[0];
    g_run.battleMode.enemyMonsterID = id;

    InitBattleMenu();
    DEBUG("IN BATTLE");
}
