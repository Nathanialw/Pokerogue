#pragma once
#include <stdint.h>

#include "lib_types.h"


void Pico_ili9341_Init(void);

void FillScreen(uint16_t rgb565);
void FillScreenColor(Color rgb565);
void ClearBuffer();
void DrawBuffer(const FrameBuffer f);

void Pico_ili9341_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color rgb565);
void Pico_ili9341_Blit(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t* data);
void Pico_ili9341_BlitKeyed(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t* data);
void Pico_DrawSprite(const FrameBuffer f, const uint8_t* sprite);

void DrawToBuffer(const FrameBuffer frameBuffer, const uint16_t* pixels, const Rect_16 rect);

void Pico_TestFrameBuffer();
void Pico_TestAnimation(FrameBuffer f, Rect_16 r, uint16_t color1);
void Pico_TestColors();
void Pico_TestText();
