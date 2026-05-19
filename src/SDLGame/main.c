//
// Created by nathanial on 3/3/26.
//


#include "constants.h"
#include "core.h"
#include "memory_ram.h"


#define SDL 1

int main(void)
{
  InitGame();

  while (g_core.state.gameState == GAME_RUNNING)
  {


  }



  DEBUG("SDL GAME! %d", SDL);
  return 0;
}
