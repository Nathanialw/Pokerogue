//
// Created by nathanial on 2/24/26.
//
#include "entities.h"
#include "lib_decl.h"

#ifdef ENABLE_DEBUG_OUTPUT
#include <stdio.h>
#endif


#include "menu.h"

#include "lib_debugging.h"
#include "memory_ram.h"
#include "memory_rom.h"

#define LIST_JUMP_AMOUNT (-10)

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
uint16_t ListSize(uint16_t n)
{
    return n > (MAIN_MENU_H - 2) ? (MAIN_MENU_H - 1) * g_run.settings.fontSize : n;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool ListJump()
{
    if (GetInputKeyState().d.x != 0)
    {
        Delta d = {};
        d.y = GetInputKeyState().d.x * LIST_JUMP_AMOUNT;
        if (HandleMenuOverflow(d)) return true;
        SetMenuDelta(d);
    }
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void ClearMenu(void)
{
    DEBUG("Clearing menu");
    g_run.menu.text[0] = NULL;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void FillListByEntityID(uint8_t n, uint8_t type, const uint8_t* e_ids)
{
    uint8_t typeIDs[n];
    const SmallStringArray* text = GetEntityTypes(typeIDs, e_ids, type, n);

    uint8_t i = g_run.menu.menuScrollOffset[g_run.menu.depth].y;
    while (i < n)
    {
        g_run.menu.text[i] = text[typeIDs[i]];
        DEBUG("text: %s enum: %d", g_run.menu.text[i], typeIDs[i]);
        i++;
    }
    g_run.menu.text[i] = NULL;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void FillListByTypeID(uint8_t n, const SmallStringArray* text, uint8_t* ids)
{
    uint8_t i = g_run.menu.menuScrollOffset[g_run.menu.depth].y;
    while (i < n)
    {
        const char* name = text[ids[i]];
        DEBUG("spell: %s enum: %d", name, ids[i]);
        g_run.menu.text[i] = name;
        i++;
    }
    g_run.menu.text[i] = NULL;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool Back(SubMainMenuWindow menuWin)
{
    if (g_run.menu.selectedMenu == menuWin)
        return true;

    g_run.menu.lineHeight = 0;
    if (g_run.menu.useOnPartyMember)
    {
        g_run.menu.useOnPartyMember = BACK_NONE;
        g_run.menu.forceRedraw = true;
        return true;
    }

    DEBUG("opening new menu %d", menuWin);
    g_run.menu.selectedMenu = menuWin;
    g_run.menu.visibleMenuOptions = MAIN_MENUS_SIZE;
    g_run.menu.menuScrollOffset[g_run.menu.depth].y = 0;
    g_run.menu.depth--;
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool ToggleMenu(SubMainMenuWindow menuWin, uint8_t numMenuOptions)
{
    if (g_run.menu.selectedMenu == menuWin)
    {
        DEBUG("menu already open %d", menuWin);
        return true;
    }
    DEBUG("opening new menu %d", menuWin);
    g_run.menu.depth++;
    g_run.menu.sel[g_run.menu.depth].x = 0;
    g_run.menu.sel[g_run.menu.depth].y = 0;
    g_run.menu.selectedMenu = menuWin;
    g_run.menu.visibleMenuOptions = ListSize(numMenuOptions);
    g_run.menu.totalMenuOptions = numMenuOptions;
    ClearMenu();
    return false;
}

/**********************************************************************************************************************/
/*  Manages the cursor position relative to the vertical screen list
 *  Ensures the cursor wraps around at the top and bottom of the menu lists
 *  maintains the cursor's position in the center of the list when the list length exceeds the screen height
**********************************************************************************************************************/
bool HandleMenuOverflow(Delta delta)
{
    bool options_exceed_menu = g_run.menu.h < g_run.menu.visibleMenuOptions;
    uint8_t sel_pos_y = g_run.menu.sel[g_run.menu.depth].y;
    uint8_t sel_off_y = g_run.menu.menuScrollOffset[g_run.menu.depth].y;

    if (options_exceed_menu)
    {
        bool cursor_at_mid = sel_pos_y == (g_run.menu.visibleMenuOptions / 2);
        bool options_within_bot = (sel_pos_y + sel_off_y) > g_run.menu.totalMenuOptions - (g_run.menu.visibleMenuOptions / 2);
        bool options_within_top = (sel_pos_y + sel_off_y + delta.y) < (g_run.menu.visibleMenuOptions / 2);

        if (cursor_at_mid)
        {
            if (!options_within_top || !options_within_bot)
            {
                bool minOffset = sel_off_y == 0 && delta.y < 0;
                bool maxOffset = sel_off_y >= g_run.menu.totalMenuOptions - g_run.menu.visibleMenuOptions && delta.y > 0;

                if (!minOffset && !maxOffset)
                {
                    g_run.menu.menuScrollOffset[g_run.menu.depth].y += delta.y;
                    g_run.menu.subMenus[g_run.menu.sel[0].y](true); // 0 call the function of the menu it is within
                    return true;
                }
            }
        }

        if (options_within_top)
        {
            if (sel_pos_y + delta.y < 0)
            {
                uint8_t topBotPage = g_run.menu.totalMenuOptions - g_run.menu.visibleMenuOptions;
                g_run.menu.menuScrollOffset[g_run.menu.depth].y = topBotPage;
                g_run.menu.subMenus[g_run.menu.sel[0].y](true); // 0 call the function of the menu it is within
            }
        }
        else if (options_within_bot)
        {
            if (sel_pos_y + delta.y >= g_run.menu.visibleMenuOptions)
            {
                g_run.menu.menuScrollOffset[g_run.menu.depth].y = 0;
                g_run.menu.subMenus[g_run.menu.sel[0].y](true); // 0 call the function of the menu it is within
                g_run.menu.forceRedraw = true;
            }
        }
    }
    return false;
}

/**********************************************************************************************************************/
/*  Validates the cursor position before update the cursor cache
 *  ON SUCCESS - returns true
 *  ON fail - returns false
**********************************************************************************************************************/
bool SetMenuDelta(Delta delta)
{
    if (delta.y == 0) return false;
    if (HandleMenuOverflow(delta)) return false;

    g_run.menu.eraseSel.y = g_run.menu.sel[g_run.menu.depth].y;
    g_run.menu.sel[g_run.menu.depth].y += delta.y;


    if (g_run.menu.sel[g_run.menu.depth].y >= g_run.menu.visibleMenuOptions)
        g_run.menu.sel[g_run.menu.depth].y = 0;


    if (g_run.menu.sel[g_run.menu.depth].y < 0)
        g_run.menu.sel[g_run.menu.depth].y = g_run.menu.visibleMenuOptions - 1;

    g_run.menu.sel[g_run.menu.depth].x = delta.x;

    return true;
}

/**********************************************************************************************************************/
/*  Returns the cursor X position
**********************************************************************************************************************/
uint8_t GetSelectorX(void)
{
    return g_run.menu.sel[g_run.menu.depth].x;
}

/**********************************************************************************************************************/
/*  Returns the cursor Y position
**********************************************************************************************************************/
uint8_t GetSelectorY(void)
{
    return g_run.menu.sel[g_run.menu.depth].y;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
const char* GetMenuLine(uint8_t idx)
{
    if (idx >= g_run.menu.visibleMenuOptions)
        return NULL;

    if (g_run.menu.selectedMenu == MAIN_MENU)
    {
        return g_gameFlash.text.menus.main[idx];
    }

    return g_run.menu.text[idx];
}
