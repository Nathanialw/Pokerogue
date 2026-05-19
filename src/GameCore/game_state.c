//
// Created by nathanial on 2/27/26.
//

#include "game_state.h"

#include "lib_debugging.h"
#include "lib_decl.h"
#include "lib_memory.h"

#include "menu.h"
#include "player.h"
#include "memory_ram.h"
#include "menu_battle.h"
#include "menu_main.h"
#include "animation.h"
#include "battles.h"
#include "camera.h"
#include "core.h"
#include "entities.h"
#include "graphics.h"
#include "map.h"
#include "memory_rom.h"
#include "rendering.h"


#define TITLE_RATE_DELAY 500


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
void SetGameLoopRateDefault()
{
    g_core.btns.gameLoopRate = g_core.btns.defaultGameLoopRate;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
void SetGameLoopRate(uint16_t time)
{
    g_core.btns.gameLoopRate = time;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".splash")
void TitleRateDelay(HardwareInterface hardware)
{
    hardware.SleepMS(TITLE_RATE_DELAY);
}

SET_MEMORY(".map")
void GameLoopRateDelay(HardwareInterface hardware)
{
    hardware.SleepMS(g_core.btns.gameLoopRate);
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".core")
State SetInputState(InputState state)
{
    g_core.state.inputState = state;
    return g_core.state;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
State SetBattleState(BattleState state)
{
    g_core.state.battleState = state;
    return g_core.state;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckBattleState(BattleState state)
{
    if (g_core.state.battleState == state)
        return true;
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
State SetGameState(GameState state)
{
    g_core.state.gameState = state;
    return g_core.state;
}


/**********************************************************************************************************************/
/*  input handling based on game state
**********************************************************************************************************************/
void UpdateBattleRunningState(GraphicsInterface graphics, HardwareInterface hardware, InputInterface input, MemoryInterface memory)
{
    if (g_core.state.inputState == INPUT_BATTLE)
    {
        if (input.GetButtonA())
        {
            bool b = BattleMenuCommand(hardware, input, memory);
            if (!b) return; //No state change for menu input
            SetBattleState(BATTLE_ATTACK);
        }

        if (input.GetButtonB())
        {
            ExitMenu();
        }

        if (input.GetButtonX())
        {
        }

        if (input.GetButtonY())
        {
        }

        if (input.GetButtonJSClick())
        {
        }

        if (input.GetButtonDPClick())
        {
        }

        if (input.GetJSPressed())
        {
            if (!SetMenuDelta(hardware, input, memory, input.GetInputKeyState().d))
                UpdateBattleMenu(input);
        }

        if (input.GetDPPressed())
        {
            if (!SetMenuDelta(hardware, input, memory, input.GetInputKeyState().d))
                UpdateBattleMenu(input);
        }
        return;
    }
}

/**********************************************************************************************************************/
/*  input handling based on game state
**********************************************************************************************************************/
SET_MEMORY(".map")
void UpdateGameRunningState(GraphicsInterface graphics, HardwareInterface hardware, InputInterface input, MemoryInterface memory)
{
    if (g_core.state.inputState == INPUT_MENU)
    {
        if (input.GetButtonA())
        {
            if (!OpenSubMenu(hardware, input, memory))
            {
                SetInputState(INPUT_MOVING);
            }
            return;
        }

        if (input.GetButtonB())
        {
            if (!MenuBack(memory))
            {
                SetInputState(INPUT_MOVING);
                SetGameLoopRateDefault();
                FullRedraw(graphics, hardware, memory);
            }

            return;
        }

        if (input.GetButtonX())
        {
        }

        if (input.GetButtonY())
        {
        }

        if (input.GetButtonJSClick())
        {
        }

        if (input.GetButtonDPClick())
        {
        }

        if (input.GetJSPressed())
        {
            if (!SetMenuDelta(hardware, input, memory, input.GetInputKeyState().d))
                OpenSubMenu(hardware, input, memory);
            return;
        }


        if (input.GetDPPressed())
        {
            if (!SetMenuDelta(hardware, input, memory, input.GetInputKeyState().d))
                OpenSubMenu(hardware, input, memory);
            return;
        }
    }


    if (g_core.state.inputState == INPUT_MOVING)
    {
        if (input.GetButtonA())
        {
            PlayerInteractItemInCell();
        }

        if (input.GetButtonB())
        {
            PlayerInteractObjectInCell(memory, hardware);
        }

        if (input.GetButtonX())
        {
        }

        if (input.GetButtonY())
        {
            InitMainMenu();
            SetInputState(INPUT_MENU);
            SetGameLoopRate(MENU_INPUT_POLLING_RATE);
            return;
        }

        if (input.GetButtonJSClick())
        {
        }

        if (input.GetButtonDPClick())
        {
        }

        if (input.GetJSPressed())
        {
            SetPlayerDelta(input.GetInputKeyState().d);
            return;
        }

        if (input.GetDPPressed())
        {
            SetPlayerDelta(input.GetInputKeyState().d);
            return;
        }
    }
}

SET_MEMORY(".splash")
bool UpdateGameTitleState(InputInterface input)
{
    if (input.GetButtonA())
    {
        return true;
    }
    if (input.GetButtonB())
    {
    }

    if (input.GetButtonX())
    {
    }

    if (input.GetButtonY())
    {
    }

    if (input.GetButtonStart())
    {
    }

    if (input.GetButtonSelect())
    {
    }

    if (input.GetButtonJSClick())
    {
    }

    if (input.GetButtonDPClick())
    {
    }

    if (input.GetJSPressed())
    {
    }

    if (input.GetDPPressed())
    {
    }


    return false;
}

/**********************************************************************************************************************/
/**  Game State forking
**********************************************************************************************************************/
SET_MEMORY(".battle")
void HandleBattleState(GameInterface* spi)
{
    if (g_core.state.inputState == INPUT_BATTLE)
    {
        if (CheckBattleState(BATTLE_INIT))
        {
            AnimationScreenClearRandom(spi->graphics, spi->hardware); //ANIMATION - move both creatures into place
            HandleBattle(spi->graphics, spi->hardware, spi->memory);
            HandleBattleMenu(spi->graphics, spi->hardware, spi->memory);
            SetBattleState(BATTLE_MENUS);
        }
        else if (CheckBattleState(BATTLE_ATTACK))
        {
            BattlerAnimationAttack(spi->graphics, spi->memory, true); //attacking animation
            BattlerAnimationStruck(spi->graphics, spi->memory, false); //hit animation
            AnimationUpdateHealth(spi->graphics, spi->hardware, true);
            if (!CheckPlayerAttackOutcome())
            {
                AnimationBattlerDie(spi->graphics, spi->hardware, spi->memory, false);
                DestroyEnemyCreature(spi->hardware);
                AnimationScreenFade(spi->graphics, spi->hardware); //ANIMATION - enemy creature drops off screen
                FullRedraw(spi->graphics, spi->hardware, spi->memory);
                SetInputState(INPUT_MOVING);
                return;
            }

            UseSkill(spi->hardware, spi->memory, false);
            BattlerAnimationAttack(spi->graphics, spi->memory, false); //attacking animation
            BattlerAnimationStruck(spi->graphics, spi->memory, true); //hit animation
            AnimationUpdateHealth(spi->graphics, spi->hardware, false);
            if (CheckEnemyAttackOutcome())
            {
                //ANIMATION - player's creature drops off screen
                if (false) // if no more creatures left
                {
                    // updateState.inputState = TITLE_SCREEN;
                }
            }
            SetBattleState(BATTLE_MENUS);
        }
        else if (CheckBattleState(BATTLE_MENUS))
        {
            HandleBattleMenu(spi->graphics, spi->hardware, spi->memory);
            DrawCursor(spi->graphics, spi->memory);
        }
    }
}


SET_MEMORY(".map.rodata")
const char test01[] = "UpdateGame\n";

SET_MEMORY(".map.rodata")
const char test02[] = "RenderObjects\n";


SET_MEMORY(".map.rodata")
const char test021[] = "DONE";
/**********************************************************************************************************************/
/**  Game State forking
**********************************************************************************************************************/
SET_MEMORY(".map")
void HandleGameState(GameInterface* spi)
{
    if (g_core.state.inputState == INPUT_MOVING)
    {
        spi->hardware.Print(test01);
        UpdateGame(spi->hardware);
        spi->hardware.Print(test02);
        RenderObjects(spi->graphics, spi->hardware, spi->memory);
    }

    if (g_core.state.inputState == INPUT_MENU)
    {
        HandleMenu(spi->graphics, spi->hardware, spi->memory);
        spi->hardware.Print(test01);
        HandleGameMenu(spi->graphics, spi->hardware, spi->memory);
        spi->hardware.Print(test02);
        DrawCursor(spi->graphics, spi->memory);
        spi->hardware.Print(test021);
    }

    // spi.audio.PlaySoundEffect();
}


SET_MEMORY(".splash.rodata")
const char str_splash_exit[] = "Exiting splash_entry, Going to map_gen_entry";

SET_MEMORY(".splash.rodata")
const char str_splash_entry[] = "Going to splash_entry";


/**********************************************************************************************************************/
/**  main game state update loop
**********************************************************************************************************************/
SET_MEMORY(".splash_entry")
uint8_t GameLoopTitleScreen(GameInterface* spi)
{
    spi->hardware.Print(str_splash_entry);
    int start = 0;
    while (start == 0)
    {
        spi->input.HandleInput();
        start = UpdateGameTitleState(spi->input);
        TitleRateDelay(spi->hardware);
    }

    spi->hardware.Print(str_splash_exit);
    return GAME_MAP_GEN;
}


bool MainBattleLoop(GameInterface* spi)
{
    return true;
}


SET_MEMORY(".battle_entry")
uint8_t BattleLoopMain(GameInterface* spi)
{
    InitBattleMenu();

    bool battling = true;
    while (battling)
    {
        UpdateBattleRunningState(spi->graphics, spi->hardware, spi->input, spi->memory);;
        HandleBattleState(spi);
        battling = MainBattleLoop(spi);;
    }

    return GAME_MAP;
}


SET_MEMORY(".map_gen.rodata")
static const char str_map_gen_entry_entry[] = "Going to map_gen_entry";

SET_MEMORY(".map_gen_entry")
uint8_t GameLoopEntry(GameInterface* spi)
{
    spi->graphics.FillScreen(0x000F); // TODO: update to real title screen
    spi->hardware.Print(str_map_gen_entry_entry);

    InitGame(spi->hardware, spi->memory);
    return GAME_MAP;;
}


SET_MEMORY(".map_entry")
uint8_t GameLoopMain(GameInterface* spi)
{
    g_core.state.overlay = OVERLAY_MAP;
    spi->graphics.FillScreen(0x0f00); // TODO: update to real title screen
    ResetEntities(spi->hardware, spi->memory, false);
    InitPlayer(spi->hardware, spi->memory);
    PopulateLevelCreatures(spi->hardware, spi->memory);
    PopulateLevelObjects(spi->hardware, spi->memory);
    PopulateLevelItems(spi->hardware, spi->memory);
    PlacePlayerOnMap(spi->hardware);
    SetMapFog(0xFF);
    InitCamera(0, 0, TILE_W * VIEW_TW, TILE_H * VIEW_TH);
    SetCameraPlayer();

    FullRedraw(spi->graphics, spi->hardware, spi->memory);

    while (g_core.state.overlay == OVERLAY_MAP)
    {
        spi->input.HandleInput();
        UpdateGameRunningState(spi->graphics, spi->hardware, spi->input, spi->memory);
        HandleGameState(spi);
        GameLoopRateDelay(spi->hardware);
    }
    return g_core.state.gameState;
}

// __attribute__((section(".strings")))
uint8_t GameLoopNewMap(GameInterface* spi)
{
    // NewMap();
    // SetGameState(OVERLAY_GAME_RUNNING);
    return 0;
}


uint8_t GameLoopShutdown(GameInterface* spi)
{
    return 0;
}


typedef uint8_t (*GameLoopFunc)(GameInterface* spi);
GameLoopFunc GameLoopState[10] = {GameLoopTitleScreen, GameLoopEntry, GameLoopMain, GameLoopNewMap, GameLoopShutdown};


void TestMain(GameInterface* spi)
{
    uint16_t color = 0x000A;

    while (1)
    {
        spi->input.HandleInput();
        if (spi->input.GetButtonA())
            color = 0xA000;
        else if (spi->input.GetButtonB())
            color = 0x041F;
        else if (spi->input.GetButtonX())
            color = 0xF10A;

        spi->graphics.FillScreen(color);
        spi->hardware.SleepMS(100);
    }
}


/**********************************************************************************************************************/
/**  main game state update loop
**********************************************************************************************************************/
void GameLoop(GameInterface* spi)
{
    SetInputState(INPUT_MOVING);
    // SetGameState(TITLE_SCREEN);  OVERLAY_TITLE_SCREEN

    while (1)
    {
        bool b = GameLoopState[g_core.state.gameState](spi);
        if (!b) return;
    }
}
