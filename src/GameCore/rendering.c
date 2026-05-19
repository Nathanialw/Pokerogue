//
// Created by nathanial on 4/11/26.
//
#include "rendering.h"

#include "lib_types.h"
#include "lib_decl.h"

#include "animation.h"
#include "camera.h"

#include "map.h"
#include "utils.h"
#include "graphics.h"
#include "memory_ram.h"
#include "memory_rom.h"


/**********************************************************************************************************************/
/**  Redraws all map tiles and entities ion the camera view to the screen
**********************************************************************************************************************/
SET_MEMORY(".map.rodata")
static const char FullRedraw_start[] = "FullRedraw_start";
SET_MEMORY(".map.rodata")
static const char FullRedraw_tiles[] = "FullRedraw_tiles";
SET_MEMORY(".map.rodata")
static const char FullRedraw_items[] = "FullRedraw_items";
SET_MEMORY(".map.rodata")
static const char FullRedraw_objects[] = "FullRedraw_objects";
SET_MEMORY(".map.rodata")
static const char FullRedraw_creaturee[] = "FullRedraw_creature";
SET_MEMORY(".map.rodata")
static const char FullRedraw_DONE[] = "FullRedraw_DONE";

SET_MEMORY(".map")
void FullRedraw(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory)
{
    hardware.Print(FullRedraw_start);

    Camera cam = GetCamera();

    hardware.Print(FullRedraw_tiles);
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        uint16_t my = cam.y + sy;
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            uint16_t mx = cam.x + sx;
            uint16_t id = GetMapTile(mx, my);
            DrawTile(graphics, memory, sx, sy, id);
            g_map.view.viewTiles[sy][sx] = id;
        }
    }

    hardware.Print(FullRedraw_items);
    for (uint8_t i = 0; i < g_core.items.total; ++i)
    {
        if (g_core.items.types[i] == NO_ITEM) continue;

        uint8_t x = g_core.items.position[i].x;
        uint8_t y = g_core.items.position[i].y;
        if (GetBit(g_core.items.onMap, i) && CameraContains(x, y))
        {
            uint8_t rx = (x - cam.x);
            uint8_t ry = (y - cam.y);
            DrawSprite(graphics, memory, rx, ry, g_core.items.types[i], ITEM);
            g_map.view.viewItems.viewEntities[ry][rx] = g_core.items.types[i];
        }
    }

    hardware.Print(FullRedraw_objects);
    for (uint8_t i = 0; i < g_core.objects.total; ++i)
    {
        if (g_core.objects.types[i] == NO_OBJECT) continue;

        uint8_t x = g_core.objects.position[i].x;
        uint8_t y = g_core.objects.position[i].y;
        if (GetBit(g_core.objects.onMap, i) && CameraContains(x, y))
        {
            uint8_t rx = (x - cam.x);
            uint8_t ry = (y - cam.y);
            DrawSprite(graphics, memory, rx, ry, g_core.objects.types[i], OBJECT);
            g_map.view.viewObjects.viewEntities[ry][rx] = g_core.objects.types[i];
        }
    }

    hardware.Print(FullRedraw_creaturee);
    for (uint8_t i = 0; i < g_core.creatures.total; ++i)
    {
        if (g_core.creatures.types[i] == NO_CREATURE) continue;

        uint8_t x = g_core.creatures.position[i].x;
        uint8_t y = g_core.creatures.position[i].y;
        if (GetBit(g_core.creatures.onMap, i) && CameraContains(x, y))
        {
            uint8_t rx = (x - cam.x);
            uint8_t ry = (y - cam.y);

            DrawSprite(graphics, memory, rx, ry, g_core.creatures.types[i], CREATURE);
            g_map.view.viewCreatures.viewEntities[ry][rx] = g_core.creatures.types[i];
        }
    }

    hardware.Print(FullRedraw_DONE);
}


/**********************************************************************************************************************/
/** Clears the dirtyTiles array to false
 * Clears the newSprites array to NO_CREATURE
**********************************************************************************************************************/
SET_MEMORY(".map")
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
SET_MEMORY(".map")
void CheckForTileChanges(Camera cam)
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        uint16_t my = cam.y + sy;
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            uint16_t mx = cam.x + sx;
            uint8_t tile = GetMapTile(mx, my);
            if (g_map.view.viewTiles[sy][sx] != tile)
                SetBit(g_map.view.dirtyTiles, (sy * VIEW_TH) + sx, true);

            g_map.view.viewTiles[sy][sx] = tile; //cache
        }
    }
}

/**********************************************************************************************************************/
/**  Retrieves all entities in the camera view
 *  Saves their types into newSprites array at the reletive tile position
**********************************************************************************************************************/
SET_MEMORY(".map")
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
SET_MEMORY(".map")
void SetDirty(ViewEntities* view)
{
    for (uint8_t sy = 0; sy < VIEW_TH; sy++)
    {
        for (uint8_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (view->viewEntities[sy][sx] != view->newSprites[sy][sx])
                SetBit(g_map.view.dirtyTiles, (sy * VIEW_TH) + sx, true);

            view->viewEntities[sy][sx] = view->newSprites[sy][sx]; //cache
        }
    }
}

/**********************************************************************************************************************/
/**  Redraws only the dirty map tiles to the screen
**********************************************************************************************************************/
SET_MEMORY(".map")
void ReDrawTiles(GraphicsInterface graphics, MemoryInterface memory, Camera cam)
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        uint16_t my = cam.y + sy;
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (!GetBit(g_map.view.dirtyTiles, (sy * VIEW_TH) + sx))
                continue;

            uint16_t mx = cam.x + sx;
            uint16_t map_id = GetMapTile(mx, my);
            DrawTileCached(graphics, memory, sx, sy, map_id);
        }
    }
}

/**********************************************************************************************************************/
/**     Redraws only the dirty map sprites to the screen
 *      Objects then Items then Creatures
**********************************************************************************************************************/
SET_MEMORY(".map")
void ReDrawSprites(GraphicsInterface graphics, MemoryInterface memory)
{
    for (uint16_t sy = 0; sy < VIEW_TH; sy++)
    {
        for (uint16_t sx = 0; sx < VIEW_TW; sx++)
        {
            if (!GetBit(g_map.view.dirtyTiles, (sy * VIEW_TH) + sx))
                continue;

            if (g_map.view.viewItems.viewEntities[sy][sx] != NO_ITEM)
            {
                uint8_t item_type = g_map.view.viewItems.viewEntities[sy][sx];
                DrawSpriteCached(graphics, memory, sx, sy, item_type, ITEM);
            }

            if (g_map.view.viewObjects.viewEntities[sy][sx] != NO_OBJECT)
            {
                uint8_t object_type = g_map.view.viewObjects.viewEntities[sy][sx];
                DrawSpriteCached(graphics, memory, sx, sy, object_type, OBJECT);
            }

            if (g_map.view.viewCreatures.viewEntities[sy][sx] != NO_CREATURE)
            {
                uint8_t creature_type = g_map.view.viewCreatures.viewEntities[sy][sx];
                DrawSpriteCached(graphics, memory, sx, sy, creature_type, CREATURE);
            }
        }
    }
}


/**********************************************************************************************************************/
/**  Routine that runs the dirty tile update
 *  Handles the alternate smooth scrolling animation mode as well
**********************************************************************************************************************/
SET_MEMORY(".map")
void RenderObjects(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory)
{
    // if (g_core.btns.gameSpeed < 5)
    // Animat
    // ionMovement(graphics, hardware, memory);

    hardware.MemSet(g_map.view.dirtyTiles, 0, sizeof(g_map.view.dirtyTiles));

    ResetRenders(&g_map.view.viewItems, NO_ITEM);
    ResetRenders(&g_map.view.viewObjects, NO_OBJECT);
    ResetRenders(&g_map.view.viewCreatures, NO_CREATURE);


    Camera cam = GetCamera();
    if (g_core.btns.gameSpeed >= 5)
        CheckForTileChanges(cam);

    GetEntitiesInView(cam, &g_core.items.onMap, &g_map.view.viewItems, g_core.items.position, g_core.items.types, g_core.items.total);
    GetEntitiesInView(cam, &g_core.objects.onMap, &g_map.view.viewObjects, g_core.objects.position, g_core.objects.types, g_core.objects.total);
    GetEntitiesInView(cam, &g_core.creatures.onMap, &g_map.view.viewCreatures, g_core.creatures.position, g_core.creatures.types, g_core.creatures.total);

    SetDirty(&g_map.view.viewItems);
    SetDirty(&g_map.view.viewObjects);
    SetDirty(&g_map.view.viewCreatures);

    if (g_core.btns.gameSpeed >= 5)
        ReDrawTiles(graphics, memory, cam);


    ReDrawSprites(graphics, memory);


    // if (g_core.btns.gameSpeed >= 5)
    // hardware.SleepMS(1000 / g_core.btns.gameSpeed);
}
