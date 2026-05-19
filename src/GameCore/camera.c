//
// Created by nathanial on 2/20/26.
//
#include "camera.h"

#include "memory_ram.h"
#include "player.h"


/**********************************************************************************************************************/
/** Returns the rect of the camera which holds the map position and dimensions of the camera
**********************************************************************************************************************/
SET_MEMORY(".map")
Camera GetCamera(void)
{
    return g_core.camera;
}

/**********************************************************************************************************************/
/** sets the camera to the given position and dimensions
**********************************************************************************************************************/
SET_MEMORY(".map")
Camera InitCamera(uint8_t x, uint8_t y, uint16_t w, uint16_t h)
{
    g_core.camera.x = x;
    g_core.camera.y = y;
    g_core.camera.w = w;
    g_core.camera.h = h;

    return g_core.camera;
}

/**********************************************************************************************************************/
/** sets the camera to the given position
 *  with default dimensions
 *  returns the camera rect
**********************************************************************************************************************/
Camera SetCamera(uint8_t x, uint8_t y)
{
    g_core.camera.x = x - CAM_OFFSET_X;
    g_core.camera.y = y - CAM_OFFSET_Y;
    return g_core.camera;
}

/**********************************************************************************************************************/
/** Sets camera position to the player position offset by the default camera size
 *  Sets camera dimensions to the default camera dimensions
**********************************************************************************************************************/
SET_MEMORY(".map")
void SetCameraPlayer(void)
{
    Position pos = GetPlayerPosition();
    uint8_t px = pos.x;
    uint8_t py = pos.y;

    g_core.camera.x = px - CAM_OFFSET_X;
    g_core.camera.y = py - CAM_OFFSET_Y;
}

/**********************************************************************************************************************/
/** Check whether the given cell position  intersects with the camera rect
 *  ON SUCCESS - returns true
 *  ON FAIL - returns false
**********************************************************************************************************************/
SET_MEMORY(".map")
bool CameraContains(uint8_t x, uint8_t y)
{
    if (x >= g_core.camera.x && x < (g_core.camera.x + VIEW_TW) && y >= g_core.camera.y && y < (g_core.camera.y + VIEW_TH))
        return true;
    return false;
}
