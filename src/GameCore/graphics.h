//
// Created by nathanial on 4/11/26.
//
#pragma once
#include "enums.h"
#include "lib_decl.h"
#include "types.h"
#include "lib_types.h"

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
void ClipTile(uint16_t* clip, const uint16_t* pixels, Rect_16 r);

void DrawTile(GraphicsInterface graphics, MemoryInterface memory, uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id);
void DrawSprite(GraphicsInterface graphics, MemoryInterface memory, uint8_t screen_tx, uint8_t screen_ty, Creature tile_id, ObjectsTypes type);
void DrawSpriteCached(GraphicsInterface graphics, MemoryInterface memory, uint8_t screen_tx, uint8_t screen_ty, uint8_t sprite_id, ObjectsTypes type);
void DrawTileCached(GraphicsInterface graphics, MemoryInterface memory, uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id);

FrameBuffer DrawBattlerToBuffer(GraphicsInterface graphics, MemoryInterface memory, uint16_t screen_x, uint16_t screen_y, const SpriteLayout layout, ObjectsTypes type, bool front);
void DrawBattler(GraphicsInterface graphics, MemoryInterface memory, uint16_t screen_x, uint16_t screen_y, const SpriteLayout layout, ObjectsTypes type, bool front);
void HandleMenu(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory);
void HandleGameMenu(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory);
void DrawCursor(GraphicsInterface graphics, MemoryInterface memory);
void HandleBattle(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory);
void HandleBattleMenu(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory);

