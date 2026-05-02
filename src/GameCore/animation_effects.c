//
// Created by nathanial on 4/8/26.
//
#include <stddef.h>

#include "animation_effects.h"

#include "entities.h"
#include "graphics.h"
#include "lib_debugging.h"
#include "memory_ram.h"
#include "memory_rom.h"
#include "lib_decl.h"

/************************************************************************************************************
 *
 *  Composable animation effects to build combat animations for skills, spells, items
 *
 ************************************************************************************************************/

/************************************************************************************************************
 *
 *      HELPERS
 *
 ************************************************************************************************************/

/************************************************************************************************************
 *  Utility functions
 ************************************************************************************************************/


Rect_16 GetBattlerRect(bool onAttacker)
{
    Rect_16 r = {0, 0, BATTLER_TILES_W * TILE_W, BATTLER_TILES_H * TILE_H};

    if (onAttacker)
    {
        r.x = PLAYER_BATTLER_FRAME.x + BATTLER_OFFSET;
        r.y = PLAYER_BATTLER_FRAME.y;
        return r;
    }

    r.x = ENEMY_BATTLER_FRAME.x + BATTLER_OFFSET;
    r.y = ENEMY_BATTLER_FRAME.y;
    return r;
}

const uint8_t* GetBattlerSprite(bool onAttacker)
{
    if (onAttacker)
        return g_gameFlash.spriteData.battlers.back;
    return g_gameFlash.spriteData.battlers.front;
}

SpriteLayout GetBattlerLayout(bool onAttacker)
{
    if (onAttacker)
        return g_gameFlash.spriteData.battlers.backLayout[GetCreatureType(g_run.battleMode.playerMonsterID)];
    return g_gameFlash.spriteData.battlers.frontLayout[GetCreatureType(g_run.battleMode.enemyMonsterID)];
}

void RefreshBattler(bool onAttacker, Rect_16 r)
{
    SpriteLayout layout = GetBattlerLayout(onAttacker);
    const uint8_t* sprite = GetBattlerSprite(onAttacker);
    DrawBattlerToBuffer(r.x, r.y, layout, sprite);
}


void ReDrawBattler(bool onAttacker, Rect_16 r)
{
    SpriteLayout layout = GetBattlerLayout(onAttacker);
    const uint8_t* sprite = GetBattlerSprite(onAttacker);
    DrawBattler(r.x, r.y, layout, sprite);
}

/************************************************************************************************************
 *  ensures the callback is not NULL
 *  validates sprite drawing rect position and size
 ************************************************************************************************************/
void BasicAsserts(Rect_16 r)
{
    ASSERT(r.x <= SCREEN_W, "r.x is larger than SCREEN_W, underflow error");
    ASSERT(r.y <= SCREEN_H, "r.y is larger than SCREEN_H, underflow error");
    ASSERT(r.w > 0 && r.h > 0, "Rectangle dimensions must be positive");
}

void AnimationsBasicAssert()
{
}

/************************************************************************************************************
 *
 *      BATTLER MOVEMENT FUNCTIONS *
 *
 ************************************************************************************************************/

/************************************************************************************************************
 *  Redraws the framebuffer contents from the center to the left d pixels
 *  Updates every frameLength ms
 ************************************************************************************************************/
void MoveCenterToLeft(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.x, "d is larger than r.x, underflow error");

    FrameBuffer f = {r.x, r.y, r.w, r.h};
    uint16_t end_pos = f.x - d;

    while (f.x > end_pos)
    {
        f.x -= 1;

        DrawBuffer(f);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *  Redraws the framebuffer contents right from the center - d pixels to the center
 *  Updates every frameLength ms
 ************************************************************************************************************/
void MoveLeftToCenter(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.x, "d is larger than r.x, underflow error");

    FrameBuffer f = {r.x - d, r.y, r.w, r.h};
    uint16_t end_pos = r.x;

    while (f.x < end_pos)
    {
        f.x += 1;
        DrawBuffer(f);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *  Redraws the framebuffer contents from the center to the right d pixels
 *  Updates every frameLength ms
 ************************************************************************************************************/
void MoveCenterToRight(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);

    FrameBuffer f = {r.x, r.y, r.w, r.h};
    uint16_t end_pos = f.x + d;

    while (f.x < end_pos)
    {
        f.x += 1;
        DrawBuffer(f);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *  Redraws the framebuffer contents left from the center + d pixels to the center
 *  Updates every frameLength ms
 ************************************************************************************************************/
void MoveRightToCenter(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);

    FrameBuffer f = {r.x + d, r.y, r.w, r.h};
    uint16_t end_pos = r.x;

    while (f.x > end_pos)
    {
        f.x -= 1;
        DrawBuffer(f);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *  Redraws the framebuffer contents down from the center to the center + d pixels
 *  Updates every frameLength ms
 ************************************************************************************************************/
void MoveCenterToDown(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");

    FrameBuffer f = {r.x, r.y, r.w, r.h};
    uint16_t end_pos = r.y + d;

    while (f.y < end_pos)
    {
        f.y += 1;
        f.h -= 1; // ensures the sprite does not draw beyond the bounds of the battler area
        DrawBuffer(f);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *  Redraws the framebuffer contents up from the center to the center + d pixels
 *  Updates every frameLength ms
 ************************************************************************************************************/
void MoveCenterToTop(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");
    const uint8_t* sprite = GetFrameBuffer1byte();

    FrameBuffer f = {r.x, r.y, r.w, r.h};
    uint16_t end_pos = r.h - d;

    while (f.h > end_pos)
    {
        sprite += (f.w * sizeof(Pixel)); // increments the pixel array by 1 row of pixels
        f.h -= 1; // shrinks the height draw frame by 1 row
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *
 *  BATTLER DRAW WARPING FUNCTIONS
 *
 ************************************************************************************************************/

/************************************************************************************************************
 *  Distorts the battler, displaying 3 of the sprite horizontally across the battle area
 *  while shifting the sprite upward pixels
 *  Advances source by partial row for distortion
 ************************************************************************************************************/
void AnimationMirrorImageFloatingUp(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");
    const uint8_t* sprite = GetFrameBuffer1byte();

    FrameBuffer f = {r.x + BATTLER_OFFSET, r.y, r.w, r.h};
    uint16_t end_pos = r.h - d;

    while (f.h > end_pos)
    {
        sprite += f.w;
        f.h -= 1;
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}


/************************************************************************************************************
 *
 ************************************************************************************************************/
void AnimationSpooky(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");

    const uint8_t* sprite = GetFrameBuffer1byte();

    FrameBuffer f = {r.x, r.y, r.w, r.h};
    uint16_t end_pos = r.w * 2;

    uint16_t j = 0;
    while (j < end_pos)
    {
        sprite++;
        j++;
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *
 ************************************************************************************************************/
void AnimationSpookyMoveCenterToLeft(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");
    const uint8_t* sprite = GetFrameBuffer1byte();

    FrameBuffer f = {r.x, r.y, r.w, r.h};
    uint16_t end_pos = r.w / 2;

    uint16_t j = 0;
    while (j < end_pos)
    {
        sprite++;
        j++;
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}


/************************************************************************************************************
 *
 ************************************************************************************************************/
void AnimationSpookyMoveLeftToCenter(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");
    const uint8_t* sprite = GetFrameBuffer1byte();

    sprite += r.w / 2;
    FrameBuffer f = {r.x, r.y, r.w, r.h};
    uint16_t end_pos = 0;

    uint16_t j = r.w / 2;
    while (j > end_pos)
    {
        sprite--;
        j--;
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}


/************************************************************************************************************
 *
 ************************************************************************************************************/
void AnimationMirrorImage2(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");
    const uint8_t* sprite = GetFrameBuffer1byte();


    FrameBuffer f = {r.x + BATTLER_OFFSET, r.y, r.w, r.h};
    uint16_t end_pos = r.h - d;

    while (f.h > end_pos)
    {
        sprite += f.w;
        f.h--;
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}

/************************************************************************************************************
 *  draws an ice shard graphic at a random screen position in the battler draw area
 ************************************************************************************************************/
void AnimationIceShards(Rect_16 r, uint16_t d, uint8_t frameLength)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");
    const uint8_t* sprite = GetFrameBuffer1byte();

    FrameBuffer f = {r.x + BATTLER_OFFSET, r.y, r.w, r.h};
    uint16_t end_pos = r.h - d;

    while (f.h > end_pos)
    {
        sprite += f.w;
        f.h--;
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}


/************************************************************************************************************
 *
 ************************************************************************************************************/
void AnimationBeam(Rect_16 r, uint16_t d, uint8_t frameLength, uint8_t palletIndex)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");;
    SetFrameBuffer(g_gameFlash.GetColor[palletIndex]);
    const uint8_t* sprite = GetFrameBuffer1byte();

    uint16_t end_pos = r.x + r.w;
    FrameBuffer f = {r.x + r.w - 64, r.y + 64, 4, 4};

    uint16_t j = 0;
    while (f.y > r.y && f.x < end_pos)
    {
        j++;
        f.y -= j % 2;
        f.x++;
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}


/************************************************************************************************************
 *
 ************************************************************************************************************/
void AnimationRandomParticles(Rect_16 r, uint16_t d, uint8_t frameLength, uint8_t palletIndex, uint8_t particleCount)
{
    BasicAsserts(r);
    ASSERT(d <= r.h, "d is larger than r.h, underflow error");
    SetFrameBuffer(g_gameFlash.GetColor[palletIndex]);
    const uint8_t* sprite = GetFrameBuffer1byte();

    uint16_t end_pos = particleCount;
    uint16_t size = 4;
    FrameBuffer f = {r.x + r.w - 64, r.y + 64, size, size};

    uint16_t j = 0;
    while (j < end_pos)
    {
        j++;
        f.y = r.y + (rand() % (r.h - size));
        f.x = r.x + (rand() % (r.w - size));
        DrawSprite(f, sprite);
        SleepMS(frameLength);
    }
}


/************************************************************************************************************/
/*  Draws a line of pixels of the given colour for each long in the battler sprite
************************************************************************************************************/
void AnimationLineEffect(Rect_16 r, uint16_t d, uint8_t frameLength, uint8_t palletIndex)
{
    BasicAsserts(r);
    //Cache the line of pixels
    //replace the linx of pixels with given colour
    //display
    //replace pixels with cached
    //move to the next line
    //cycle through the frame buffer
    uint16_t n = r.w;
    const uint16_t* sprite = GetFrameBuffer2bytes();

    uint16_t buffer[n];
    SetBuffer(n, buffer, g_gameFlash.GetColor[palletIndex]);

    FrameBuffer f = {r.x, r.y, n, 1};
    uint16_t start_pos = 0;
    uint16_t end_pos = r.h;

    while (start_pos < end_pos)
    {
        DrawSprite(f, (uint8_t*)buffer);
        SleepMS(frameLength);
        const uint16_t* b = sprite + (n * start_pos);
        DrawSprite(f, (uint8_t*)b);
        start_pos++;
        f.y++;
    }
}

/************************************************************************************************************/
/*  Draws a line of pixels of the given colour for each long in the battler sprite
************************************************************************************************************/
void AnimationRainbowEffects(Rect_16 r, uint16_t d, uint8_t frameLength, uint8_t palletIndex)
{
    BasicAsserts(r);

}
