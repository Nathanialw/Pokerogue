//
// Created by nathanial on 4/10/26.
//

#include "animation_tests.h"

#include "lib_debugging.h"
#include "memory_rom.h"


void CheckAnimationImplemented()
{
    DEBUG("=== CheckAnimationImplemented() ===");
    uint8_t n = 0;
    uint16_t t = 0;

    for (uint8_t i = 0; i < ABILITY_COUNT; i++)
    {
        bool b = g_gameFlash.animation.skillsAttack[i](true);
        if (b) n++;
    }
    DEBUG("skills attack %d/%d", n, ABILITY_COUNT);
    t += n;
    n = 0;

    for (uint8_t i = 0; i < ITEM_COUNT; i++)
    {
        bool b = g_gameFlash.animation.itemsAttack[i](true);
        if (b) n++;
    }
    DEBUG("items attack %d/%d", n, ITEM_COUNT);
    t += n;
    n = 0;

    for (uint8_t i = 0; i < SPELL_COUNT; i++)
    {
        bool b = g_gameFlash.animation.spellsAttack[i](true);
        if (b) n++;
    }

    DEBUG("spell attack %d/%d", n, SPELL_COUNT);
    t += n;
    n = 0;

    for (uint8_t i = 0; i < ABILITY_COUNT; i++)
    {
        bool b = g_gameFlash.animation.skillsStruck[i](true);
        if (b) n++;
    }
    DEBUG("skills struck %d/%d", n, ABILITY_COUNT);
    t += n;
    n = 0;

    for (uint8_t i = 0; i < ITEM_COUNT; i++)
    {
        bool b = g_gameFlash.animation.itemsStruck[i](true);
        if (b) n++;
    }
    DEBUG("items struck %d/%d", n, ITEM_COUNT);
    t += n;
    n = 0;

    for (uint8_t i = 0; i < SPELL_COUNT; i++)
    {
        bool b = g_gameFlash.animation.spellsStruck[i](true);
        if (b) n++;
    }
    DEBUG("spell struck %d/%d", n, SPELL_COUNT);
    t += n;

    uint16_t total = (ABILITY_COUNT + ITEM_COUNT + SPELL_COUNT) * 2;
    uint16_t a = ((float)t / (float)total) * 100;
    uint16_t d = ((((float)t / (float)total) * 100) - a) * 10;
    DEBUG("total %d/%d - %d.%d%% Done!", t, total, a, d);
}
