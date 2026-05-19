//
// Created by nathanial on 3/15/26.
//

#include "sound.h"
#include "lib_enums.h"

#include <stdlib.h>

#include "types.h"


/**********************************************************************************************************************/
/*  Musical configuration levers
**********************************************************************************************************************/
#define ROOT_NOTE            NOTE_A5
// #define ROOT_NOTE            NOTE_A4
// #define ROOT_NOTE            NOTE_A3
// #define ROOT_NOTE            NOTE_A2
// #define ROOT_NOTE            NOTE_A1
#define SCALE_LENGTH         7

#define MapDescend_BIAS         60      // % chance notes move downward
#define STEP_SMALL_PROB      70      // % chance of leap
#define STEP_LARGE_PROB      10      // % chance of leap
#define STEP_REPEAT_PROB     20      // % chance repeat note

#define MAX_STEP_SMALL       2       // step size within scale
#define MAX_STEP_LARGE       4

#define PHRASE_LENGTH        8       // notes per phrase
#define PHRASE_REPEAT_PROB   60      // % chance phrase repeats

#define REST_PROBABILITY     0       // % chance of rest

#define STACCATO_PROB        0      // % chance staccato articulation

#define DEFAULT_INSTRUMENT   VIOLIN
#define DEFAULT_VOLUME       5


/**********************************************************************************************************************/
/*  Rhythm patterns (durations are stored as duration field values)
**********************************************************************************************************************/
#define RHYTHM_PATTERNS 6
#define MAX_PATTERN_LEN 4

SET_MEMORY(".core")
static const uint8_t RhythmPatterns[RHYTHM_PATTERNS][MAX_PATTERN_LEN] =
{
    {3, 3, 3, 3}, // slow quarters
    {7, 3, 0, 0}, // half + quarter
    {1, 1, 3, 3}, // creeping steps
    {3, 1, 1, 3}, // tense pulse
    {3, 3, 1, 1}, // ending push
    {7, 0, 0, 0} // long sustained
};


/**********************************************************************************************************************/
/*  Minor scale (relative offsets)
**********************************************************************************************************************/
SET_MEMORY(".core")
static const int8_t Scale[SCALE_LENGTH] = {
    0, 2, 3, 5, 7, 8, 10
};


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
static int RandPercent(HardwareInterface hardware)
{
    return hardware.GetRandom_uint8_t(0, 100);
}


/**********************************************************************************************************************/
/*  Choose next scale movement
**********************************************************************************************************************/
static int NextScaleIndex(HardwareInterface hardware, int current)
{
    int r = hardware.GetRandom_uint8_t(0, 100);

    int step = 0;

    if (r < STEP_SMALL_PROB)
        step = hardware.GetRandom_uint8_t(0, MAX_STEP_SMALL * 2 + 1) - MAX_STEP_SMALL;
    else if (r < STEP_SMALL_PROB + STEP_LARGE_PROB)
        step = hardware.GetRandom_uint8_t(0, MAX_STEP_LARGE * 2 + 1) - MAX_STEP_LARGE;

    if (hardware.GetRandom_uint8_t(0, 100) < MapDescend_BIAS)
        step = -hardware.Abs(step);

    current += step;

    if (current < 0) current = 0;
    if (current >= SCALE_LENGTH) current = SCALE_LENGTH - 1;

    return current;
}

/**********************************************************************************************************************/
/*  Generate melody
**********************************************************************************************************************/
void GenerateDungeonMelody(HardwareInterface hardware, Note* melody)
{
    int scale_index = SCALE_LENGTH / 2;

    int rhythm_pattern = hardware.GetRandom_uint8_t(0, RHYTHM_PATTERNS);
    int rhythm_pos = 0;
    int duration = 0;

    for (int i = 0; i < GENERATED_MELODY_LENGTH; i++)
    {
        Note n = {0};

        if (hardware.GetRandom_uint8_t(0, 100) < REST_PROBABILITY)
        {
            n.rest = 1;
        }
        else
        {
            scale_index = NextScaleIndex(hardware, scale_index);

            n.note = ROOT_NOTE + Scale[scale_index];
            n.rest = 0;
        }

        n.duration = RhythmPatterns[rhythm_pattern][rhythm_pos];

        rhythm_pos++;
        if (rhythm_pos >= MAX_PATTERN_LEN ||
            RhythmPatterns[rhythm_pattern][rhythm_pos] == 0)
        {
            rhythm_pattern = hardware.GetRandom_uint8_t(0, RHYTHM_PATTERNS);
            rhythm_pos = 0;
        }

        n.instrument = DEFAULT_INSTRUMENT;
        n.volume = DEFAULT_VOLUME;

        if (hardware.GetRandom_uint8_t(0, 100) < STACCATO_PROB)
            n.articulation = ARTICULATION_STACCATO;
        else
            n.articulation = ARTICULATION_LEGATO;

        duration += n.duration;
        melody[i] = n;
    }

    // int note_count = duration / 16;
    //
    // for (int i = 0; i < note_count; i++)
    // {
    //     Note n = {0};
    //     n.note = NOTE_A4;
    //     n.duration = 15;
    //     n.volume = 1;
    //     n.instrument = 0;
    //     n.articulation = ARTICULATION_NONE;
    //     melody[GENERATED_MELODY_LENGTH + i] = n;
    // }
    // DEBUG("total %d avg %f", note_count, (float)note_count/GENERATED_MELODY_LENGTH);
}


/**********************************************************************************************************************/
/**  Fill the Music struct data and return it
**********************************************************************************************************************/
MusicData InitMusicData()
{
    MusicData music_data = {
        .numVoices = 1,
        .offset = {0, GENERATED_MELODY_LENGTH},
        .length = {GENERATED_MELODY_LENGTH, GENERATED_MELODY_LENGTH},
        .delay = {0, 0}
    };

    return music_data;
}
