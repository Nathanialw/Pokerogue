//
// Created by nathanial on 2/26/26.
//

#include "memory_ram.h"
#include "constants.h"

GameRunState g_run = {0};

void GameRunInit()
{
    g_run.floor = 0;
    g_run.settings.fontSize = FONT8x8;

    g_run.btns.gameSpeed = 10; //200

    g_run.menu.selectedMenu = MAIN_MENU;
    g_run.menu.displayedMenu = MENU_NONE;
    g_run.menu.visibleMenuOptions = MAIN_MENUS_SIZE;
    g_run.menu.forceRedraw = true;
    g_run.menu.lineHeight = 0;

    g_run.menu.gameMenu.open = false;
    g_run.menu.gameMenu.displayId = NO_CREATURE;
    g_run.menu.gameMenu.id = NO_CREATURE;

    g_run.music.master_volume = 32768;
}
