//
// Created by nathanial on 4/7/26.
//

#include "creatures.h"

#include "lib_debugging.h"
#include "memory_rom.h"
#include "stats.h"
#include "types.h"


void CreatureTests()
{
    uint8_t max_level = 99;
    uint8_t max_stat = 200;

    for (uint8_t l = 0; l <= max_level; l++)
        for (uint8_t c = 0; c < CREATURE_COUNT; c++)
        {
            Stats s = GetStats(c, l);
            const char* name = g_gameFlash.text.names.monsters[c];
            if (s.attack > max_stat || s.defence > max_stat || s.magic > max_stat || s.speed > max_stat)
                DEBUG("%s %d:      %d %d %d %d ", name, l, s.attack, s.defence, s.magic, s.speed);
        }
}
