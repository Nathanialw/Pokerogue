//
// Created by nathanial on 3/8/26.
//

#include "menu_battle.h"

#include "memory_ram.h"
#include <stdint.h>

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
    if (g_run.menu.depth == 1)
    {
        return true;
    }
    g_run.menu.depth++;
    g_run.menu.visibleMenuOptions = listSize;
    g_run.menu.menuScrollOffset[g_run.menu.depth].y = 0;
    g_run.menu.sel[g_run.menu.depth].y = 0;
    g_run.menu.x = 0;
    g_run.menu.y = 9;

    return false;
}

/**********************************************************************************************************************/
/** Set menu cursor cache data to defaults
 *  Close main menu
**********************************************************************************************************************/
void ExitMenu(void)
{
    if (g_run.menu.depth == 1)
    {
        ClearMenu();
        g_run.menu.sel[g_run.menu.depth].y = 0;
        g_run.menu.menuScrollOffset[g_run.menu.depth].y = 0;
        g_run.menu.visibleMenuOptions = BATTLE_MENU_SIZE;
        g_run.menu.x = 10;
        g_run.menu.y = BATTLE_MENU_Y;
        g_run.menu.depth--;
        g_run.menu.lineHeight = 0;
    }
}

/**********************************************************************************************************************/
/** Initial invocation updates the string list cache for display, opening the party frame
 *  Subsequent invocation sets the creature at the cursor position as the active battle creature
**********************************************************************************************************************/
bool BattleSwap(bool update)
{
    DEBUG("BattleSwap");
    if (EnterMenu(MAX_PARTY_SIZE))
    {
        uint8_t sel = g_run.menu.sel[g_run.menu.depth].y;
        DEBUG("Select new monster %d %d", g_run.battleMode.playerMonsterID, sel);
        if (GetCreatureType(g_run.player.partyID[sel]) == NO_CREATURE) return true;
        g_run.battleMode.playerMonsterID = g_run.player.partyID[sel];
        return true;
    }

    DEBUG("Drawing BattleSwap List");

    FillListByEntityID(MAX_PARTY_SIZE, CREATURE, GetPlayerMonsterIDs());
    return true;
}

/**********************************************************************************************************************/
/** Initial invocation updates the string list cache for display, opening the spellbook list frame
 *  Subsequent invocation runs the spell action for the spell id at the cursor index of the spellbook
**********************************************************************************************************************/
bool BattleSpell(bool update)
{
    DEBUG("BattleSpell");
    if (EnterMenu(g_run.player.currentSpellbookSize))
    {
        uint8_t sel = g_run.menu.sel[g_run.menu.depth].y;
        SpellId spell_id = g_run.player.spellID[sel];
        DEBUG("Casting Spell");
        CastSpell(spell_id, g_run.battleMode.playerMonsterID, g_run.battleMode.enemyMonsterID);
        return true;
    }

    DEBUG("Drawing BattleSpell List");

    FillListByTypeID(g_run.player.currentSpellbookSize, g_gameFlash.text.names.spells, g_run.player.spellID);
    return true;
}


/**********************************************************************************************************************/
/** Initial invocation updates the string list cache for display, opening the backpack list frame
 *  Subsequent invocation runs the item action for the item id at the cursor index of the backpack
**********************************************************************************************************************/
bool BattleItems(bool update)
{
    DEBUG("BattleItems");
    if (EnterMenu(g_run.player.currentBagSize))
    {
        uint8_t idx = g_run.menu.sel[g_run.menu.depth].y + g_run.menu.menuScrollOffset[g_run.menu.depth].y;
        EntityId item_id = g_run.player.itemID[idx];
        EntityId entity_id = g_run.battleMode.playerMonsterID;
        DEBUG("using battle item %d %d %d", idx, item_id, entity_id);
        if (UseItem(item_id, entity_id))
        {
            ConsumeItem(idx, item_id);
            FillListByEntityID(g_run.player.currentBagSize, ITEM, g_run.player.itemID);
        }
        return true;
    }

    FillListByEntityID(g_run.player.currentBagSize, ITEM, g_run.player.itemID);
    return true;
}

/**********************************************************************************************************************/
/** attempts the end the battle and queue a move to an empty adjacent map cell
 *  TODO: NOT YET IMPLEMENTED
**********************************************************************************************************************/
bool BattleFlee(bool update)
{
    DEBUG("BattleFlee");

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
bool BattleCombatLog(bool update)
{
    DEBUG("BattleCombatLog");

    // fire on initial entry
    if (EnterMenu(4))
    {
        return true;
    }

    return true;
}

/**********************************************************************************************************************/
/** Returns a pointer to the char array for the given index of the battle menu
 *  Used for printing the menu options to the screen
**********************************************************************************************************************/
const char* GetBattleMenuList(uint8_t idx)
{
    return g_gameFlash.text.menus.battleMenu[idx];
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
        if (g_run.creatures.attacks[g_run.battleMode.playerMonsterID][num_abilities] == NO_ABILITY) break;
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
    DEBUG("INIT BATTLE MENU");
    SetInputState(BATTLE);
    SetBattleState(BATTLE_INIT);
    g_run.menu.x = BATTLE_MENU_X;
    g_run.menu.y = BATTLE_MENU_Y;
    g_run.menu.h = MAX_ABILITIES;

    g_run.menu.subMenus = battleSubmenus;
    g_run.menu.visibleMenuOptions = AbilityCount();

    g_run.menu.depth = 0;
    g_run.menu.sel[g_run.menu.depth].x = 0;
    g_run.menu.sel[g_run.menu.depth].y = 0;
    g_run.menu.selectedMenu = BATTLE_MENU;
}

/**********************************************************************************************************************/
/** toggles menu cache cursor data between the use ability list and the battle menu list
**********************************************************************************************************************/
void UpdateBattleMenu(void)
{
    if (GetInputKeyState().d.x == 0) return;

    if (g_run.menu.x == BATTLE_MENU_X)
    {
        battleMenu = BATTLE_MENU;
        g_run.menu.x = 10;
        g_run.menu.sel[g_run.menu.depth].y = 0;
        g_run.menu.visibleMenuOptions = BATTLE_MENU_SIZE;
    }
    else
    {
        battleMenu = ABILITY_MENU;
        g_run.menu.x = BATTLE_MENU_X;
        g_run.menu.sel[g_run.menu.depth].y = 0;
        g_run.menu.visibleMenuOptions = AbilityCount();
    }
}

/**********************************************************************************************************************/
/** Main entry point for battle menu actions
 *  Checks which menu the cursor is active in
 *  Then runs the functino pointer at the cursor index
 *  Returns true if Attacking
 *  Returns false if menu
**********************************************************************************************************************/
bool BattleMenuCommand(void)
{
    bool b = false;
    DEBUG("BattleMenuCommand===================================");

    if (battleMenu == ABILITY_MENU)
        b = UseSkill(true);
    if (battleMenu == BATTLE_MENU)
        battleSubmenus[g_run.menu.sel[0].y](true); //0 to enter the menu base entry point

    return b;
}
