//
// Created by nathanial on 2/27/26.
//

#include "game_state.h"

#include <sys/unistd.h>

#include "menu.h"
#include "player.h"
#include "memory_ram.h"
#include "menu_battle.h"
#include "menu_main.h"
#include "animation.h"
#include "battles.h"
#include "core.h"
#include "graphics.h"
#include "lib_debugging.h"
#include "memory_rom.h"
#include "rendering.h"
#include "sound.h"
#include "lib_decl.h"


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
inline void GameLoopRateDelay()
{
    SleepMS(g_run.btns.gameLoopRate);
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
void UpdateGameRunningState()
{
    if (g_run.state.gameState == GAME_RUNNING)
    {
        if (g_run.state.inputState == BATTLE)
        {
            if (GetButtonA())
            {
                bool b = BattleMenuCommand();
                if (!b) return; //No state change for menu input
                SetBattleState(BATTLE_ATTACK);
            }

            if (GetButtonB())
            {
                ExitMenu();
            }

            if (GetButtonX())
            {
            }

            if (GetButtonY())
            {
            }

            if (GetButtonJSClick())
            {
            }

            if (GetButtonDPClick())
            {
            }

            if (GetJSPressed())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    UpdateBattleMenu();
            }

            if (GetDPPressed())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    UpdateBattleMenu();
            }
            return;
        }


        if (g_run.state.inputState == MENU)
        {
            if (GetButtonA())
            {
                if (!OpenSubMenu())
                {
                    SetInputState(MOVING);
                }
                return;
            }

            if (GetButtonB())
            {
                if (!MenuBack())
                {
                    SetInputState(MOVING);
                    SetGameLoopRateDefault();
                    FullRedraw();
                }

                return;
            }

            if (GetButtonX())
            {
            }

            if (GetButtonY())
            {
            }

            if (GetButtonJSClick())
            {
            }

            if (GetButtonDPClick())
            {
            }

            if (GetJSPressed())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    OpenSubMenu();
                return;
            }


            if (GetDPPressed())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    OpenSubMenu();
                return;
            }
        }


        if (g_run.state.inputState == MOVING)
        {
            if (GetButtonA())
            {
                PlayerInteractItemInCell();
            }

            if (GetButtonB())
            {
                PlayerInteractObjectInCell();
            }

            if (GetButtonX())
            {
            }

            if (GetButtonY())
            {
                InitMainMenu();
                SetInputState(MENU);
                SetGameLoopRate(MENU_INPUT_POLLING_RATE);
                return;
            }

            if (GetButtonJSClick())
            {
            }

            if (GetButtonDPClick())
            {
            }

            if (GetJSPressed())
            {
                SetPlayerDelta(GetInputKeyState().d);
                return;
            }

            if (GetDPPressed())
            {
                SetPlayerDelta(GetInputKeyState().d);
                return;
            }
        }
    }
}


void UpdateGameTitleState()
{
    if (g_run.state.gameState == TITLE_SCREEN)
    {
        if (GetButtonA())
        {
            SetGameState(GAME_RUNNING);
            return;
        }

        if (GetButtonB())
        {
        }

        if (GetButtonX())
        {
        }

        if (GetButtonY())
        {
        }

        if (GetButtonJSClick())
        {
        }

        if (GetButtonDPClick())
        {
        }

        if (GetJSPressed())
        {
        }

        if (GetDPPressed())
        {
        }

        if (GetJSPressed())
        {
        }

        if (GetDPPressed())
        {
        }
    }
}

/**********************************************************************************************************************/
/**  Game State forking
**********************************************************************************************************************/
void HandleGameState()
{
    if (g_run.state.inputState == MOVING)
    {
        UpdateGame();
        RenderObjects();
    }
    if (g_run.state.inputState == MENU)
    {
        HandleMenu();
        HandleGameMenu();
        DrawCursor();
    }
    if (g_run.state.inputState == BATTLE)
    {
        if (CheckBattleState(BATTLE_INIT))
        {
            AnimationScreenClearRandom(); //ANIMATION - move both creatures into place
            HandleBattle();
            HandleBattleMenu();
            SetBattleState(BATTLE_MENUS);
        }
        else if (CheckBattleState(BATTLE_ATTACK))
        {
            BattlerAnimationAttack(true); //attacking animation
            BattlerAnimationStruck(false); //hit animation
            AnimationUpdateHealth(true);
            if (!CheckPlayerAttackOutcome())
            {
                AnimationBattlerDie(false);
                DestroyEnemyCreature();
                AnimationScreenFade(); //ANIMATION - enemy creature drops off screen
                FullRedraw();
                SetInputState(MOVING);
                return;
            }

            UseSkill(false);
            BattlerAnimationAttack(false); //attacking animation
            BattlerAnimationStruck(true); //hit animation
            AnimationUpdateHealth(false);
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
            HandleBattleMenu();
        }
        DrawCursor();
    }

    PlaySoundEffect();
}

/**********************************************************************************************************************/
/**  main game state update loop
**********************************************************************************************************************/
bool GameLoopTitleScreen()
{
    FillScreen(g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]); // TODO: update to real title screen

    while (g_run.state.gameState == TITLE_SCREEN)
    {
        // Pico_AnimationTitle();
        HandleInput();
        UpdateGameTitleState();
        GameLoopRateDelay();
    }

    InitGame();
    return true;
}


bool GameLoopMain()
{
    FullRedraw();
    while (g_run.state.gameState == GAME_RUNNING)
    {
        HandleInput();
        UpdateGameRunningState();
        HandleGameState();
        GameLoopRateDelay();
    }

    DEBUG("GameLoopMain break, %d", g_run.state.gameState);
    return true;
}

bool GameLoopNewMap()
{
    NewMap();
    SetGameState(GAME_RUNNING);
    return true;
}


bool GameLoopShutdown()
{
    return false;
}


typedef bool (*GameLoopFunc)();
GameLoopFunc GameLoopState[GAME_STATE_SIZE] = {GameLoopTitleScreen, GameLoopMain, GameLoopNewMap, GameLoopShutdown};

/**********************************************************************************************************************/
/**  main game state update loop
**********************************************************************************************************************/
void GameLoop()
{
    SetGameState(TITLE_SCREEN);
    SetInputState(MOVING);

    while (1)
    {
        bool b = GameLoopState[g_run.state.gameState]();
        if (!b) return;
    }
}
