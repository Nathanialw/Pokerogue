//
// Created by nathanial on 2/27/26.
//

#include "game_state.h"

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
    DEBUG("SetBattleState %d", state);
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
State UpdateGameState(State state)
{


    if (state.gameState == GAME_RUNNING)
    {
        if (GetButtonX())
        {
            state.inputState = REBOOT;
        }


        if (state.inputState == BATTLE)
        {
            if (GetButtonX())
            {
            }

            if (GetButtonA())
            {
                bool b = BattleMenuCommand();
                if (!b) return state; //No state change for menu input
                SetBattleState(BATTLE_ATTACK);
                state.battleState = BATTLE_ATTACK;
                return state;
            }

            if (GetButtonB())
            {
                ExitMenu();
            }


            if (GetButtonJSClick())
            {
            }

            if (GetButtonJSClick())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    UpdateBattleMenu();
            }

            if (GetButtonDPClick())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    UpdateBattleMenu();
            }
            return state;
        }


        if (state.inputState == MENU)
        {
            if (GetButtonA())
            {
                if (!OpenSubMenu())
                {
                    state.inputState = MOVING;
                }
                return state;
            }


            if (GetButtonB())
            {
                if (!MenuBack())
                {
                    state.inputState = MOVING;
                }

                return state;
            }


            if (GetButtonDPClick())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    OpenSubMenu();
                return state;
            }


            if (GetButtonDPClick())
            {
                if (!SetMenuDelta(GetInputKeyState().d))
                    OpenSubMenu();
                return state;
            }
        }


        if (state.inputState == MOVING)
        {
            if (GetButtonA())
            {
            }


            if (GetButtonB())
            {
            }


            if (GetButtonY())
            {
                InitMainMenu();
                state.inputState = MENU;
                return state;
            }


            if (GetButtonJSClick())
            {
                SetPlayerDelta(GetInputKeyState().d);
                return state;
            }

            if (GetButtonDPClick())
            {
                SetPlayerDelta(GetInputKeyState().d);
                return state;
            }
        }
    }


    if (state.gameState == TITLE_SCREEN)
    {
        if (GetButtonA())
            state.gameState = GAME_RUNNING;
    }

    return state;
}

/**********************************************************************************************************************/
/**  Game State forking
**********************************************************************************************************************/
void HandleGameState(State updateState)
{

    if (updateState.inputState == MOVING)
    {
        DEBUG("MOVING");
        UpdateGame();
        RenderObjects();
    }
    if (updateState.inputState == MENU)
    {
        DEBUG("MENU");
        HandleMenu();
        HandleGameMenu();
        DrawCursor();
    }
    if (updateState.inputState == BATTLE)
    {
        DEBUG("BATTLE");
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

    DEBUG("play sound");
    PlaySoundEffect();
    DEBUG("GameLoopMain frame end");
}


/**********************************************************************************************************************/
/**  main game state update loop
**********************************************************************************************************************/
bool GameLoopTitleScreen()
{
    DEBUG("GameLoopTitleScreen");
    FillScreen(g_gameFlash.GetColor[PAL_OFF_WHITE_GRAY]); // TODO: update to real title screen

    while (g_run.state.gameState == TITLE_SCREEN)
    {
        // Pico_AnimationTitle();
        g_run.state = HandleInput(g_run.state);
        DEBUG("title input end");
    }

    InitGame();
    DEBUG("game init complete");
    return true;
}


bool GameLoopMain()
{
    DEBUG("GameLoopMain");
    FullRedraw();
    DEBUG("entering game loop");
    while (g_run.state.gameState == GAME_RUNNING)
    {
        DEBUG("starting input");
        g_run.state = HandleInput(g_run.state);
        DEBUG("GameLoopMain input end");
        HandleGameState(g_run.state);
        DEBUG("GameLoopMain frame end");
    }
    return true;
}

bool GameLoopNewMap()
{
    DEBUG("GameLoopNewMap");
    NewMap();
    g_run.state.gameState = GAME_RUNNING;
    return true;
}


bool GameLoopShutdown()
{
    DEBUG("GameLoopShutdown");
    return false;
}


typedef bool (*GameLoopFunc)();
GameLoopFunc GameLoopState[GAME_STATE_SIZE] = {GameLoopTitleScreen, GameLoopMain, GameLoopNewMap, GameLoopShutdown};

/**********************************************************************************************************************/
/**  main game state update loop
**********************************************************************************************************************/
void GameLoop()
{

    g_run.state.gameState = TITLE_SCREEN;
    g_run.state.inputState = MOVING;

    DEBUG("GameLoop %d", g_run.state.gameState);

    while (1)
    {
        bool b = GameLoopState[g_run.state.gameState]();
        if (!b) return;
    }
}
