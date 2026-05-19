//
// Created by nathanial on 3/31/26.
//

#include "ui.h"

#include "memory_ram.h"
#include "memory_rom.h"
#include "entities.h"
#include "memory_access.h"
#include "utils.h"


/**********************************************************************************************************************/
/** Populates a given array of chars with with the given integer value formatted with the given prefix and suffix char arrays
**********************************************************************************************************************/
void CombatLogLine(HardwareInterface hardware, char* dest, const char* prefix, const char* suffix, uint16_t value)
{
    memset(dest, ' ', 40);

    uint8_t i = 0;
    while (prefix[i])
    {
        dest[i] = prefix[i];
        i++;
    }

    CharStr_max999 value_str;
    GetAsChars_Max999(value, &value_str);


    uint8_t j = i;
    i = 0;
    bool prepend_zeroes = false;
    while (i < 3)
    {
        char c = value_str[i];
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

    i = 0;
    while (suffix[i])
    {
        dest[j + i] = suffix[i];
        i++;
    }
}

/**********************************************************************************************************************/
/** Prints one line of the combat log
 *  Takes in the entity id and the damage value
 *  prepares the text buffers and copies them into the combat log cache
**********************************************************************************************************************/
void PrintCombatLog(HardwareInterface hardware, MemoryInterface memory, EntityId creatureID, uint16_t damage)
{
    const char suffix[] = " Frst Dmg";
    const char a[] = " hit ";
    const char b[] = " ";
    char attacker[SMALL_STRINGS];
    Flash_GetCreatureName(memory, attacker, GetCreatureType(creatureID));
    char target[SMALL_STRINGS];
    Flash_GetCreatureName(memory, target, GetCreatureType(creatureID));
    char prefix[40];

    uint8_t cursor = 0;
    uint8_t i = 0;
    while (attacker[i])
    {
        prefix[cursor] = attacker[i];
        i++;
        cursor++;
    }

    i = 0;
    while (a[i])
    {
        prefix[cursor] = a[i];
        i++;
        cursor++;
    }

    i = 0;
    while (target[i])
    {
        prefix[cursor] = target[i];
        i++;
        cursor++;
    }

    i = 0;
    while (b[i])
    {
        prefix[cursor] = b[i];
        i++;
        cursor++;
    }


    if (g_core.battleMode.combatLog[0][0] == '\0')
        CombatLogLine(hardware, g_core.battleMode.combatLog[0], prefix, suffix, damage);
    else
        CombatLogLine(hardware, g_core.battleMode.combatLog[1], prefix, suffix, damage);
}
