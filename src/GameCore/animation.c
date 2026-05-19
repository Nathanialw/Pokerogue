//
// Created by nathanial on 4/10/26.
//

#include "animation.h"

#include "lib_debugging.h"
#include "lib_decl.h"

#include "animation_effects.h"
#include "camera.h"
#include "graphics.h"
#include "map.h"
#include "memory_access.h"
#include "memory_ram.h"
#include "memory_rom.h"
#include "tilesets.h"
#include "types.h"

/************************************************************************************************************
 *
 *      Core animation control, including general animation definitions
 *
 ************************************************************************************************************/


/**********************************************************************************************************************
 *  ANIMATIONS: --- WORK IN PROGRESS ---
 *
 *  TITLE SCREEN
 *
 *  MAP
 *      MOVE - SMOOTH MOVEMENT BETWEEN CELLS
 *      IDLE - 3 FRAME ANIMATIONS
 *      SPELL - CUSTOM ANIMATIONS (difficult, perhaps animations by spell type)
 *  BATTLES
 *      BATTLE START    - CLEAR SCREEN (several animations, random) - CREATURE BATTERS SLIDE INTO FRAME
 *      BATTLE END      - CLEAR SCREEN FADEOUT
 *      USE SKILL       - MANA SLIDES DOWN - CUSTOM ANIMATIONS (difficult, perhaps animations by skill type)
 *      DAMAGE TAKEN    - HEALTH SLIDES DOWN
 *      ON KILL         - XP SLIDES UP
 *      ON LEVEL UP     - LEVEL UP ANIMATION
 *  MENU
 *      PARTY FRAME - HEALTH SLIDES UP - MANA SLIDES UP
 *
**********************************************************************************************************************/


/************************************************************************************************************
 *  MOVE - SMOOTH SCREEN SCROLLING
 ************************************************************************************************************/
SET_MEMORY(".map")
void AnimationMovement(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory)
{
    DEBUG("Pico_AnimationMovement() scroll : %d %d", g_core.player.scroll.x, g_core.player.scroll.y);
    if (g_core.player.scroll.x == 0 && g_core.player.scroll.y == 0) return;


    Glyph16x16 glyph[4];
    uint8_t tile_cache[4];
    for (int16_t i = 0; i < 4; i++)
        tile_cache[i] = 255;

    const int16_t tile_size = 16;
    FrameBuffer f = {0, 0, tile_size * 2, 240, 0xd6fa};
    int16_t x_offset = 0;
    int16_t y_offset = 0;

    int8_t movement = 0;
    const int8_t speed = 16;

    Camera cam = GetCamera();

    int16_t tile_offset_x = 0;
    int16_t tile_offset_y = 0;

    uint8_t max_k = 2;
    if (g_core.player.scroll.x) max_k = 3;
    uint8_t max_y = VIEW_TH;
    if (g_core.player.scroll.y) max_y = VIEW_TH + 1;

    while (1)
    {
        for (int16_t j = 0; j < VIEW_TW; j++)
        {
            graphics.ClearBuffer();
            f.x = j * tile_size * 2;

            // DRAW TILES TO BUFFER
            for (int16_t k = 0; k < max_k; k++)
            {
                for (int16_t i = 0; i < max_y; i++)
                {
                    uint16_t mx = cam.x + (j * 2) + k;
                    uint16_t my = cam.y + i;
                    uint16_t partial_tile_id0 = GetMapTile(mx, my);
                    Rect_16 r;


                    Glyph16x16* glyph_ptr = NULL;
                    for (int16_t idx = 0; idx < 4; idx++)
                    {
                        if (partial_tile_id0 == tile_cache[idx])
                        {
                            glyph_ptr = &glyph[idx];
                            break;
                        }
                    }

                    if (!glyph_ptr)
                    {
                        for (int16_t idx = 0; idx < 4; idx++)
                        {
                            if (tile_cache[idx] == 255)
                            {
                                tile_cache[idx] = partial_tile_id0;
                                Tile t;
                                Flash_GetBiomeTile(memory, &t, g_core.biome, partial_tile_id0);
                                CharFromGlyph1bpp(memory, g_map.tileCache.spriteCache, glyph[idx].pixels, t.glyph_index, FONT16x16, Flash_GetColor(memory, t.fg), Flash_GetColor(memory, t.bg));
                                glyph_ptr = &glyph[idx];
                                break;
                            }
                        }
                        if (!glyph_ptr)
                        {
                            uint8_t fallback_idx = 3;
                            tile_cache[fallback_idx] = partial_tile_id0;
                            Tile t;
                            Flash_GetBiomeTile(memory, &t, g_core.biome, partial_tile_id0);
                            CharFromGlyph1bpp(memory, g_map.tileCache.spriteCache, glyph[fallback_idx].pixels, t.glyph_index, FONT16x16, Flash_GetColor(memory, t.fg), Flash_GetColor(memory, t.bg));
                            glyph_ptr = &glyph[fallback_idx];
                        }
                    }

                    r.x = (k * tile_size) + x_offset;
                    r.y = (i * tile_size) + y_offset;
                    r.w = tile_size - tile_offset_x;
                    r.h = tile_size - tile_offset_y;
                    Glyph16x16 clip_glyph;
                    ClipTile(clip_glyph.pixels, glyph_ptr->pixels, r);
                    graphics.DrawToBuffer(f, clip_glyph.pixels, r);

                    // TODO: handle partial tile drawing to buffer - should be 2 partial tiles drawn per glyph scrolling
                    // if (g_run.tileCache.tile_id != partial_tile_id1)
                    // {
                    //     g_run.tileCache.tile_id = partial_tile_id1;
                    //     const Tile t = g_gameFlash.sprites.biomes[g_run.biome][partial_tile_id1]; //TODO: FIX
                    //     CharFromGlyph1bpp(g_run.tileCache.tilePixels, t.glyph_index, FONT16x16, Flash_GetColor(memory, t.fg], Flash_GetColor(memory, t.bg]);
                    // }
                    //
                    // uint16_t partial_tile_id1 = GetMapTile(mx + tile_offset_x, my + tile_offset_y);
                    // Rect_16 r = {0, 0, tile_size, tile_size};
                    // r.x = (k * tile_size) + x_offset;
                    // r.y = (i * tile_size) + y_offset;
                    // r.w = tile_size - tile_offset_x;
                    // r.h = tile_size - tile_offset_y;
                    // DrawToBuffer(f, g_run.tileCache.tilePixels, r);
                }
            }

            // DRAW EACH BUFFER
            graphics.DrawBuffer(f);
        }

        // sleep_ms(16);

        if (g_core.player.scroll.x != 0)
        {
            movement += (speed * g_core.player.scroll.x) * -1;
            if (hardware.Abs(movement) > tile_size)
                break;
            x_offset = movement;
        }
        else if (g_core.player.scroll.y != 0)
        {
            movement += (speed * g_core.player.scroll.y) * -1;
            if (hardware.Abs(movement) > tile_size)
                break;
            y_offset = movement;
        }
        else
        {
            DEBUG("Pico_AnimationMovement() - IMPOSSIBLE : %d", movement);
        }
    }

    g_core.player.scroll.y = 0;
    g_core.player.scroll.x = 0;
}


/************************************************************************************************************
 *
 *      UI ANIMATIONS
 *
 ************************************************************************************************************/

/************************************************************************************************************
 *  HEALTH - FALL
 *  HEALTH - RISE TODO: NOT IMPLEMENTED YET
 ************************************************************************************************************/
SET_MEMORY(".battle")
void AnimationUpdateHealth(GraphicsInterface graphics, HardwareInterface hardware, bool attackersTurn)
{
    Rect_16 r = {0};
    uint16_t cur_hp = 0;
    uint16_t max_hp = 0;

    if (attackersTurn)
    {
        r = ENEMY_RESOURCE_FRAME;
        EntityId e_id = g_core.battleMode.enemyMonsterID;
        cur_hp = Int999GetCurrent(&g_core.creatures.hp[e_id]);
        max_hp = Int999GetMax(&g_core.creatures.hp[e_id]);
    }
    else
    {
        r = PLAYER_RESOURCE_FRAME;
        EntityId p_id = g_core.battleMode.playerMonsterID;
        cur_hp = Int999GetCurrent(&g_core.creatures.hp[p_id]);
        max_hp = Int999GetMax(&g_core.creatures.hp[p_id]);
    }


    uint16_t x = r.x + r.w - 8;
    const uint16_t y = r.y + (8 * 3);
    const uint16_t w = 1;
    const uint16_t h = RESOURCE_HEIGHT;
    ASSERT(r.w > 0 && r.h > 0, "Rectangle dimensions must be positive");


    const uint16_t bar_w = r.w - 16;
    const uint16_t dmg_w = ((float)cur_hp / (float)max_hp) * (float)bar_w;
    const uint16_t end_pos = x - (bar_w - dmg_w);

    while (true)
    {
        if (x <= end_pos)
            break;
        x -= 1;

        graphics.FillRect(x, y, w, h, 0xf00a);
        hardware.SleepMS(16);
    }
}

/************************************************************************************************************
 *  TODO: NOT IMPLEMENTED YET
 *  MANA - RISE AND FALL
 ************************************************************************************************************/
SET_MEMORY(".battle")
void AnimationUpdateMana(GraphicsInterface graphics, HardwareInterface hardware)
{
    FrameBuffer f = {50, 100, 64, 80, 0xd6fa};
    Rect_16 r = {5, 20, 10, 10};

    Color c;
    c.color = 0x001f;

    while (true)
    {
        graphics.TestAnimation(f, r, c);
        hardware.SleepMS(16);

        r.h += 1;
        if (r.h > 60)
            break;
    }
}


/************************************************************************************************************
 *  TODO: NOT IMPLEMENTED YET *
 *  XP - RISE AND FALL
 ************************************************************************************************************/
SET_MEMORY(".battle")
void AnimationUpdateXP(GraphicsInterface graphics, HardwareInterface hardware)
{
    FrameBuffer f = {50, 100, 64, 80, 0xd6fa};
    Rect_16 r = {5, 20, 10, 10};
    Color c;
    c.color = 0x001f;

    while (true)
    {
        graphics.TestAnimation(f, r, c);
        hardware.SleepMS(16);

        r.h += 1;
        if (r.h > 60)
            break;
    }
}

/************************************************************************************************************
 *  TODO: NOT IMPLEMENTED YET
 *  FADE - APPLY A MERGE SHADER THAT REACHES 100% INPUT COLOUR AT END
 ************************************************************************************************************/
SET_MEMORY(".map")
void AnimationScreenFade(GraphicsInterface graphics, HardwareInterface hardware)
{
    FrameBuffer f = {50, 100, 64, 80, 0xd6fa};
    Rect_16 r = {5, 20, 10, 10};
    Color c;
    c.color = 0x001f;

    while (true)
    {
        graphics.TestAnimation(f, r, c);
        hardware.SleepMS(16);

        r.h += 1;
        if (r.h > 60)
            break;
    }
}

/************************************************************************************************************
 *  TODO: NOT IMPLEMENTED YET
 *  CLEAR SCREEN WITH A RANDOM ANIMATION
 ************************************************************************************************************/
// SET_MEMORY(".core")
void AnimationScreenClearRandom(GraphicsInterface graphics, HardwareInterface hardware)
{
    FrameBuffer f = {50, 100, 64, 80, 0xd6fa};
    Rect_16 r = {5, 20, 10, 10};
    Color c;
    c.color = 0x001f;

    while (true)
    {
        graphics.TestAnimation(f, r, c);
        hardware.SleepMS(16);

        r.h += 1;
        if (r.h > 60)
            break;
    }
}


/************************************************************************************************************
 *  TODO: NOT IMPLEMENTED YET
 *  TITLE SCREEN WITH A CUSTOM ANIMATION
 ************************************************************************************************************/
SET_MEMORY(".splash")
void AnimationTitle(GraphicsInterface graphics, HardwareInterface hardware)
{
    FrameBuffer f = {50, 100, 64, 80, 0xd6fa};
    Rect_16 r = {5, 20, 10, 10};
    Color c;
    c.color = 0x001f;

    while (true)
    {
        graphics.TestAnimation(f, r, c);
        hardware.SleepMS(16);

        r.h += 1;
        if (r.h > 60)
            break;
    }
}


/************************************************************************************************************
 *  BATTLE START animation
 ************************************************************************************************************/
SET_MEMORY(".battle")
void AnimationBattlerStart(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory, bool onAttacker)
{
    Rect_16 r = GetBattlerRect(onAttacker);
    RefreshBattler(graphics, memory, onAttacker, r);

    MoveCenterToDown(graphics, hardware, r, r.h, 8);
}


/************************************************************************************************************
 *  DEATH animation
 ************************************************************************************************************/
SET_MEMORY(".battle")
void AnimationBattlerDie(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface memory, bool onAttacker)
{
    Rect_16 r = GetBattlerRect(onAttacker);
    RefreshBattler(graphics, memory, onAttacker, r);

    MoveCenterToDown(graphics, hardware, r, r.h, 8);
}


/************************************************************************************************************
 *  Handles all battler animations for spells, items and skills
 ************************************************************************************************************/
SET_MEMORY(".battle")
void BattlerAnimationAttack(GraphicsInterface graphics, MemoryInterface memory, bool player)
{
    const ObjectsTypes move_type = g_core.battleMode.moveType;

    const ObjectType move_id = g_core.battleMode.moveID;

    if (move_type == ITEM)
        Flash_BattlerAttackAnimation(memory, move_id.ItemId, ITEM, player);
    else if (SKILL)
        Flash_BattlerAttackAnimation(memory, move_id.AbilityId, SKILL, player);
    else if (SPELL)
        Flash_BattlerAttackAnimation(memory, move_id.SpellId, SPELL, player);
}


SET_MEMORY(".battle")
void BattlerAnimationStruck(GraphicsInterface graphics, MemoryInterface memory, bool player)
{
    const ObjectsTypes move_type = g_core.battleMode.moveType;

    const ObjectType move_id = g_core.battleMode.moveID;

    if (move_type == ITEM)
        Flash_BattlerStruckAnimation(memory, move_id.ItemId, ITEM, player);
    else if (SKILL)
        Flash_BattlerStruckAnimation(memory, move_id.AbilityId, SKILL, player);
    else if (SPELL)
        Flash_BattlerStruckAnimation(memory, move_id.SpellId, SPELL, player);
}
