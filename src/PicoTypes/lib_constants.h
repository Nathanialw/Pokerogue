//
// Created by nathanial on 5/2/26.
//
#pragma once

#define MAX_VOICES 4

#define RES_320x240
// #define RES_320x320

#if defined(RES_320x240)
#define TFT_W 320
#define TFT_H 240
#elif defined(RES_320x320)
#define TFT_W 320
#define TFT_H 320
#else
#error "Define RES_320x240 or RES_320x320"
#endif


#define TRANSPARENCY 0xF81F

/**********************************************************************************************************************/
/**
**********************************************************************************************************************/
#if defined(RES_320x240)
#define BUFFER_H (16)
#define BUFFER_W (392)
#elif defined(RES_320x320)
#define BUFFER_H (60)
#define BUFFER_W (320)
#else
#error "Define RES_320x240 or RES_320x320"
#endif

#define BUFFER_SIZE_FRONT (BUFFER_W * BUFFER_H)
#define BUFFER_SIZE_BACK (BUFFER_W * BUFFER_H)
#define BUFFER_SIZE_1BYTE ((BUFFER_W * BUFFER_H) * 4)
#define BUFFER_SIZE_2BYTES ((BUFFER_W * BUFFER_H) * 2)
