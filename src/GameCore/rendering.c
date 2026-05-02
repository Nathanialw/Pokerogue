//
// Created by nathanial on 4/11/26.
//

#include "stdbool.h"
#include <string.h>

#include "rendering.h"
#include "animation.h"
#include "camera.h"

#include "map.h"
#include "memory_ram.h"
#include "utils.h"
#include "graphics.h"
#include "lib_debugging.h"


/**********************************************************************************************************************/
/**  Redraws all map tiles and entities ion the camera view to the screen
**********************************************************************************************************************/
void FullRedraw()
{
    Camera cam = GetCamera();

    DEBUG("FullRedraw()");
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        uint16_t my = cam.y + sy;
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            DEBUG("FullRedraw() - Drawing tile %d %d", sx, sy);
            uint16_t mx = cam.x + sx;
            uint16_t id = GetMapTile(mx, my);
            DrawTile(sx, sy, id);
            g_run.viewTiles[sy][sx] = id;
        }
    }

    DEBUG("FullRedraw() - Drawing monsters");
    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
    {
        uint8_t x = g_run.creatures.position[i].x;
        uint8_t y = g_run.creatures.position[i].y;
        if (GetBit(g_run.creatures.onMap, i) && CameraContains(x, y))
        {
            DEBUG("FullRedraw() - Drawing monster %d", i);
            uint8_t rx = (x - cam.x);
            uint8_t ry = (y - cam.y);
            DEBUG("FullRedraw() - Drawing monster %d %d", rx, ry);
            DrawMonster(rx, ry, g_run.creatures.types[i]);
            DEBUG("FullRedraw() - Drawing monster %d", g_run.creatures.types[i]);
            g_run.viewObjects[ry][rx] = g_run.creatures.types[i];
        }
    }
}


/**********************************************************************************************************************/
/** Clears the dirtyTiles array to false
 * Clears the newSprites array to NO_CREATURE
**********************************************************************************************************************/
void ResetRenders(void)
{
    memset(g_run.dirtyTiles, 0, sizeof(g_run.dirtyTiles));

    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
            g_run.newSprites[sy][sx] = NO_CREATURE;
}

/**********************************************************************************************************************/
/** Checks whether the tiles on the newTiles array has changed
 * Save changes as dirty bool in the dirtyTiles array
**********************************************************************************************************************/
void CheckForTileChanges(Camera cam)
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        uint16_t my = cam.y + sy;
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            uint16_t mx = cam.x + sx;
            uint8_t tile = GetMapTile(mx, my);
            if (g_run.viewTiles[sy][sx] != tile)
                g_run.dirtyTiles[sy][sx] = true;

            g_run.viewTiles[sy][sx] = tile; //cache
        }
    }
}

/**********************************************************************************************************************/
/**  Retrieves all entities in the camera view
 *  Saves their types into newSprites array at the reletive tile position
**********************************************************************************************************************/
void GetObjectsInView(Camera cam)
{
    uint8_t id = 0;
    while (1)
    {
        if (GetBit(g_run.creatures.onMap, id))
        {
            uint8_t x = g_run.creatures.position[id].x;
            uint8_t y = g_run.creatures.position[id].y;
            if (CameraContains(x, y))
            {
                uint8_t sx = (x - cam.x);
                uint8_t sy = (y - cam.y);
                g_run.newSprites[sy][sx] = g_run.creatures.types[id];
                DEBUG("-----------------------GetObjectsInView() %d %d %d", id, sx, sy);
            }
        }

        id++;
        if (id == ENTITY_COUNT) break;
    }
}

/**********************************************************************************************************************/
/** Checks whether the sprites in the viewObjects array has changed
 * Save changes as dirty bool in the dirtyTiles array
**********************************************************************************************************************/
void SetDirty(void)
{
    for (uint8_t sy = 0; sy < VIEW_TH; sy++)
    {
        for (uint8_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (g_run.viewObjects[sy][sx] != g_run.newSprites[sy][sx])
                g_run.dirtyTiles[sy][sx] = true;

            g_run.viewObjects[sy][sx] = g_run.newSprites[sy][sx]; //cache
        }
    }
}

/**********************************************************************************************************************/
/**  Redraws only the dirty map tiles to the screen
**********************************************************************************************************************/
void ReDrawTiles(Camera cam)
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        uint16_t my = cam.y + sy;
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (!g_run.dirtyTiles[sy][sx])
                continue;

            uint16_t mx = cam.x + sx;
            uint16_t map_id = GetMapTile(mx, my);
            DrawTileCached(sx, sy, map_id);
        }
    }
}

/**********************************************************************************************************************/
/**  Redraws only the dirty map sprites to the screen
**********************************************************************************************************************/
void ReDrawSprites()
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (!g_run.dirtyTiles[sy][sx] && g_run.viewObjects[sy][sx] == NO_CREATURE)
                continue;

            uint8_t creature_type = g_run.viewObjects[sy][sx];
            if (creature_type != NO_CREATURE)
            {
                DEBUG("rendering unit at: %d %d %d", creature_type, sx, sy);
                DrawMonsterCached(sx, sy, creature_type);
            }
        }
    }
}


/**********************************************************************************************************************/
/**  Routine that runs the dirty tile update
 *  Handles the alternate smooth scrolling animation mode as well
**********************************************************************************************************************/
void RenderObjects()
{
    if (g_run.btns.gameSpeed < 5)
        AnimationMovement();

    Camera cam = GetCamera();
    ResetRenders();

    if (g_run.btns.gameSpeed >= 5)
        CheckForTileChanges(cam);

    GetObjectsInView(cam);
    SetDirty();

    if (g_run.btns.gameSpeed >= 5)
        ReDrawTiles(cam);

    ReDrawSprites();

    if (g_run.btns.gameSpeed >= 5)
        SleepMS(1000 / g_run.btns.gameSpeed);

    DEBUG("Finished Pico_RenderObjects()");
}
