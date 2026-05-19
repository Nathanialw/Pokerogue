//
// Created by nathanial on 2/24/26.
//
#include "menu.h"

#include "lib_decl.h"
#include "lib_debugging.h"

#include "entities.h"
#include "memory_access.h"
#include "memory_ram.h"
#include "memory_rom.h"

#define LIST_JUMP_AMOUNT (-10)

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint16_t ListSize(uint16_t n)
{
    return n > (MAIN_MENU_H - 2) ? (MAIN_MENU_H - 1) * g_core.settings.fontSize : n;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".core")
bool ListJump(HardwareInterface hardware, InputInterface input, MemoryInterface memory)
{
    if (input.GetInputKeyState().d.x != 0)
    {
        Delta d = {};
        d.y = input.GetInputKeyState().d.x * LIST_JUMP_AMOUNT;
        if (HandleMenuOverflow(hardware, input, memory, d)) return true;
        SetMenuDelta(hardware, input, memory, d);
    }
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".core")
void ClearMenu(void)
{
    g_core.menu.text[0][0] = '\0';
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".core")
void FillListByEntityID(MemoryInterface memory, uint8_t n, uint8_t type, const uint8_t* e_ids)
{
    uint8_t typeIDs[n];
    uint8_t offset = g_core.menu.menuScrollOffset[g_core.menu.depth].y;
    GetEntityTypes(memory, typeIDs, e_ids, type, n, offset);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
void FillListByTypeID(MemoryInterface memory, uint8_t n, uint8_t* ids)
{
    uint8_t i = g_core.menu.menuScrollOffset[g_core.menu.depth].y;
    while (i < n)
    {
        Flash_GetSpellbookText(memory, g_core.menu.text[i], ids[i]);
        i++;
    }
    g_core.menu.text[i][0] = '\0';
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
bool Back(SubMainMenuWindow menuWin)
{
    if (g_core.menu.selectedMenu == menuWin)
        return true;

    g_core.menu.lineHeight = 0;
    if (g_core.menu.useOnPartyMember)
    {
        g_core.menu.useOnPartyMember = BACK_NONE;
        g_core.menu.forceRedraw = true;
        return true;
    }

    g_core.menu.selectedMenu = menuWin;
    g_core.menu.visibleMenuOptions = MAIN_MENUS_SIZE;
    g_core.menu.menuScrollOffset[g_core.menu.depth].y = 0;
    g_core.menu.depth--;
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
bool ToggleMenu(SubMainMenuWindow menuWin, uint8_t numMenuOptions)
{
    if (g_core.menu.selectedMenu == menuWin)
    {
        return true;
    }
    g_core.menu.depth++;
    g_core.menu.sel[g_core.menu.depth].x = 0;
    g_core.menu.sel[g_core.menu.depth].y = 0;
    g_core.menu.selectedMenu = menuWin;
    g_core.menu.visibleMenuOptions = ListSize(numMenuOptions);
    g_core.menu.totalMenuOptions = numMenuOptions;
    ClearMenu();
    return false;
}

/**********************************************************************************************************************/
/*  Manages the cursor position relative to the vertical screen list
 *  Ensures the cursor wraps around at the top and bottom of the menu lists
 *  maintains the cursor's position in the center of the list when the list length exceeds the screen height
**********************************************************************************************************************/
SET_MEMORY(".core")
bool HandleMenuOverflow(HardwareInterface hardware, InputInterface input, MemoryInterface memory, Delta delta)
{
    bool options_exceed_menu = g_core.menu.h < g_core.menu.visibleMenuOptions;
    uint8_t sel_pos_y = g_core.menu.sel[g_core.menu.depth].y;
    uint8_t sel_off_y = g_core.menu.menuScrollOffset[g_core.menu.depth].y;

    if (options_exceed_menu)
    {
        bool cursor_at_mid = sel_pos_y == (g_core.menu.visibleMenuOptions / 2);
        bool options_within_bot = (sel_pos_y + sel_off_y) > g_core.menu.totalMenuOptions - (g_core.menu.visibleMenuOptions / 2);
        bool options_within_top = (sel_pos_y + sel_off_y + delta.y) < (g_core.menu.visibleMenuOptions / 2);

        if (cursor_at_mid)
        {
            if (!options_within_top || !options_within_bot)
            {
                bool minOffset = sel_off_y == 0 && delta.y < 0;
                bool maxOffset = sel_off_y >= g_core.menu.totalMenuOptions - g_core.menu.visibleMenuOptions && delta.y > 0;

                if (!minOffset && !maxOffset)
                {
                    g_core.menu.menuScrollOffset[g_core.menu.depth].y += delta.y;
                    g_core.menu.subMenus[g_core.menu.sel[0].y](hardware, input, memory, true); // 0 call the function of the menu it is within
                    return true;
                }
            }
        }

        if (options_within_top)
        {
            if (sel_pos_y + delta.y < 0)
            {
                uint8_t topBotPage = g_core.menu.totalMenuOptions - g_core.menu.visibleMenuOptions;
                g_core.menu.menuScrollOffset[g_core.menu.depth].y = topBotPage;
                g_core.menu.subMenus[g_core.menu.sel[0].y](hardware, input, memory, true); // 0 call the function of the menu it is within
            }
        }
        else if (options_within_bot)
        {
            if (sel_pos_y + delta.y >= g_core.menu.visibleMenuOptions)
            {
                g_core.menu.menuScrollOffset[g_core.menu.depth].y = 0;
                g_core.menu.subMenus[g_core.menu.sel[0].y](hardware, input, memory, true); // 0 call the function of the menu it is within
                g_core.menu.forceRedraw = true;
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
SET_MEMORY(".core")
bool SetMenuDelta(HardwareInterface hardware, InputInterface input, MemoryInterface memory, Delta delta)
{
    if (delta.y == 0) return false;
    if (HandleMenuOverflow(hardware, input, memory, delta)) return false;

    g_core.menu.eraseSel.y = g_core.menu.sel[g_core.menu.depth].y;
    g_core.menu.sel[g_core.menu.depth].y += delta.y;


    if (g_core.menu.sel[g_core.menu.depth].y >= g_core.menu.visibleMenuOptions)
        g_core.menu.sel[g_core.menu.depth].y = 0;


    if (g_core.menu.sel[g_core.menu.depth].y < 0)
        g_core.menu.sel[g_core.menu.depth].y = g_core.menu.visibleMenuOptions - 1;

    g_core.menu.sel[g_core.menu.depth].x = delta.x;

    return true;
}

/**********************************************************************************************************************/
/*  Returns the cursor X position
**********************************************************************************************************************/
uint8_t GetSelectorX(void)
{
    return g_core.menu.sel[g_core.menu.depth].x;
}

/**********************************************************************************************************************/
/*  Returns the cursor Y position
**********************************************************************************************************************/
SET_MEMORY(".core")
uint8_t GetSelectorY(void)
{
    return g_core.menu.sel[g_core.menu.depth].y;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/


SET_MEMORY(".core")
void GetMenuLine(MemoryInterface memory, char* text, uint8_t idx)
{
    if (idx >= g_core.menu.visibleMenuOptions)
    {
        text[0] = '\0';
        return;
    }

    if (g_core.menu.selectedMenu == MAIN_MENU)
    {
        Flash_GetMenuText(memory, (uint8_t*)text, idx);
    }

    for (uint8_t i = 0; i < SMALL_STRINGS; ++i)
    {
        text[i] = g_core.menu.text[idx][i];
    }
}
