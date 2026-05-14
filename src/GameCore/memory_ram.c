//
// Created by nathanial on 2/26/26.
//

#include "memory_ram.h"

#include "lib_memory.h"

#include "constants.h"

GameRunState g_run = {0};

TitleRunState g_title = {0};

void GameRunInit()
{
    g_run.floor = 0;
    g_run.settings.fontSize = FONT8x8;

    g_run.btns.gameSpeed = 10; //200
    g_run.btns.defaultGameLoopRate = 12;
    g_run.btns.gameLoopRate = 12;

    g_run.menu.selectedMenu = MAIN_MENU;
    g_run.menu.displayedMenu = MENU_NONE;
    g_run.menu.visibleMenuOptions = MAIN_MENUS_SIZE;
    g_run.menu.forceRedraw = true;
    g_run.menu.lineHeight = 0;

    g_run.menu.gameMenu.open = false;
    g_run.menu.gameMenu.displayId = NO_CREATURE;
    g_run.menu.gameMenu.id = NO_CREATURE;
    g_run.menu.menuSleepTime = 200; //200

    g_run.music.master_volume = 32768;
}


void TitleRunInit()
{
}
