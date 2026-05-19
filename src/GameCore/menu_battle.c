//
// Created by nathanial on 3/8/26.
//

#include "menu_battle.h"

#include "memory_ram.h"

#include "battles.h"
#include "game_state.h"
#include "memory_rom.h"
#include "entities.h"
#include "lib_debugging.h"
#include "lib_decl.h"
#include "menu.h"
#include "player.h"

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
typedef enum
{
    ABILITY_MENU,
    BATTLE_MENU,
} BattleMenu;

BattleMenu battleMenu = ABILITY_MENU;

/**********************************************************************************************************************/
/** Open list frame
 *  Set menu cursor cache data to defaults for lists
**********************************************************************************************************************/
bool EnterMenu(const uint8_t listSize)
{
    if (g_core.menu.depth == 1)
    {
        return true;
    }
    g_core.menu.depth++;
    g_core.menu.visibleMenuOptions = listSize;
    g_core.menu.menuScrollOffset[g_core.menu.depth].y = 0;
    g_core.menu.sel[g_core.menu.depth].y = 0;
    g_core.menu.x = 0;
    g_core.menu.y = 9;

    return false;
}

/**********************************************************************************************************************/
/** Set menu cursor cache data to defaults
 *  Close main menu
**********************************************************************************************************************/
void ExitMenu(void)
{
    if (g_core.menu.depth == 1)
    {
        ClearMenu();
        g_core.menu.sel[g_core.menu.depth].y = 0;
        g_core.menu.menuScrollOffset[g_core.menu.depth].y = 0;
        g_core.menu.visibleMenuOptions = BATTLE_MENU_SIZE;
        g_core.menu.x = 10;
        g_core.menu.y = BATTLE_MENU_Y;
        g_core.menu.depth--;
        g_core.menu.lineHeight = 0;
    }
}

/**********************************************************************************************************************/
/** Initial invocation updates the string list cache for display, opening the party frame
 *  Subsequent invocation sets the creature at the cursor position as the active battle creature
**********************************************************************************************************************/
bool BattleSwap(HardwareInterface hardware, InputInterface input, MemoryInterface memory, bool update)
{
    if (EnterMenu(MAX_PARTY_SIZE))
    {
        uint8_t sel = g_core.menu.sel[g_core.menu.depth].y;
        if (GetCreatureType(g_core.player.partyID[sel]) == NO_CREATURE) return true;
        g_core.battleMode.playerMonsterID = g_core.player.partyID[sel];
        return true;
    }


    FillListByEntityID(memory, MAX_PARTY_SIZE, CREATURE, GetPlayerMonsterIDs());
    return true;
}

/**********************************************************************************************************************/
/** Initial invocation updates the string list cache for display, opening the spellbook list frame
 *  Subsequent invocation runs the spell action for the spell id at the cursor index of the spellbook
**********************************************************************************************************************/
bool BattleSpell(HardwareInterface hardware, InputInterface input, MemoryInterface memory, bool update)
{
    if (EnterMenu(g_core.player.currentSpellbookSize))
    {
        uint8_t sel = g_core.menu.sel[g_core.menu.depth].y;
        SpellId spell_id = g_core.player.spellID[sel];
        CastSpell(hardware, memory, spell_id, g_core.battleMode.playerMonsterID, g_core.battleMode.enemyMonsterID);
        return true;
    }


    FillListByTypeID(memory, g_core.player.currentSpellbookSize, g_core.player.spellID);
    return true;
}


/**********************************************************************************************************************/
/** Initial invocation updates the string list cache for display, opening the backpack list frame
 *  Subsequent invocation runs the item action for the item id at the cursor index of the backpack
**********************************************************************************************************************/
bool BattleItems(HardwareInterface hardware, InputInterface input, MemoryInterface memory, bool update)
{
    if (EnterMenu(g_core.player.currentBagSize))
    {
        uint8_t idx = g_core.menu.sel[g_core.menu.depth].y + g_core.menu.menuScrollOffset[g_core.menu.depth].y;
        EntityId item_id = g_core.player.itemID[idx];
        EntityId entity_id = g_core.battleMode.playerMonsterID;
        if (UseItem(memory, item_id, entity_id))
        {
            ConsumeItem(idx, item_id);
            FillListByEntityID(memory, g_core.player.currentBagSize, ITEM, g_core.player.itemID);
        }
        return true;
    }

    FillListByEntityID(memory, g_core.player.currentBagSize, ITEM, g_core.player.itemID);
    return true;
}

/**********************************************************************************************************************/
/** attempts the end the battle and queue a move to an empty adjacent map cell
 *  TODO: NOT YET IMPLEMENTED
**********************************************************************************************************************/
bool BattleFlee(HardwareInterface hardware, InputInterface input, MemoryInterface memory, bool update)
{
    if (EnterMenu(4))
    {
        return true;
    }

    return true;
}

/**********************************************************************************************************************/
/** display a full screen of teh combat lines cache
 *  TODO: NOT YET IMPLEMENTED
**********************************************************************************************************************/
bool BattleCombatLog(HardwareInterface hardware, InputInterface input, MemoryInterface memory, bool update)
{

    // fire on initial entry
    if (EnterMenu(4))
    {
        return true;
    }

    return true;
}

/**********************************************************************************************************************/
/** Function pointer array for battle menu options
**********************************************************************************************************************/
SubMenu battleSubmenus[BATTLE_MENU_SIZE] =
{
    BattleSwap,
    BattleSpell,
    BattleItems,
    BattleCombatLog,
    BattleFlee,
};

/**********************************************************************************************************************/
/** Returns the number of usable abilities of the player's active battle creature
**********************************************************************************************************************/
uint8_t AbilityCount(void)
{
    uint8_t num_abilities = 0;
    while (num_abilities < MAX_ABILITIES)
    {
        if (g_core.creatures.attacks[g_core.battleMode.playerMonsterID][num_abilities] == NO_ABILITY) break;
        num_abilities++;
    }

    return num_abilities;
}

/**********************************************************************************************************************/
/** Sets battle menu cache data to defaults
 *  Sets input state to BATTLE
 *  Sets battle state to BATTLE_INIT to trigger drawing the battle screen procedure
**********************************************************************************************************************/
void InitBattleMenu(void)
{
    SetInputState(INPUT_BATTLE);
    SetBattleState(BATTLE_INIT);
    g_core.menu.x = BATTLE_MENU_X;
    g_core.menu.y = BATTLE_MENU_Y;
    g_core.menu.h = MAX_ABILITIES;

    g_core.menu.subMenus = battleSubmenus;
    g_core.menu.visibleMenuOptions = AbilityCount();

    g_core.menu.depth = 0;
    g_core.menu.sel[g_core.menu.depth].x = 0;
    g_core.menu.sel[g_core.menu.depth].y = 0;
    g_core.menu.selectedMenu = BATTLE_MENU;
}

/**********************************************************************************************************************/
/** toggles menu cache cursor data between the use ability list and the battle menu list
**********************************************************************************************************************/
void UpdateBattleMenu(InputInterface input)
{
    if (input.GetInputKeyState().d.x == 0) return;

    if (g_core.menu.x == BATTLE_MENU_X)
    {
        battleMenu = BATTLE_MENU;
        g_core.menu.x = 10;
        g_core.menu.sel[g_core.menu.depth].y = 0;
        g_core.menu.visibleMenuOptions = BATTLE_MENU_SIZE;
    }
    else
    {
        battleMenu = ABILITY_MENU;
        g_core.menu.x = BATTLE_MENU_X;
        g_core.menu.sel[g_core.menu.depth].y = 0;
        g_core.menu.visibleMenuOptions = AbilityCount();
    }
}

/**********************************************************************************************************************/
/** Main entry point for battle menu actions
 *  Checks which menu the cursor is active in
 *  Then runs the functino pointer at the cursor index
 *  Returns true if Attacking
 *  Returns false if menu
**********************************************************************************************************************/
bool BattleMenuCommand(HardwareInterface hardware, InputInterface input, MemoryInterface memory)
{
    bool b = false;
    DEBUG("BattleMenuCommand===================================");

    if (battleMenu == ABILITY_MENU)
        b = UseSkill(hardware, memory, true);
    if (battleMenu == BATTLE_MENU)
        battleSubmenus[g_core.menu.sel[0].y](hardware, input, memory, true); //0 to enter the menu base entry point

    return b;
}
