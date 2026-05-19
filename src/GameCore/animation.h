//
// Created by nathanial on 4/10/26.
//
#pragma once
#include "lib_decl.h"
#include "types.h"

void AnimationUpdateHealth(GraphicsInterface graphics, HardwareInterface hardware, bool attackersTurn);
void BattlerAnimationAttack(GraphicsInterface graphics, MemoryInterface memory, bool player);
void BattlerAnimationStruck(GraphicsInterface graphics, MemoryInterface memory, bool player);
void AnimationBattlerStart(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface, bool onAttacker);
void AnimationBattlerDie(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface, bool onAttacker);
void AnimationScreenFade(GraphicsInterface graphics, HardwareInterface hardware);
void AnimationScreenClearRandom(GraphicsInterface graphics, HardwareInterface hardware);
void AnimationTitle(GraphicsInterface graphics, HardwareInterface hardware);
void AnimationUpdateXP(GraphicsInterface graphics, HardwareInterface hardware);
void AnimationUpdateMana(GraphicsInterface graphics, HardwareInterface hardware);
void AnimationMovement(GraphicsInterface graphics, HardwareInterface hardware, MemoryInterface);
