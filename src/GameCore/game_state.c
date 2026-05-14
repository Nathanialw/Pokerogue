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
#include "core.h"
#include "graphics.h"
#include "memory_rom.h"
#include "rendering.h"

#define TITLE_RATE_DELAY 500

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void SetGameLoopRateDefault()
{
    g_run.btns.gameLoopRate = g_run.btns.defaultGameLoopRate;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void SetGameLoopRate(uint16_t time)
{
    g_run.btns.gameLoopRate = time;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
// SET_MEMORY(".title")
void TitleRateDelay(HardwareInterface hardware)
{
    hardware.SleepMS(TITLE_RATE_DELAY);
}


void GameLoopRateDelay(HardwareInterface hardware)
{
    hardware.SleepMS(g_run.btns.gameLoopRate);
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
State SetInputState(InputState state)
{
    g_run.state.inputState = state;
    return g_run.state;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
State SetBattleState(BattleState state)
{
    g_run.state.battleState = state;
    return g_run.state;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CheckBattleState(BattleState state)
{
    if (g_run.state.battleState == state)
        return true;
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
State SetGameState(GameState state)
{
    g_run.state.gameState = state;
    return g_run.state;
}


/**********************************************************************************************************************/
/*  input handling based on game state
**********************************************************************************************************************/
void UpdateBattleRunningState(GraphicsInterface graphics, HardwareInterface hardware, InputInterface input)
{
    if (g_run.state.inputState == BATTLE)
    {
        if (input.GetButtonA())
        {
            bool b = BattleMenuCommand(hardware, input);
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
            if (!SetMenuDelta(input, input.GetInputKeyState().d))
                UpdateBattleMenu(input);
        }

        if (input.GetDPPressed())
        {
            if (!SetMenuDelta(input, input.GetInputKeyState().d))
                UpdateBattleMenu(input);
        }
        return;
    }
}

/**********************************************************************************************************************/
/*  input handling based on game state
**********************************************************************************************************************/
void UpdateGameRunningState(GraphicsInterface graphics, HardwareInterface hardware, InputInterface input)
{
    if (g_run.state.inputState == MENU)
    {
        if (input.GetButtonA())
        {
            if (!OpenSubMenu(input))
            {
                SetInputState(MOVING);
            }
            return;
        }

        if (input.GetButtonB())
        {
            if (!MenuBack())
            {
                SetInputState(MOVING);
                SetGameLoopRateDefault();
                FullRedraw(graphics);
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
            if (!SetMenuDelta(input, input.GetInputKeyState().d))
                OpenSubMenu(input);
            return;
        }


        if (input.GetDPPressed())
        {
            if (!SetMenuDelta(input, input.GetInputKeyState().d))
                OpenSubMenu(input);
            return;
        }
    }


    if (g_run.state.inputState == MOVING)
    {
        if (input.GetButtonA())
        {
            PlayerInteractItemInCell();
        }

        if (input.GetButtonB())
        {
            PlayerInteractObjectInCell();
        }

        if (input.GetButtonX())
        {
        }

        if (input.GetButtonY())
        {
            InitMainMenu();
            SetInputState(MENU);
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

SET_MEMORY(".title")
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


    return 0;
}

/**********************************************************************************************************************/
/**  Game State forking
**********************************************************************************************************************/
void HandleGameState(GameInterface* spi)
{
    if (g_run.state.inputState == MOVING)
    {
        UpdateGame(spi->hardware);
        RenderObjects(spi->graphics, spi->hardware);
    }
    if (g_run.state.inputState == MENU)
    {
        HandleMenu(spi->graphics, spi->hardware);
        HandleGameMenu(spi->graphics, spi->hardware);
        DrawCursor(spi->graphics);
    }
    if (g_run.state.inputState == BATTLE)
    {
        if (CheckBattleState(BATTLE_INIT))
        {
            AnimationScreenClearRandom(spi->graphics, spi->hardware); //ANIMATION - move both creatures into place
            HandleBattle(spi->graphics, spi->hardware);
            HandleBattleMenu(spi->graphics, spi->hardware);
            SetBattleState(BATTLE_MENUS);
        }
        else if (CheckBattleState(BATTLE_ATTACK))
        {
            BattlerAnimationAttack(spi->graphics, true); //attacking animation
            BattlerAnimationStruck(spi->graphics, false); //hit animation
            AnimationUpdateHealth(spi->graphics, spi->hardware, true);
            if (!CheckPlayerAttackOutcome())
            {
                AnimationBattlerDie(spi->graphics, spi->hardware, false);
                DestroyEnemyCreature(spi->hardware);
                AnimationScreenFade(spi->graphics, spi->hardware); //ANIMATION - enemy creature drops off screen
                FullRedraw(spi->graphics);
                SetInputState(MOVING);
                return;
            }

            UseSkill(spi->hardware, false);
            BattlerAnimationAttack(spi->graphics, false); //attacking animation
            BattlerAnimationStruck(spi->graphics, true); //hit animation
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
            HandleBattleMenu(spi->graphics, spi->hardware);
        }
        DrawCursor(spi->graphics);
    }

    // spi.audio.PlaySoundEffect();
}

/**********************************************************************************************************************/
/**  main game state update loop
**********************************************************************************************************************/
// SET_MEMORY(".battle")
SET_MEMORY(".title_entry")
void GameLoopTitleScreen(GameInterface* spi)
{
    spi->graphics.FillScreen(0xFDA0); // TODO: update to real title screen
    int start = 0;
    while (start == 0)
    {
        // Pico_AnimationTitle();
        spi->input.HandleInput();
        start = UpdateGameTitleState(spi->input);
        TitleRateDelay(spi->hardware);
    }
    spi->graphics.FillScreen(0xA000);
}


bool GameLoopMain(GameInterface* spi)
{
    InitGame(spi->hardware);
    FullRedraw(spi->graphics);
    while (g_run.state.gameState == GAME_RUNNING)
    {
        spi->input.HandleInput();
        UpdateGameRunningState(spi->graphics, spi->hardware, spi->input);
        HandleGameState(spi);
        GameLoopRateDelay(spi->hardware);
    }

    DEBUG("GameLoopMain break, %d", g_run.state.gameState);
    return true;
}

// __attribute__((section(".strings")))
bool GameLoopNewMap(GameInterface* spi)
{
    NewMap();
    SetGameState(GAME_RUNNING);
    return true;
}

SET_MEMORY(".map")
bool GameLoopShutdown(GameInterface* spi)
{
    return false;
}


typedef bool (*GameLoopFunc)(GameInterface* spi);
GameLoopFunc GameLoopState[GAME_STATE_SIZE] = {GameLoopTitleScreen, GameLoopMain, GameLoopNewMap, GameLoopShutdown};


SET_MEMORY(".battle")
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
    SetGameState(TITLE_SCREEN);
    SetInputState(MOVING);

    while (1)
    {
        bool b = GameLoopState[g_run.state.gameState](spi);
        if (!b) return;
    }
}
