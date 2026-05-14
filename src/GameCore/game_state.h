//
// Created by nathanial on 2/27/26.
//
#pragma once
#include "lib_decl.h"
#include "types.h"

State UpdateGameState(State state);
State SetGameState(GameState state);
State SetInputState(InputState state);
State SetBattleState(BattleState state);
bool CheckBattleState(BattleState state);

void GameLoopRateDelay(HardwareInterface hardware);
void SetGameLoopRate(uint16_t time);
void GameLoop(GameInterface *spi);
void SetGameLoopRateDefault();
