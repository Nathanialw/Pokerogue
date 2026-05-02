//
// Created by nathanial on 4/30/26.
//

#include "pico_ram.h"

Data g_pico_ram = {0};

void InitRam()
{
    g_pico_ram.input.defaultSleepTime = 120; //120
    g_pico_ram.input.sleepTime = 120; //120
    g_pico_ram.input.menuSleepTime = 200; //200

    g_pico_ram.sound.master_volume = 32768;
}
