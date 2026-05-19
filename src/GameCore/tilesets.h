//
// Created by nathanial on 2/23/26.
//
#pragma once
#include "types.h"


void CharFromGlyph1bpp(MemoryInterface memory, Glyph buffer, uint16_t* character, uint8_t glyph_index, FontSize fontSize, uint16_t fg, uint16_t bg);
void TilesetFromGlyph1bpp(TileSet *tileset, uint16_t tile_id, uint8_t glyph_index, uint16_t fg, uint16_t bg);
void Expand4bppPackedToRGB(const uint16_t* src, const uint16_t* pal, uint16_t* dest);
uint8_t Expand4bppPackedToByte(const uint8_t* src, const uint16_t* pal, uint16_t* dest);