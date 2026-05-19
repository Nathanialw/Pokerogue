//
// Created by nathanial on 5/2/26.
//
#pragma once



/**********************************************************************************************************************/
/** Sets input logic path for game state branching
**********************************************************************************************************************/
typedef enum
{
    // IDLE,
    INPUT_MOVING,
    INPUT_MENU,
    INPUT_ENTER_BATTLE,
    INPUT_BATTLE,
    INPUT_INPUT_STATE_SIZE,
} InputState;

/**********************************************************************************************************************/
/** Instrument types
**********************************************************************************************************************/
typedef enum
{
    VIOLIN = 0,
    PIANO = 1,
    FLUTE = 2,
    DRUM = 3,
} Instruments;

typedef enum
{
    ARTICULATION_NONE = 0,
    ARTICULATION_STACCATO = 1,
    ARTICULATION_LEGATO = 2,
    ARTICULATION_SIZE = 3,
} Articulation;