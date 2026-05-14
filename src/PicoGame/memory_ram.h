//
// Created by nathanial on 4/30/26.
//
#pragma once
#include "pico_constants.h"
#include "lib_types.h"

#define MAX_OVERLAYS 256

typedef struct __attribute__((packed))
    {
    uint32_t addr; //memory adress
    uint32_t size;
} Overlay;

typedef union
{
    Overlay overlay[MAX_OVERLAYS];
    uint8_t bytes[4 * 4 * MAX_OVERLAYS];
} OverlayData;

typedef struct
{
    OverlayData overlays;
    PartialFrameBuffer frameBuffer;
    // uint8_t game_logic[32 * 1024];
    // uint8_t game_music[6 * 1024];
    // uint8_t game_aa[1 * 1024];

    struct
    {
        KeyState keyState;
        uint8_t defaultPollingRate;
        uint16_t pollingRate;
    } input;

    struct
    {
        uint16_t master_volume;
    } sound;
} Data;

extern Data g_pico_ram;

void InitRam();
