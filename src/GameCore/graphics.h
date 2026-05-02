//
// Created by nathanial on 4/11/26.
//
#pragma once
#include "enums.h"
#include "types.h"
#include "lib_types.h"

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
void ClipTile(uint16_t* clip, const uint16_t* pixels, Rect_16 r);

void DrawMonster(uint8_t screen_tx, uint8_t screen_ty, Creature tile_id);
void DrawTile(uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id);
void DrawMonsterCached(uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id);
void DrawTileCached(uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id);

FrameBuffer DrawBattlerToBuffer(uint16_t screen_x, uint16_t screen_y, const SpriteLayout layout, const uint8_t* sprite);
void DrawBattler(uint16_t screen_x, uint16_t screen_y, const SpriteLayout layout, const uint8_t* sprite);
void HandleMenu();
void HandleGameMenu();
void DrawCursor();
void HandleBattle();
void HandleBattleMenu();

