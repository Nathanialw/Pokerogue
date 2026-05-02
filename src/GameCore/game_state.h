//
// Created by nathanial on 2/27/26.
//
#pragma once
#include "types.h"

State UpdateGameState(State state);
State SetGameState(GameState state);
State SetInputState(InputState state);
State SetBattleState(BattleState state);
bool CheckBattleState(BattleState state);


void GameLoop();


/**********************************************************************************************************************/
/**  IMPLEMENTATION DEFINED
**********************************************************************************************************************/
State HandleInput(State state);
