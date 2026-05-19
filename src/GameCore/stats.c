//
// Created by nathanial on 2/22/26.
//

#include "stats.h"

#include "lib_debugging.h"
#include "lib_types.h"
#include "memory_access.h"

#include "memory_rom.h"
#include "memory_ram.h"
#include "utils.h"

/**********************************************************************************************************************/
/** extracts growth values from bytes 4 bits each
**********************************************************************************************************************/
SET_MEMORY(".map")
static inline uint8_t GrowthAttack(uint16_t g) { return (g >> 12) & 0xF; }

SET_MEMORY(".map")
static inline uint8_t GrowthDefence(uint16_t g) { return (g >> 8) & 0xF; }

SET_MEMORY(".map")
static inline uint8_t GrowthMagic(uint16_t g) { return (g >> 4) & 0xF; }

SET_MEMORY(".map")
static inline uint8_t GrowthSpeed(uint16_t g) { return g & 0xF; }


SET_MEMORY(".map.rodata")
static const uint8_t growth_table[8] = {1, 2, 3, 4, 5, 6, 7, 8};


SET_MEMORY(".map.rodata")
static const char str_spawn_creature_type[] = "1";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_skills[] = "2";
SET_MEMORY(".map.rodata")
static const char str_spawn_creature_done[] = "3";

/**********************************************************************************************************************/
/** Returns the stats of a given creature type and level
**********************************************************************************************************************/
SET_MEMORY(".map")
void GetStats(HardwareInterface hardware, MemoryInterface memory, Stats *stats, Creature type, uint8_t level)
{
    memory.Print(str_spawn_creature_type);

    Flash_GetCreatureStatsRange(memory, &g_map.statsCache, type);
    Stats minStats = g_map.statsCache.min;
    Stats maxStats = g_map.statsCache.max;
    memory.Print(str_spawn_creature_skills);

    uint8_t growth = Flash_GetStatGrowth(memory, type);

    memory.Print(str_spawn_creature_done);
    uint8_t a = growth_table[GrowthAttack(growth)];
    uint8_t d = growth_table[GrowthDefence(growth)];
    uint8_t m = growth_table[GrowthMagic(growth)];
    uint8_t s = growth_table[GrowthSpeed(growth)];

    memory.Print(str_spawn_creature_type);

    stats->attack = hardware.GetRandom_uint8_t(minStats.attack, maxStats.attack);
    stats->defence = hardware.GetRandom_uint8_t(minStats.defence, maxStats.defence);
    stats->magic = hardware.GetRandom_uint8_t(minStats.magic, maxStats.magic);
    stats->speed = hardware.GetRandom_uint8_t(minStats.speed, maxStats.speed);

    stats->attack += a * (level / 4);
    stats->defence += d * (level / 4);
    stats->magic += m * (level / 4);
    stats->speed += s * (level / 4);
}

/**********************************************************************************************************************/
/** Takes in an entity id
 *  calculate the xp required for the next level
 *  Saves it into the Max value of the entity xp array value
 *  Sets current x to 0
**********************************************************************************************************************/
SET_MEMORY(".map")
void SetXPToLevel(EntityId id, IntMax999 xp)
{
    const Stats stats = g_core.creatures.stats[id];
    const Int99 level = g_core.creatures.level[id];
    const uint16_t total_stats = stats.magic + stats.attack + stats.defence + stats.speed + (50 * level.value);

    Int999SetCurrent(&xp, 0);
    Int999SetMax(&xp, total_stats / 10);
}

/**********************************************************************************************************************/
/** Retrieves the xp value of a killed creature and adds it to the xp of the slaying creature
 *  handles level up
**********************************************************************************************************************/
SET_MEMORY(".core")
void GainXP(EntityId id, EntityId dead_id)
{
    Stats stats = g_core.creatures.stats[dead_id];
    uint8_t level = g_core.creatures.level[dead_id].value;
    uint16_t total_stats = stats.magic + stats.attack + stats.defence + stats.speed + (50 * level);
    uint16_t xp_value = total_stats / 20;

    IntMax999 xp = g_core.creatures.xp[id];
    uint16_t cur = Int999GetCurrent(&xp);
    uint16_t max = Int999GetMax(&xp);
    uint16_t new_xp = (cur + xp_value);
    if (new_xp < max)
    {
        Int999SetCurrent(&xp, (cur + xp_value));
    }
    else
    {
        g_core.creatures.level[id].value++;
        new_xp = new_xp - max;
        SetXPToLevel(id, xp);
        Int999SetCurrent(&xp, new_xp);
    }

    g_core.creatures.xp[id] = xp;
}

/**********************************************************************************************************************/
/**Returns the current and max hp of a creature by type and level
 *  TODO get the values from the DB values and calc with growth
**********************************************************************************************************************/
SET_MEMORY(".map")
IntMax999 GetHP(Creature type, uint8_t level)
{
    IntMax999 k = {0};
    Int999SetCurrent(&k, level * 20);
    Int999SetMax(&k, level * 20);
    return k;
}

/**********************************************************************************************************************/
/**Returns the current and max mp of a creature by type and level
 *  TODO get the values from the DB values and calc with growth
**********************************************************************************************************************/
SET_MEMORY(".map")
IntMax999 GetMP(Creature type, uint8_t level)
{
    IntMax999 k = {0};
    Int999SetCurrent(&k, level * 10);
    Int999SetMax(&k, level * 10);
    return k;
}

/**********************************************************************************************************************/
/** modifies a char array with the formatted values of "min/max" of a resource
**********************************************************************************************************************/
void GetStatLine(HardwareInterface hardware, uint16_t cur, uint16_t max, uint8_t max_chars, char* dest, const StatusPrefix prefix)
{
    hardware.MemSet(dest, '\0', max_chars);

    for (int i = 0; i < 3 && prefix[i]; i++)
    {
        dest[i] = prefix[i];
    }

    CharStr_max999 hp_str;
    GetAsChars_Max999(cur, &hp_str);

    CharStr_max999 max_hp_str;
    GetAsChars_Max999(max, &max_hp_str);

    uint8_t i = 0;
    uint8_t j = 3;
    bool prepend_zeroes = false;
    while (i < 3)
    {
        char c = hp_str[i];
        if (c == '0' && !prepend_zeroes)
        {
            dest[j] = ' ';
        }
        else
        {
            prepend_zeroes = true;
            dest[j] = c;
        }

        i++;
        j++;
    }

    dest[6] = '/';

    i = 0;
    j = 7;
    prepend_zeroes = false;
    while (i < 3)
    {
        char c = max_hp_str[i];
        if (c == '0' && !prepend_zeroes)
        {
        }
        else
        {
            prepend_zeroes = true;
            dest[j] = c;
            j++;
        }
        i++;
    }
}
