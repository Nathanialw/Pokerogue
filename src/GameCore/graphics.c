//
// Created by nathanial on 4/11/26.
//
#include <stdint.h>
#include <string.h>

#include "graphics.h"


#include "tilesets.h"
#include "map.h"
#include "camera.h"
#include "memory_rom.h"
#include "memory_ram.h"
#include "menu.h"
#include "types.h"
#include "enums.h"
#include "menu_battle.h"
#include "stats.h"
#include "entities.h"
#include "lib_debugging.h"
#include "player.h"
#include "utils.h"
#include "lib_decl.h"

/**********************************************************************************************************************/
/**  copy the given rect of the give pixel array into the given clip pixel array
**********************************************************************************************************************/
void ClipTile(uint16_t* clip, const uint16_t* pixels, Rect_16 r)
{
    for (int y = 0; y < TILE_W; ++y)
    {
        for (int x = 0; x < TILE_H; ++x)
        {
            clip[(TILE_W * y) + x] = pixels[(TILE_H * y) + x];
        }
    }
}

/**********************************************************************************************************************/
/**  Blit the given tile id to the given screen coords
**********************************************************************************************************************/
void DrawChar(uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id)
{
    uint16_t px = (uint16_t)(screen_tx * TILE_W);
    uint16_t py = (uint16_t)(screen_ty * TILE_H);

    g_run.tileCache.tile_id = tile_id;
    const Tile t = g_gameFlash.sprites.biomes[g_run.biome][tile_id];
    CharFromGlyph1bpp(g_run.tileCache.tilePixels.pixels, t.glyph_index, FONT16x16, g_gameFlash.GetColor[t.fg], g_gameFlash.GetColor[t.bg]);
    DrawTileKeyed(px, py, TILE_W, TILE_H, g_run.tileCache.tilePixels.pixels);
}

/**********************************************************************************************************************/
/**  Blit the given tile id to the given screen coords
**********************************************************************************************************************/
void DrawTile(uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id)
{
    uint16_t px = (uint16_t)(screen_tx * TILE_W);
    uint16_t py = (uint16_t)(screen_ty * TILE_H);

    g_run.tileCache.tile_id = tile_id;
    const Tile t = g_gameFlash.sprites.biomes[g_run.biome][tile_id];
    CharFromGlyph1bpp(g_run.tileCache.tilePixels.pixels, t.glyph_index, FONT16x16, g_gameFlash.GetColor[t.fg], g_gameFlash.GetColor[t.bg]);
    DrawTileKeyed(px, py, TILE_W, TILE_H, g_run.tileCache.tilePixels.pixels);
}

/**********************************************************************************************************************/
/**  Blit the given creature id to the given screen coords
**********************************************************************************************************************/
void DrawMonster(uint8_t screen_tx, uint8_t screen_ty, Creature sprite_id, const Sprite* spriteArray)
{
    uint16_t px = (uint16_t)(screen_tx * TILE_W);
    uint16_t py = (uint16_t)(screen_ty * TILE_H);

    g_run.tileCache.sprite_id = sprite_id;
    const Sprite s = spriteArray[sprite_id];
    CharFromGlyph1bpp(g_run.tileCache.spritePixels.pixels, s.glyph_index, FONT16x16, g_gameFlash.GetColor[s.fg], g_gameFlash.GetColor[PAL_KEY]);
    DrawTileKeyed(px, py, TILE_W, TILE_H, g_run.tileCache.spritePixels.pixels);
}

/**********************************************************************************************************************/
/**  Checks cache
 *  Updates cache
 *  Blit the given tile id to the given screen coords
**********************************************************************************************************************/
void DrawTileCached(uint8_t screen_tx, uint8_t screen_ty, uint8_t tile_id)
{
    uint16_t px = (uint16_t)(screen_tx * TILE_W);
    uint16_t py = (uint16_t)(screen_ty * TILE_H);

    if (g_run.tileCache.tile_id != tile_id)
    {
        g_run.tileCache.tile_id = tile_id;
        const Tile t = g_gameFlash.sprites.biomes[g_run.biome][tile_id]; //TODO: FIX
        CharFromGlyph1bpp(g_run.tileCache.tilePixels.pixels, t.glyph_index, FONT16x16, g_gameFlash.GetColor[t.fg], g_gameFlash.GetColor[t.bg]);
    }
    DrawTileKeyed(px, py, TILE_W, TILE_H, g_run.tileCache.tilePixels.pixels);
}

/**********************************************************************************************************************/
/**  Checks cache
 *  Updates cache
 *  Blit the given creature id to the given screen coords
**********************************************************************************************************************/
void DrawMonsterCached(uint8_t screen_tx, uint8_t screen_ty, uint8_t sprite_id, const Sprite* spriteArray)
{
    uint16_t px = (uint16_t)(screen_tx * TILE_W);
    uint16_t py = (uint16_t)(screen_ty * TILE_H);

    if (g_run.tileCache.sprite_id != sprite_id)
    {
        g_run.tileCache.sprite_id = sprite_id;
        const Sprite s = spriteArray[sprite_id];
        CharFromGlyph1bpp(g_run.tileCache.spritePixels.pixels, s.glyph_index, FONT16x16, g_gameFlash.GetColor[s.fg], g_gameFlash.GetColor[PAL_KEY]);
    }

    DrawTileKeyed(px, py, TILE_W, TILE_H, g_run.tileCache.spritePixels.pixels);
}


/**********************************************************************************************************************/
/**  copies text pixel data of the given char array into the buffer then draws the buffer at the given screen position
**********************************************************************************************************************/
uint8_t PrintLineStr(uint16_t x, uint16_t y, FontSize fontSize, uint8_t maxChars, const char* textLine, bool indent)
{
    uint8_t text_size;
    uint8_t char_idx = 0;
    Glyph16x16 glyph;
    uint8_t c = CH_NONE;

    if (fontSize == FONT8x8)
        text_size = TEXT_W;
    else
        text_size = TILE_W;

    if (indent)
        x += text_size;

    while (textLine[char_idx])
        char_idx++;

    maxChars = char_idx;
    char_idx = 0;

    SetFrameBuffer(0xd6fa); // gray
    FrameBuffer frameBuffer = {x, y, maxChars * text_size, text_size, 0xd6fa};

    while (textLine[char_idx] && char_idx < maxChars)
    {
        char nc = textLine[char_idx] - FONT_OFFSET;
        if (c != nc)
        {
            c = nc;
            CharFromGlyph1bpp(glyph.pixels, c, fontSize, g_gameFlash.GetColor[PAL_DARK_BLUE_GRAY], g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);
        }

        uint16_t char_pos = (char_idx * text_size);
        Rect_16 rect = {char_pos, 0, text_size, text_size};

        DrawToBuffer(frameBuffer, glyph.pixels, rect);
        char_idx++;
    }

    DrawBuffer(frameBuffer);
    return text_size;
}

/**********************************************************************************************************************/
/**  Check whether cursor should be drawn
**********************************************************************************************************************/
bool HideCursor()
{
    if (g_run.menu.displayedMenu == MINIMAP)
    {
        return true;
    }
    return false;
}

/**********************************************************************************************************************/
/**  Draw menu cursor at the cached cursor location
**********************************************************************************************************************/
void DrawCursor()
{
    if (HideCursor()) return;

    const uint16_t x = g_run.menu.x * TILE_W;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t list_y = g_run.menu.y * size;

    uint8_t sel_y = GetSelectorY();


    FillRect(x, list_y + (g_run.menu.eraseSel.y * (size + g_run.menu.lineHeight)), size, size, g_run.menu.colorCache);

    if (font_size == FONT8x8)
    {
        Glyph8x8 character;
        CharFromGlyph1bpp(character.pixels, '>' - FONT_OFFSET, font_size, g_gameFlash.GetColor[PAL_DARK_BLUE_GRAY], g_gameFlash.GetColor[PAL_KEY]);
        DrawTileKeyed(x, list_y + (sel_y * (size + g_run.menu.lineHeight)), size, size, character.pixels);
    }
    else
    {
        Glyph16x16 character;
        CharFromGlyph1bpp(character.pixels, '>' - FONT_OFFSET, font_size, g_gameFlash.GetColor[PAL_DARK_BLUE_GRAY], g_gameFlash.GetColor[PAL_KEY]);
        DrawTileKeyed(x, list_y + (sel_y * (size + g_run.menu.lineHeight)), size, size, character.pixels);
    }
}

/**********************************************************************************************************************/
/**  sorts units into the order they are drawn into the partial frame buffer for the minimap
**********************************************************************************************************************/
void OrderUnitsByBufferLine(EntityId* units, uint8_t* meta)
{
    for (uint16_t id = 0; id < ENTITY_COUNT; ++id)
    {
        if (!GetBit(g_run.creatures.onMap, id)) continue;
        Position pos = g_run.creatures.position[id];
        uint8_t row = pos.y / GetBufferWidth();

        uint16_t cursor = 0;
        for (uint8_t i = 0; i <= row; i++)
            cursor += meta[i];

        EntityId cache1 = units[cursor];
        units[cursor] = id;
        meta[row]++;

        for (uint16_t i = cursor + 1; i < ENTITY_COUNT; i++)
        {
            if (cache1 == NO_ENTITY) break;
            EntityId cache2 = units[i];
            units[i] = cache1;
            cache1 = cache2;
        }
    }
}


/**********************************************************************************************************************/
/**  Draws each tile as a pixel on the screen
 *  colour coded
 *  draws creature position pixels on top of their tile position
**********************************************************************************************************************/
void DrawMiniMap()
{
    DEBUG("==== DRAWING MINIMAP ====   \n");


    uint8_t colors[16] =
    {
        PAL_PEACH_SKIN,
        PAL_MEDIUM_BROWN,
        PAL_PALE_GREEN,
        PAL_GRAYISH_GREEN,
        PAL_DIRTY_YELLOW_GRN,
        PAL_BROWNISH_RED,
        PAL_LIGHT_TAN,
        PAL_GRAY_BLUE,
        PAL_DULL_ORANGE,
        PAL_DARK_BROWN,
        PAL_MUTED_GREEN,
        PAL_DARK_GRAY_GREEN,
        PAL_GRAY_BROWN,
        PAL_LIGHT_GRAY,
        PAL_TAN_BROWN,
        PAL_BRIGHT_LIGHT_GRN,
    };


    const uint16_t screen_w = 320;
    const uint8_t buffer_lines = 320 / GetBufferHeight();
    const uint8_t centerOffset = 32;

    uint8_t meta[buffer_lines];
    memset(meta, 0, sizeof(meta));
    EntityId units[ENTITY_COUNT];
    memset(units, NO_ENTITY, sizeof(units));
    OrderUnitsByBufferLine(units, meta);

    uint16_t cursor = 0;
    uint16_t transparency = g_gameFlash.GetColor[PAL_KEY];
    for (uint16_t y = 0; y < MAP_H; y += GetBufferHeight())
    {
        SetFrameBuffer(g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);
        memset(GetFrameBuffer2bytes(), g_gameFlash.GetColor[PAL_DARK_GRN_BLACK], sizeof(*GetFrameBuffer2bytes()));
        cursor = (screen_w - MAP_W) / 2; //reset position
        for (uint16_t row = 0; row < GetBufferHeight(); row++)
        {
            uint16_t cy = y + row;
            if (cy >= MAP_H) break;
            for (uint16_t x = 0; x < MAP_W; x++)
            {
                uint16_t color = g_gameFlash.GetColor[colors[GetMapTile(x, cy)]];
                if (color == transparency) continue;
                GetFrameBuffer2bytes()[cursor++] = color;
            }
            cursor += (screen_w - MAP_W);
        }

        uint16_t c = 0;
        for (uint8_t i = 0; i < y / GetBufferHeight(); i++) c += meta[i];

        uint8_t buffer_line = y / GetBufferHeight();
        uint8_t n = meta[buffer_line];
        for (uint16_t j = c; j < c + n; j++)
        {
            uint16_t color = g_gameFlash.GetColor[PAL_BRIGHT_RED];
            Position pos = g_run.creatures.position[units[j]];
            uint8_t row = pos.y - y;
            if (GetPlayerID() == units[j])
                color = g_gameFlash.GetColor[PAL_DEEP_BLUE];

            GetFrameBuffer2bytes()[centerOffset + (row * (centerOffset * 2)) + (row * MAP_W) + pos.x] = color;
        }

        Draw(0, y, screen_w, GetBufferWidth(), GetFrameBuffer1byte());
    }

    DEBUG("==== DRAWING MINIMAP DONE! ====   \n");
}

/**********************************************************************************************************************/
/**  Draws the player party frame
 *  Draws each creature name and their resources
**********************************************************************************************************************/
void DrawParty()
{
    DEBUG("==== DRAWING PARTY ====   \n");

    const uint16_t x = MAIN_MENU_X * TILE_W;
    uint8_t y = MAIN_MENU_Y * TILE_H;
    const uint16_t w = MAIN_MENU_W * TILE_W;
    const uint8_t h = MAIN_MENU_H * TILE_H;
    FillRect(x, y, w, h, g_gameFlash.GetColor[PAL_LIGHT_GRAY]);

    const uint8_t indent = 1;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t max_lines = (MAIN_MENU_H * font_size);
    const uint8_t max_chars = (((MAIN_MENU_W) * font_size) - indent) + 1;

    char border[max_chars + 1];
    memset(border, '-', max_chars);
    border[max_chars] = '\0';

    y += PrintLineStr(x, y, font_size, max_chars, border, false);
    const uint8_t list_y = y;

    uint8_t lineHeight = 0;
    uint8_t i = 0;
    while (i < MAX_PARTY_SIZE)
    {
        lineHeight = 0;
        DEBUG("const char* line = GetMenuLine(i);");
        const char* line = GetMenuLine(i);
        DEBUG("drawing ----   %s", line);
        if (!line || i > (max_lines)) break;


        //level
        if (GetCreatureType(g_run.player.partyID[i]) == NO_CREATURE)
        {
            PrintLineStr(x, y, font_size, 3, "---", indent);
        }
        else
        {
            Int99 level = g_run.creatures.level[g_run.player.partyID[i]];
            DEBUG("level %d", level.value);
            CharStr_99 levelStr;
            GetAsChars_99(level, &levelStr, false);
            DEBUG("levelStr %s", levelStr);
            PrintLineStr(x, y, font_size, 3, levelStr, indent);
        }

        y += PrintLineStr(x + (3 * size), y + lineHeight, font_size, max_chars, line, indent);

        if (GetCreatureType(g_run.player.partyID[i]) == NO_CREATURE)
        {
            lineHeight += (size * 3);
            lineHeight += (size / 2);
            y += lineHeight;
            i++;
            continue;
        }

        //hunger?

        uint16_t cur = 0;
        uint16_t max = 0;
        float line_w = 0.0f;

        IntMax999 hp = g_run.creatures.hp[g_run.player.partyID[i]];
        cur = Int999GetCurrent(&hp);
        max = Int999GetMax(&hp);
        line_w = (((float)cur / (float)max) * ((float)(max_chars - 2) * (float)size));
        if (line_w > 2) line_w -= 2;
        FillRect(x + size, y + lineHeight, (max_chars - 2) * size, size, g_gameFlash.GetColor[PAL_DARK_GRN_BLACK]);
        FillRect(x + 1 + size, y + lineHeight + 1, ((max_chars - 2) * size) - 2, size - 2, g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);
        FillRect(x + 1 + size, y + lineHeight + 1, (uint16_t)line_w, size - 2, g_gameFlash.GetColor[PAL_EMERALD_GREEN]);
        lineHeight += size;

        IntMax999 mp = g_run.creatures.mp[g_run.player.partyID[i]];
        cur = Int999GetCurrent(&mp);
        max = Int999GetMax(&mp);
        line_w = (((float)cur / (float)max) * ((float)(max_chars - 2) * (float)size));
        if (line_w > 2) line_w -= 2;
        FillRect(x + size, y + lineHeight, (max_chars - 2) * size, size, g_gameFlash.GetColor[PAL_DARK_GRN_BLACK]);
        FillRect(x + 1 + size, y + lineHeight + 1, ((max_chars - 2) * size) - 2, size - 2, g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);
        FillRect(x + 1 + size, y + lineHeight + 1, (uint16_t)line_w, size - 2, g_gameFlash.GetColor[PAL_ICE_BLUE]);
        lineHeight += size;

        IntMax999 xp = g_run.creatures.xp[g_run.player.partyID[i]];
        cur = Int999GetCurrent(&xp);
        max = Int999GetMax(&xp);
        line_w = (((float)cur / (float)max) * ((float)(max_chars - 2) * (float)size));
        if (line_w > 2) line_w -= 2;
        FillRect(x + size, y + lineHeight, (max_chars - 2) * size, size, g_gameFlash.GetColor[PAL_DARK_GRN_BLACK]);
        FillRect(x + 1 + size, y + lineHeight + 1, ((max_chars - 2) * size) - 2, size - 2, g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);
        FillRect(x + 1 + size, y + lineHeight + 1, (uint16_t)line_w, size - 2, g_gameFlash.GetColor[PAL_MEDIUM_PURPLE]);

        lineHeight += size;
        lineHeight += (size / 2);
        y += lineHeight;
        i++;
    }
    DEBUG(" + lineHeight %d", lineHeight);
    g_run.menu.lineHeight = lineHeight;

    PrintLineStr(x, (max_lines - 1) * size, font_size, max_chars, border, false);

    g_run.menu.colorCache = g_gameFlash.GetColor[PAL_LIGHT_GRAY];
}


/**********************************************************************************************************************/
/**  Draws the cached char arrays menu list to the screen
**********************************************************************************************************************/
void DrawList()
{
    DEBUG("==== DRAWING MENU ====   \n");

    const uint16_t x = MAIN_MENU_X * TILE_W;
    uint8_t y = MAIN_MENU_Y * TILE_H;
    const uint16_t w = MAIN_MENU_W * TILE_W;
    const uint8_t h = MAIN_MENU_H * TILE_H;
    FillRect(x, y, w, h, g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);

    const uint8_t indent = 1;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t max_lines = (MAIN_MENU_H * font_size);
    const uint8_t max_chars = (((MAIN_MENU_W) * font_size) - indent) + 1;

    char border[max_chars + 1];
    memset(border, '-', max_chars);
    border[max_chars] = '\0';

    y += PrintLineStr(x, y, font_size, max_chars, border, false);
    const uint8_t list_y = y;

    uint8_t i = 0;
    while (1)
    {
        DEBUG("const char* line = GetMenuLine(i);");
        const char* line = GetMenuLine(i);
        DEBUG("drawing ----   %s", line);
        if (!line || i > (max_lines)) break;
        y += PrintLineStr(x, y, font_size, max_chars, line, indent);
        i++;
    }

    uint16_t numSpaces = (max_lines - i - 2) * size;
    PrintLineStr(x, y + numSpaces, font_size, max_chars, border, false);

    g_run.menu.colorCache = g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY];
}

/**********************************************************************************************************************/
/**  handles while menu selection to draw
**********************************************************************************************************************/
void HandleMenu()
{
    if (g_run.menu.displayedMenu == g_run.menu.selectedMenu && (g_run.menu.menuScrollOffset[g_run.menu.depth].y <= 0 && !g_run.menu.forceRedraw)) return;
    g_run.menu.displayedMenu = g_run.menu.selectedMenu;
    g_run.menu.forceRedraw = false;

    if (g_run.menu.displayedMenu == MINIMAP) // draw minimap
    {
        DrawMiniMap();
        return;
    }

    if (g_run.menu.displayedMenu == PARTY || g_run.menu.useOnPartyMember) //draw party
    {
        DrawParty();
        return;
    }

    DrawList();
}

/**********************************************************************************************************************/
/**  Draws battle mode menu list - ie bag items, party, spells
**********************************************************************************************************************/
void HandleBattleLists()
{
    // use screen area of player battler and down to the bottom of the skill list
    const uint16_t x = PLAYER_BATTLER_FRAME.x;
    const uint8_t y = PLAYER_BATTLER_FRAME.y;
    const uint16_t w = PLAYER_BATTLER_FRAME.w;

    const uint8_t indent = 1;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t max_chars = w / 8;
    const uint8_t max_lines = (MAIN_MENU_H * font_size);

    //display menu text
    uint8_t list_y = y + size;
    uint8_t i = 0;

    while (1)
    {
        DEBUG("const char* line = GetMenuLine(i);");
        const char* line = GetMenuLine(i);
        DEBUG("drawing ----   %s", line);
        if (!line || i > (max_lines)) break;

        // if selected into SwapMenu
        if (g_run.menu.sel[0].y == 0) // drawing party
        {
            //level //name
            list_y += PrintLineStr(x, list_y, font_size, max_chars, line, indent);

            //health //mana
            uint8_t rect_w = w / 2;
            //TODO: replace with actual values
            FillRect(x, list_y, rect_w, size, g_gameFlash.GetColor[PAL_EMERALD_GREEN]);
            FillRect(x + rect_w, list_y, rect_w, size, g_gameFlash.GetColor[PAL_ICE_BLUE]);

            g_run.menu.lineHeight = size + (size / 2);
            list_y += g_run.menu.lineHeight;
        }
        else // drawing general list
        {
            // name
            list_y += PrintLineStr(x, list_y, font_size, max_chars, line, indent);
        }
        i++;
    }
}

/**********************************************************************************************************************/
/**  Draws the back panel of the battle menu list and calls the draw battle list function
**********************************************************************************************************************/
void HandleBattleMenu()
{
    if (g_run.menu.depth == 0) return;


    // use screen area of player battler and down to the bottom of the skill list
    const uint16_t x = PLAYER_BATTLER_FRAME.x;
    const uint8_t y = PLAYER_BATTLER_FRAME.y;
    const uint16_t w = PLAYER_BATTLER_FRAME.w;
    const uint8_t h = PLAYER_BATTLER_FRAME.h + (8 * 6);
    FillRect(x, y, w, h, g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);

    const uint8_t indent = 1;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t max_chars = w / 8;
    const uint8_t max_lines = (MAIN_MENU_H * font_size);

    char border[max_chars + 1];
    memset(border, '-', max_chars);
    border[max_chars] = '\0';

    HandleBattleLists();

    //borders
    PrintLineStr(x, y, font_size, max_chars, border, false);
    PrintLineStr(x, y + (h - size), font_size, max_chars, border, false);

    g_run.menu.colorCache = g_gameFlash.GetColor[PAL_LIGHT_GRAY];
}


/**********************************************************************************************************************/
/**     Clears the Buffer to gray
 *      Draws a multitile sprite to a buffer one TILE_W x TILE_H tile at a tile
 *      Draws the buffer to the screen
**********************************************************************************************************************/
FrameBuffer DrawBattlerToBuffer(uint16_t screen_x, uint16_t screen_y, const SpriteLayout layout, const uint8_t* sprite)
{
    Glyph16x16 temp; // temp RGB565 buffer (512 bytes on stack)

    uint32_t byte_offset = 0; // starts at beginning of this sprite's data
    uint8_t tile_counter = 0;

    uint16_t pixel_w = BATTLER_TILES_W * TILE_W;
    uint16_t pixel_h = BATTLER_TILES_H * TILE_H;

    FrameBuffer f = {screen_x, screen_y, pixel_w, pixel_h};
    SetFrameBuffer(g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);

    while (tile_counter < BATTLER_TILES_H * BATTLER_TILES_W)
    {
        // DEBUG("while (tile_counter < BATTLER_TILES_H * BATTLER_TILES_W)");
        int row = tile_counter / BATTLER_TILES_H;
        int col = tile_counter % BATTLER_TILES_W;

        uint8_t tile_idx = row * BATTLER_TILES_W + col;
        uint8_t mask_byte = tile_idx / 8;
        uint8_t mask_bit = tile_idx % 8;
        // Check if this tile position is suppposed to be drawn
        if ((layout.emptyIndexes[mask_byte] & (1u << mask_bit)) == 0)
        {
            tile_counter++;
            continue;
        }

        int draw_x = col * TILE_W;
        int draw_y = row * TILE_H;

        const uint8_t* packed_tile = sprite + layout.idx + byte_offset;

        // Decompress and get how many compressed bytes were used
        uint8_t bytes_used = Expand4bppPackedToByte(packed_tile, layout.palette, temp.pixels);

        // Advance offset for NEXT non-empty tile
        byte_offset += bytes_used;

        Rect_16 r = {draw_x, draw_y, TILE_W, TILE_H};
        DrawToBuffer(f, temp.pixels, r);
        // DEBUG("DrawBattler");

        tile_counter++;
    }

    return f;
}

/**********************************************************************************************************************/
/**     Clears the Buffer to gray
 *      Draws a multitile sprite to a buffer one TILE_W x TILE_H tile at a tile
 *      Draws the buffer to the screen
**********************************************************************************************************************/
void DrawBattler(uint16_t screen_x, uint16_t screen_y, const SpriteLayout layout, const uint8_t* sprite)
{
    FrameBuffer f = DrawBattlerToBuffer(screen_x, screen_y, layout, sprite);
    DrawBuffer(f);
}

/**********************************************************************************************************************/
/**  Draws the background panel to the screen for the "pokedex" style encyclopedias
**********************************************************************************************************************/
void HandleGameMenuLeftBG(const uint16_t x, uint8_t y)
{
    const uint16_t w = (VIEW_TW * TILE_W) - (MAIN_MENU_W * TILE_W);
    const uint8_t h = (VIEW_TH * TILE_H);
    FillRect(x, y, w, h, g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);
}

/**********************************************************************************************************************/
/**  Draws the sprite to the screen for the "pokedex" style encyclopedias
**********************************************************************************************************************/
void HandleGameMenuLeftBattler(const uint16_t x, uint8_t y, uint8_t size, const SpriteLayout* spriteData, const uint8_t* pixels)
{
    DrawBattler(x, y + (size), *spriteData, pixels);
}

/**********************************************************************************************************************/
/**  Draws the name text to the screen for the "pokedex" style encyclopedias
**********************************************************************************************************************/
void HandleGameMenuLeftName(const uint16_t x, uint8_t y, const char* name)
{
    const uint8_t indent = 1;
    PrintLineStr(x, y, g_run.settings.fontSize, SMALL_STRINGS, name, indent);
}

/**********************************************************************************************************************/
/**  Draws the rarity and power bars to the screen for the "pokedex" style encyclopedias
**********************************************************************************************************************/
void HandleGameMenuLeftStat(const uint16_t x, const uint16_t x2, uint8_t y, uint8_t size, const char* power_str)
{
    const uint8_t indent = 1;

    //strength line
    float power = 115;
    float max_power = 255;
    // power
    PrintLineStr(x, y, g_run.settings.fontSize, 10, power_str, indent);

    //strength bar
    float f_bar_width = power / max_power;
    uint8_t bar_width = (uint8_t)((float)(4 * size) * f_bar_width);
    FillRect(x2, y, (4 * size), size, g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]);
    FillRect(x2, y, bar_width, size, g_gameFlash.GetColor[PAL_BROWN_GREEN]);
}

/**********************************************************************************************************************/
/**  Draws the borders to the screen for the "pokedex" style encyclopedias
**********************************************************************************************************************/
void HandleGameMenuBorders()
{
    const uint16_t x = 0;
    uint8_t y = 0;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t max_lines = (VIEW_TH * font_size);
    const uint8_t max_chars = (((VIEW_TW) * font_size)) + 1;

    char border[max_chars + 1];
    memset(border, '-', max_chars);
    border[max_chars] = '\0';
    PrintLineStr(x, y, font_size, max_chars, border, false);
    uint16_t numSpaces = (max_lines - 1) * size;
    PrintLineStr(x, y + numSpaces, font_size, max_chars, border, false);
}

/**********************************************************************************************************************/
/**  Draws the type info text to the screen for the "pokedex" style encyclopedias
**********************************************************************************************************************/
void HandleGameMenuTypes(const char* typeA, const char* typeB)
{
    if (!typeA || !typeB) return;

    const uint16_t x = 0;
    uint8_t y = 0;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t indent = 1;


    if (typeA)
        PrintLineStr(x + (size * 10), y + (size * 18), font_size, 10, typeA, indent);
    if (typeB)
        PrintLineStr(x + (size * 10), y + (size * 20) - 4, font_size, 10, typeB, indent);
}

/**********************************************************************************************************************/
/**  Draws the description text to the screen for the "pokedex" style encyclopedias
 *  Handles line breaks and word breaks
**********************************************************************************************************************/
void HandleGameMenuDescription(uint16_t y, const char* desc)
{
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    const uint8_t max_chars = (VIEW_TW * font_size) + 1;
    const uint8_t indent = 1;
    const uint8_t MAX_LINES = 8;
    const uint8_t LINE_LEN = 20;

    uint8_t desc_x = 1;
    uint8_t desc_y = y;

    if (!desc) return;

    bool done = false;
    uint16_t cursor = 0;
    uint8_t lines_printed = 0;

    for (uint8_t i = 0; i < MAX_LINES; i++)
    {
        if (done) break;

        char line[LINE_LEN + 2]; // +2 for possible hyphen and null
        memset(line, ' ', LINE_LEN);
        line[LINE_LEN] = '\0';
        line[LINE_LEN + 1] = '\0';

        // Fill line
        uint8_t j = 0;
        while (j < LINE_LEN && !done)
        {
            // Skip leading spaces
            if (j == 0 && desc[cursor] == ' ')
            {
                cursor++;
                continue;
            }

            if (desc[cursor] == '\0')
            {
                done = true;
                break;
            }

            line[j] = desc[cursor];
            cursor++;
            j++;
        }

        // Word wrap/hyphenation if needed
        if (!done && j == LINE_LEN)
        {
            // First try to find a natural break (space or hyphen)
            int8_t break_pos = -1;
            for (int8_t k = LINE_LEN - 1; k >= 0; k--)
            {
                if (line[k] == ' ' || line[k] == '-')
                {
                    break_pos = k;
                    break;
                }
            }

            if (break_pos >= 0)
            {
                // Natural break found
                cursor -= (LINE_LEN - break_pos - 1);

                if (line[break_pos] == '-')
                {
                    line[break_pos + 1] = '\0';
                }
                else // Space
                {
                    line[break_pos] = '\0';
                }
            }
            else
            {
                // No natural break - try hyphenation
                // Find the current word we're in
                uint8_t word_start = 0;
                for (int8_t k = LINE_LEN - 1; k >= 0; k--)
                {
                    if (line[k] == ' ')
                    {
                        word_start = k + 1;
                        break;
                    }
                }

                uint8_t word_len = LINE_LEN - word_start;

                // Look for a hyphenation point in the last 4-5 characters
                int8_t hyphen_pos = -1;
                for (int8_t k = LINE_LEN - 2; k >= word_start + 2; k--)
                {
                    if (IsHyphenationPoint(line, k, LINE_LEN))
                    {
                        hyphen_pos = k;
                        break;
                    }
                }

                if (hyphen_pos >= 0)
                {
                    // Insert hyphen and wrap
                    line[hyphen_pos] = '-';
                    line[hyphen_pos + 1] = '\0';
                    cursor -= (LINE_LEN - hyphen_pos - 1);
                }
                else
                {
                    // No hyphenation point - hard break
                    line[LINE_LEN] = '\0';
                }
            }
        }

        PrintLineStr(desc_x, desc_y + (lines_printed * size), font_size, max_chars, line, indent);
        lines_printed++;
    }
}


typedef struct
{
    const char* desc;
    const char* name;
    const SpriteLayout* layout;
    const uint8_t* sprite;
    const char* typeA;
    const char* typeB;
} EntityData;

/**********************************************************************************************************************/
/**  Retrieves the entity type data for the entity type found at the menu cursor
**********************************************************************************************************************/
EntityData FillObjectData()
{
    EntityData entityData = {};
    uint8_t menu_idx = g_run.menu.sel[0].y;
    DEBUG("menu_idx: %d\n", menu_idx);

    if (menu_idx == 1) // creature
    {
        entityData.desc = g_gameFlash.text.descriptions.monsters[g_run.menu.gameMenu.displayId];
        entityData.name = g_gameFlash.text.names.monsters[g_run.menu.gameMenu.displayId];
        entityData.layout = &g_gameFlash.spriteData.battlers.frontLayout[g_run.menu.gameMenu.displayId];
        entityData.sprite = g_gameFlash.spriteData.battlers.front;
        entityData.typeA = g_gameFlash.text.types[g_gameFlash.gameData.creatureTypes[g_run.menu.gameMenu.displayId].typeA];
        entityData.typeB = g_gameFlash.text.types[g_gameFlash.gameData.creatureTypes[g_run.menu.gameMenu.displayId].typeB];
    }
    else if (menu_idx == 2) // objects
    {
        entityData.desc = g_gameFlash.text.descriptions.objects[g_run.menu.gameMenu.displayId];
        entityData.name = g_gameFlash.text.names.objects[g_run.menu.gameMenu.displayId];
        entityData.layout = &g_gameFlash.spriteData.objectLayout[g_run.menu.gameMenu.displayId];
        entityData.sprite = g_gameFlash.spriteData.objects;
        entityData.typeA = NULL;
        entityData.typeB = NULL;
    }
    else if (menu_idx == 3) // item
    {
        entityData.desc = g_gameFlash.text.descriptions.items[g_run.menu.gameMenu.displayId];
        entityData.name = g_gameFlash.text.names.items[g_run.menu.gameMenu.displayId];
        entityData.layout = &g_gameFlash.spriteData.itemLayout[g_run.menu.gameMenu.displayId];
        entityData.sprite = g_gameFlash.spriteData.items;
        entityData.typeA = NULL;
        entityData.typeB = NULL;
    }
    else if (menu_idx == 4) //spell
    {
        entityData.desc = g_gameFlash.text.descriptions.spells[g_run.menu.gameMenu.displayId];
        entityData.name = g_gameFlash.text.names.spells[g_run.menu.gameMenu.displayId];
        entityData.layout = &g_gameFlash.spriteData.spellLayout[g_run.menu.gameMenu.displayId];
        entityData.sprite = g_gameFlash.spriteData.spells;
        entityData.typeA = NULL;
        entityData.typeB = NULL;
    }
    else if (menu_idx == 5) // skill
    {
        entityData.desc = g_gameFlash.text.descriptions.attacks[g_run.menu.gameMenu.displayId];
        entityData.name = g_gameFlash.text.names.attacks[g_run.menu.gameMenu.displayId];
        entityData.layout = &g_gameFlash.spriteData.skillLayout[g_run.menu.gameMenu.displayId];
        entityData.sprite = g_gameFlash.spriteData.skills;
        entityData.typeA = NULL;
        entityData.typeB = NULL;
    }

    return entityData;
}

/**********************************************************************************************************************/
/**  Draws the "pokedex" style encyclopedia frame
**********************************************************************************************************************/
void HandleGameMenu()
{
    if (!g_run.menu.gameMenu.open && (g_run.menu.gameMenu.displayId == g_run.menu.gameMenu.id)) return;
    g_run.menu.gameMenu.displayId = g_run.menu.gameMenu.id;

    DEBUG("==== DRAWING GAME MENU ====   \n");
    const uint16_t x = 0;
    uint8_t y = 0;
    const FontSize font_size = g_run.settings.fontSize;
    const uint8_t size = (font_size == FONT8x8) ? TEXT_W : TILE_W;
    EntityData entityData = FillObjectData();

    HandleGameMenuLeftBG(x, y);
    HandleGameMenuBorders();
    HandleGameMenuLeftBattler(x, y, size, entityData.layout, entityData.sprite);
    HandleGameMenuLeftName(x, y + (size * 16), entityData.name);
    HandleGameMenuLeftStat(x, x + (size * 6), y + (size * 18), size, "Powr:\0");
    HandleGameMenuLeftStat(x, x + (size * 6), y + (size * 20) - 4, size, "Rare:\0");
    HandleGameMenuTypes(entityData.typeA, entityData.typeB);
    HandleGameMenuDescription((size * 21), entityData.desc);
}

/**********************************************************************************************************************/
/*  Draws the formatted Creature stats in battle - hp, mama
**********************************************************************************************************************/

void CreatureStats(EntityId id, Rect_16 rect, uint8_t size, FontSize font_size)
{
    uint8_t max_chars = 12;

    DEBUG("CreatureStats");
    FillRect(rect.x, rect.y, rect.w, rect.h, g_gameFlash.GetColor[PAL_LIGHT_GRAY]);
    PrintLineStr(rect.x + TEXT_W, rect.y + TEXT_W, font_size, max_chars, g_gameFlash.text.names.monsters[GetCreatureType(id)], false);

    char status_line[max_chars];

    const uint16_t cur_hp = Int999GetCurrent(&g_run.creatures.hp[id]);
    const uint16_t max_hp = Int999GetMax(&g_run.creatures.hp[id]);
    GetStatLine(cur_hp, max_hp, max_chars, status_line, "HP:");
    PrintLineStr(rect.x + TEXT_W, rect.y + size + TEXT_W, font_size, max_chars, status_line, false);

    DEBUG("CreatureStats HP: %d/%d", cur_hp, max_hp);
    const uint16_t hp_w = ((float)cur_hp / (float)max_hp) * (float)(rect.w - 16);
    FillRect(rect.x + TEXT_W, rect.y + (size * 2) + TEXT_W, hp_w, TEXT_W, g_gameFlash.GetColor[PAL_BRIGHT_LIGHT_GRN]);

    const uint16_t cur_mp = Int999GetCurrent(&g_run.creatures.mp[id]);
    const uint16_t max_mp = Int999GetMax(&g_run.creatures.mp[id]);

    DEBUG("CreatureStats MP: %d/%d", cur_mp, max_mp);
    GetStatLine(cur_mp, max_mp, max_chars, status_line, "MP:");
    PrintLineStr(rect.x + TEXT_W, rect.y + (size * 3) + TEXT_W, font_size, max_chars, status_line, false);

    const uint16_t mp_w = ((float)cur_mp / (float)max_mp) * (float)(rect.w - 16);
    FillRect(rect.x + TEXT_W, rect.y + (size * 4) + TEXT_W, mp_w, TEXT_W, g_gameFlash.GetColor[PAL_GRAY_BLUE]);
}

/**********************************************************************************************************************/
/*   Draws the main Battle screen
 *      -2 battler sprites
 *      -2 status boxes
 *      -battle menu list for actuve creature abilities
 *      -battle menu list for player actions ie swap, use item, use spell etx
**********************************************************************************************************************/
void HandleBattle()
{
    DEBUG("HandleBattle");
    FontSize font_size = g_run.settings.fontSize;
    uint8_t size = TILE_W;
    if (font_size == FONT8x8)
        size = TEXT_W;
    uint8_t indent = 1;
    uint8_t max_chars = (320 / 2 / size) - indent;

    Rect_16 player = PLAYER_BATTLER_FRAME;
    Rect_16 playerHP = PLAYER_RESOURCE_FRAME;
    Rect_16 enemy = ENEMY_BATTLER_FRAME;
    Rect_16 enemyHP = ENEMY_RESOURCE_FRAME;
    Rect_16 dialogue = DIALOGUE_BOX_FRAME;


    const SpriteLayout pLayout = g_gameFlash.spriteData.battlers.backLayout[GetCreatureType(g_run.battleMode.playerMonsterID)];
    const uint8_t* pSprite = g_gameFlash.spriteData.battlers.back;
    DrawBattler(player.x + 24, player.y, pLayout, pSprite);

    const SpriteLayout eLayout = g_gameFlash.spriteData.battlers.frontLayout[GetCreatureType(g_run.battleMode.enemyMonsterID)];
    const uint8_t* eSprite = g_gameFlash.spriteData.battlers.front;
    DrawBattler(enemy.x + 24, enemy.y, eLayout, eSprite);

    CreatureStats(g_run.battleMode.playerMonsterID, playerHP, size, font_size);
    CreatureStats(g_run.battleMode.enemyMonsterID, enemyHP, size, font_size);


    uint16_t x = dialogue.x;
    uint16_t y = dialogue.y;
    uint8_t i = 0;
    while (i < MAX_ABILITIES)
    {
        const char* line = g_gameFlash.text.names.attacks[g_run.creatures.attacks[g_run.battleMode.playerMonsterID][i]];
        DEBUG("line: %s", line);
        if (!line) break;
        y += PrintLineStr(x, y, font_size, max_chars, line, indent);
        i++;
    }

    x = dialogue.x + (20 * size);
    y = dialogue.y;
    i = 0;
    while (i < BATTLE_MENU_SIZE)
    {
        const char* line = GetBattleMenuList(i);
        y += PrintLineStr(x, y, font_size, max_chars, line, indent);
        i++;
    }


    //combat log
    FillRect(dialogue.x, dialogue.y + (6 * size), dialogue.w, dialogue.h, g_gameFlash.GetColor[PAL_BRIGHT_LIGHT_GRN]);
    PrintLineStr(dialogue.x, dialogue.y + (6 * size), font_size, 40, g_run.battleMode.combatLog[0], false);
    PrintLineStr(dialogue.x, dialogue.y + (7 * size), font_size, 40, g_run.battleMode.combatLog[1], false);

    g_run.menu.colorCache = g_gameFlash.GetColor[PAL_BRIGHT_CYAN];
}
