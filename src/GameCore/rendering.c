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
#include "lib_decl.h"
#include "memory_rom.h"


/**********************************************************************************************************************/
/**  Redraws all map tiles and entities ion the camera view to the screen
**********************************************************************************************************************/
void FullRedraw()
{
    Camera cam = GetCamera();

    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        uint16_t my = cam.y + sy;
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            uint16_t mx = cam.x + sx;
            uint16_t id = GetMapTile(mx, my);
            DrawTile(sx, sy, id);
            g_run.view.viewTiles[sy][sx] = id;
        }
    }

    for (uint8_t i = 0; i < ITEM_COUNT; ++i)
    {
        uint8_t x = g_run.items.position[i].x;
        uint8_t y = g_run.items.position[i].y;
        if (GetBit(g_run.items.onMap, i) && CameraContains(x, y))
        {
            uint8_t rx = (x - cam.x);
            uint8_t ry = (y - cam.y);
            DrawMonster(rx, ry, g_run.items.types[i], g_gameFlash.sprites.items);
            g_run.view.viewItems.viewEntities[ry][rx] = g_run.items.types[i];
        }
    }

    for (uint8_t i = 0; i < OBJECT_COUNT; ++i)
    {
        uint8_t x = g_run.objects.position[i].x;
        uint8_t y = g_run.objects.position[i].y;
        if (GetBit(g_run.objects.onMap, i) && CameraContains(x, y))
        {
            uint8_t rx = (x - cam.x);
            uint8_t ry = (y - cam.y);
            DrawMonster(rx, ry, g_run.objects.types[i], g_gameFlash.sprites.objects);
            g_run.view.viewObjects.viewEntities[ry][rx] = g_run.objects.types[i];
        }
    }

    for (uint8_t i = 0; i < ENTITY_COUNT; ++i)
    {
        uint8_t x = g_run.creatures.position[i].x;
        uint8_t y = g_run.creatures.position[i].y;
        if (GetBit(g_run.creatures.onMap, i) && CameraContains(x, y))
        {
            uint8_t rx = (x - cam.x);
            uint8_t ry = (y - cam.y);

            DrawMonster(rx, ry, g_run.creatures.types[i], g_gameFlash.sprites.monsters);
            g_run.view.viewCreatures.viewEntities[ry][rx] = g_run.creatures.types[i];
        }
    }
}


/**********************************************************************************************************************/
/** Clears the dirtyTiles array to false
 * Clears the newSprites array to NO_CREATURE
**********************************************************************************************************************/
void ResetRenders(ViewEntities* view, uint8_t count)
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
            view->newSprites[sy][sx] = count;
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
            if (g_run.view.viewTiles[sy][sx] != tile)
                SetBit(g_run.view.dirtyTiles, (sy * VIEW_TH) + sx, true);

            g_run.view.viewTiles[sy][sx] = tile; //cache
        }
    }
}

/**********************************************************************************************************************/
/**  Retrieves all entities in the camera view
 *  Saves their types into newSprites array at the reletive tile position
**********************************************************************************************************************/
void GetEntitiesInView(Camera cam, BitFieldUint8* onMap, ViewEntities* view, Position* pos, const uint8_t* types, uint8_t count)
{
    uint8_t id = 0;
    while (1)
    {
        if (GetBit(*onMap, id))
        {
            uint8_t x = pos[id].x;
            uint8_t y = pos[id].y;
            if (CameraContains(x, y))
            {
                uint8_t sx = (x - cam.x);
                uint8_t sy = (y - cam.y);
                view->newSprites[sy][sx] = types[id];
            }
        }

        id++;
        if (id == count) break;
    }
}

/**********************************************************************************************************************/
/** Checks whether the sprites in the viewObjects array has changed
 * Save changes as dirty bool in the dirtyTiles array
**********************************************************************************************************************/
void SetDirty(ViewEntities* view)
{
    for (uint8_t sy = 0; sy < VIEW_TH; sy++)
    {
        for (uint8_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (view->viewEntities[sy][sx] != view->newSprites[sy][sx])
                SetBit(g_run.view.dirtyTiles, (sy * VIEW_TH) + sx, true);

            view->viewEntities[sy][sx] = view->newSprites[sy][sx]; //cache
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
            if (!GetBit(g_run.view.dirtyTiles, (sy * VIEW_TH) + sx))
                continue;

            uint16_t mx = cam.x + sx;
            uint16_t map_id = GetMapTile(mx, my);
            DrawTileCached(sx, sy, map_id);
        }
    }
}

/**********************************************************************************************************************/
/**     Redraws only the dirty map sprites to the screen
 *      Objects then Items then Creatures
**********************************************************************************************************************/
void ReDrawSprites()
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (!GetBit(g_run.view.dirtyTiles, (sy * VIEW_TH) + sx))
                continue;

            if (g_run.view.viewItems.viewEntities[sy][sx] != NO_ITEM)
            {
                uint8_t item_type = g_run.view.viewItems.viewEntities[sy][sx];
                DrawMonsterCached(sx, sy, item_type);
            }

            if (g_run.view.viewObjects.viewEntities[sy][sx] != NO_OBJECT)
            {
                uint8_t object_type = g_run.view.viewObjects.viewEntities[sy][sx];
                DrawMonsterCached(sx, sy, object_type);
            }

            if (g_run.view.viewCreatures.viewEntities[sy][sx] != NO_CREATURE)
            {
                uint8_t creature_type = g_run.view.viewCreatures.viewEntities[sy][sx];
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

    memset(g_run.view.dirtyTiles, 0, sizeof(g_run.view.dirtyTiles));

    ResetRenders(&g_run.view.viewItems, NO_ITEM);
    ResetRenders(&g_run.view.viewObjects, NO_OBJECT);
    ResetRenders(&g_run.view.viewCreatures, NO_CREATURE);

    Camera cam = GetCamera();
    if (g_run.btns.gameSpeed >= 5)
        CheckForTileChanges(cam);

    GetEntitiesInView(cam, &g_run.items.onMap, &g_run.view.viewItems, g_run.items.position, g_run.items.types, ITEM_COUNT);
    GetEntitiesInView(cam, &g_run.objects.onMap, &g_run.view.viewObjects, g_run.objects.position, g_run.objects.types, OBJECT_COUNT);
    GetEntitiesInView(cam, &g_run.creatures.onMap, &g_run.view.viewCreatures, g_run.creatures.position, g_run.creatures.types, ENTITY_COUNT);

    SetDirty(&g_run.view.viewItems);
    SetDirty(&g_run.view.viewObjects);
    SetDirty(&g_run.view.viewCreatures);

    if (g_run.btns.gameSpeed >= 5)
        ReDrawTiles(cam);

    ReDrawSprites();

    if (g_run.btns.gameSpeed >= 5)
        SleepMS(1000 / g_run.btns.gameSpeed);
}
