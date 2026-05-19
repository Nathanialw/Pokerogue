//
// Created by nathanial on 2/23/26.
//

#include "core.h"
#include "lib_debugging.h"
#include "lib_constants.h"

#include "ai.h"
#include "camera.h"
#include "collision.h"
#include "map.h"
#include "entities.h"
#include "memory_access.h"
#include "player.h"
#include "status_effects.h"
#include "utils.h"


/**********************************************************************************************************************/
/** Runs All game init functions in order
 *      -> Game data
 *      -> Sets entities to default state
 *      -> Creates the map
 *      -> inits player data
 *      -> creates map entities
 *      -> sets camera to player position
**********************************************************************************************************************/


SET_MEMORY(".map_gen.rodata")
static const char init_gmae[] = "init_gmae";
SET_MEMORY(".map_gen.rodata")
static const char init_map[] = "init_map";
SET_MEMORY(".map_gen.data")
static char test_str[SMALL_STRINGS];


SET_MEMORY(".map_gen")
void InitGame(HardwareInterface hardware, MemoryInterface memory)
{
    hardware.Print(init_gmae);
    GameRunInit();

    hardware.Print(init_map);
    InitMap(hardware);


}

/**********************************************************************************************************************/
/** Runs All game init functions in order
 *      -> Sets entities to default state
 *      -> Clears the map
 *      -> Creates the map
 *      -> creates map entities
 *      -> sets camera to player position
**********************************************************************************************************************/
//  Generate a new map and objects, set camera
void NewMap(void)
{
    // DEBUG("new map");
    // ResetEntities(false);
    // InitMap();
    // PopulateLevelCreatures();
    // SetCameraPlayer();
}

/**********************************************************************************************************************/
/** Initializes title screen data
 *  TODO: NOT YET IMPLEMENTED
**********************************************************************************************************************/
void InitTitleScreen(void)
{
}

/**********************************************************************************************************************/
/** Updates player position first
 *  Checks for battle
 *  Iterates through all entities, run the AI function to update their position
**********************************************************************************************************************/
SET_MEMORY(".map.rodata")
const char test07[] = "    EntityId p_id = GetPlayerID();\n";

SET_MEMORY(".map.rodata")
const char test08[] = "    for (uint8_t id = 0; id < ENTITY_COUNT; ++id)\n";

SET_MEMORY(".map.rodata")
const char test09[] = " for (uint8_t id = 0; id < ENTITY_COUNT; ++id)\n";

SET_MEMORY(".map.rodata")
const char test10[] = "    if (combat_id != NO_ENTITY)\n";

SET_MEMORY(".map.rodata")
const char test11[] = "    UpdatePlayerPosition();\n";

SET_MEMORY(".map.rodata")
const char test12[] = " ObjectCollision(creature_id)\n";

SET_MEMORY(".map.rodata")
const char test13[] = "SetCameraPlayer\n";

SET_MEMORY(".map.rodata")
const char test14[] = "SetCameraPlayer\n";

SET_MEMORY(".map.rodata")
const char test15[] = "SetCameraPlayer\n";

SET_MEMORY(".map")
bool UpdatePositions(HardwareInterface hardware)
{
    hardware.Print(test07);

    EntityId p_id = GetPlayerID();
    IntMax99* speed = GetCreatureSpeeds();
    EntityId combat_id = NO_ENTITY;
    uint8_t player_speed = speed[p_id].current; //awkwardly as my speed value decreases my speed goes 'up', I may need to rethink this
    uint8_t* onMap = GetEntitiesOnMap(CREATURE);

    hardware.Print(test08);

    for (uint8_t id = 0; id < ENTITY_COUNT; ++id)
    {
        if (!GetBit(onMap, id)) continue; //need to make sure this only triggers for dynamic map entities
        if (id == p_id) continue;

        uint8_t max = speed[id].max;
        uint8_t cur = speed[id].current;

        if (cur + player_speed < max)
        {
            speed[id].current += player_speed;
            g_core.creatures.newPosition[id] = g_core.creatures.position[id];
            continue;
        }

        speed[id].current = player_speed - (max - cur);
        CreatureAI(hardware, id);

        hardware.Print(test07);
    }

    hardware.Print(test09);

    for (uint8_t id = 0; id < ENTITY_COUNT; ++id)
    {
        if (!GetBit(onMap, id)) continue;
        if (id == p_id) continue;
        EntityId collision_id = CheckCollision(id);
        if (collision_id == p_id) combat_id = id;
    }

    hardware.Print(test10);
    if (combat_id != NO_ENTITY)
    {
        ObjectCollision(combat_id);
        return false;
    }

    hardware.Print(test11);
    UpdatePlayerPosition();
    uint8_t creature_id = CheckCollision(p_id);
    if (creature_id != NO_ENTITY && creature_id != p_id)
    {
        hardware.Print(test12);
        ObjectCollision(creature_id);
        return false;
    }

    return true;
}

/**********************************************************************************************************************/
/**Iterates through all entities sets position to the queued position
**********************************************************************************************************************/
SET_MEMORY(".map")
void SetPositions(void)
{
    uint8_t* onMap = GetEntitiesOnMap(CREATURE); //array is 256 bytes
    Position* position = GetEntityPositions(CREATURE); //array is 512 bytes
    Position* newPosition = GetEntityNewPositions(); //array is 512 bytes

    // DEBUG("Updating Object Positions");

    for (uint8_t id = 0; id < ENTITY_COUNT; ++id)
    {
        if (!GetBit(onMap, id)) continue;

        Position pos = position[id];
        Position nPos = newPosition[id];

        uint8_t x = pos.x;
        uint8_t y = pos.y;

        uint8_t nx = nPos.x;
        uint8_t ny = nPos.y;

        //check current tile
        uint8_t tileID = GetMapTile(x, y);
        CheckInteraction(tileID, id, x, y);

        //check next tile
        tileID = GetMapTile(nx, ny);
        if (CheckInteraction(tileID, id, nx, ny))
            SetEntityPosition(CREATURE, id, x, y, nx, ny);
    }

    // DEBUG("Updating Object done");
}


SET_MEMORY(".map.data")
const char test05[] = "UpdateObjectStatusEffects\n";

SET_MEMORY(".map.data")
const char test06[] = "UpdatePositions\n";

SET_MEMORY(".map.data")
const char test03[] = "SetPositions\n";

SET_MEMORY(".map.data")
const char test04[] = "SetCameraPlayer\n";

/**********************************************************************************************************************/
/** Main update
 *  call every frame
**********************************************************************************************************************/
SET_MEMORY(".map")
void UpdateGame(HardwareInterface hardware)
{
    hardware.Print(test05);
    UpdateObjectStatusEffects(hardware);

    hardware.Print(test06);
    if (UpdatePositions(hardware))
    {
        hardware.Print(test03);
        SetPositions();
    }

    hardware.Print(test04);
    SetCameraPlayer();
}
