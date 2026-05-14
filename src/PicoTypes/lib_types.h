//
// Created by nathanial on 5/2/26.
//
#pragma once
#include "lib_constants.h"
#include "lib_memory.h"
#include "stdbool.h"
#include "stdint.h"


/**********************************************************************************************************************/
/*  The main frame buffer
*   can be accessed as:
*   linear array of  25088 uint8_t
*   linear array of  12544 uint16_t
*   front buff and back buffers each of 6272 uint16_t
**********************************************************************************************************************/
typedef union
{
    uint16_t frameBuffer[BUFFER_SIZE_2BYTES];
    uint8_t frameBuffer1byte[BUFFER_SIZE_1BYTE];

    struct
    {
        uint16_t front[BUFFER_SIZE_BACK];
        uint16_t back[BUFFER_SIZE_FRONT];
    };
} PartialFrameBuffer;

_Static_assert(sizeof(PartialFrameBuffer) == BUFFER_SIZE_1BYTE, "SpriteLayout must be 76800 bytes");


/**********************************************************************************************************************/
/**  Stores the current game state
 *   Holds enums:
*       GameState
*       InputState
*       BattleState
**********************************************************************************************************************/
typedef struct State
{
    uint8_t gameState : 2;
    uint8_t battleState : 2;
    uint8_t inputState : 3;
    uint8_t _pad : 1;
} State;


typedef union Color
{
    struct
    {
        uint16_t r : 5;
        uint16_t g : 6;
        uint16_t b : 5;
    };

    uint16_t color;
} Color;

/**********************************************************************************************************************/
/**holds the data for each music note
*   note            max <128
*   rest            bool
*   duration        max <16
*   instrument      max <4
*   articulation    max <4
*   volume          max <8
**********************************************************************************************************************/
typedef struct
{
    uint8_t note : 7;
    uint8_t rest : 1;
    uint8_t duration : 4; // increments of 1/16 for note length - 15 is a full note
    uint8_t instrument : 2;
    uint8_t articulation : 2;
    uint8_t volume : 3;
    //TODO: bits unused
    uint8_t _pad : 5;
} Note;

_Static_assert(sizeof(Note) == 3, "Note must be 3 bytes");

/**********************************************************************************************************************/
/**holds the data for a song
*   idx
*   offset
*   length
*   delay
*   numVoices
*   BPM
**********************************************************************************************************************/
typedef struct
{
    uint32_t idx;
    uint32_t offset[MAX_VOICES];
    uint16_t length[MAX_VOICES];
    uint16_t delay[MAX_VOICES];
    uint8_t numVoices;
    uint8_t BPM;
} MusicData;


/**********************************************************************************************************************/
/** defines a rectangle each value is a 2-byte signed integer
**********************************************************************************************************************/
typedef struct
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
} Rect_16;

_Static_assert(sizeof(Rect_16) == 8, "Rect_16 must be 8 bytes");

/**********************************************************************************************************************/
/**  Defines the position, dimensions and background colour of a frame buffer to draw to
**********************************************************************************************************************/
typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    uint16_t bgColor;
} FrameBuffer;

_Static_assert(sizeof(FrameBuffer) == 10, "FrameBuffer must be 10 bytes");

typedef struct
{
    int8_t x;
    int8_t y;
    bool down;
    bool left;
    bool right;
    bool center;
    bool js_up;
    bool js_down;
    bool js_left;
    bool js_right;
    bool js_center;
    bool a;
    bool b;
    bool c;
    bool d;
    bool start;
    bool select;
} ControlState;


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
typedef struct Delta
{
    int8_t x : 4;
    int8_t y : 4;
} Delta;

//
_Static_assert(sizeof(Delta) == 1, "Delta must be 1 bytes");


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
typedef struct
{
    uint16_t buttons;
    Delta d;
    Delta dp;
    Delta js;
} KeyState;

_Static_assert(sizeof(KeyState) == 6, "KeyState must be 4 bytes");
