//
// Created by nathanial on 2/26/26.
//

#include "memory_ram.h"

#include "lib_memory.h"

#include "constants.h"

SET_MEMORY(".core.data")
CoreRunState g_core = {0};
SET_MEMORY(".battle.data")
BattleRunState g_battle = {0};
SET_MEMORY(".map.data")
MapRunState g_map = {0};
SET_MEMORY(".splash.data")
TitleRunState g_title = {0};

SET_MEMORY(".map_gen")
void GameRunInit()
{
    g_core.floor = 0;
    g_core.settings.fontSize = FONT8x8;

    g_core.btns.gameSpeed = 10; //200
    g_core.btns.defaultGameLoopRate = 12;
    g_core.btns.gameLoopRate = 12;

    g_core.menu.selectedMenu = MAIN_MENU;
    g_core.menu.displayedMenu = MENU_NONE;
    g_core.menu.visibleMenuOptions = MAIN_MENUS_SIZE;
    g_core.menu.forceRedraw = true;
    g_core.menu.lineHeight = 0;

    g_core.menu.gameMenu.open = false;
    g_core.menu.gameMenu.displayId = NO_CREATURE;
    g_core.menu.gameMenu.id = NO_CREATURE;
    g_core.menu.menuSleepTime = 200; //200

    g_core.music.master_volume = 32768;
}


void TitleRunInit()
{

}
