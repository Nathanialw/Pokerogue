//
// Created by nathanial on 4/11/26.
//

#include "status_effects.h"
#include "constants.h"
#include "memory_ram.h"


typedef uint8_t (*TurnEffect)(HardwareInterface hardware, uint8_t power, EntityId id);

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t UpdateStatus(HardwareInterface hardware, uint8_t n)
{
    if (n == 0) return 0;
    uint8_t chance = hardware.GetRandom_uint8_t(10, 100);
    if (chance <= n) n--;
    return n;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t HoveringEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t WaterWalkEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t WaterBreathingEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t RepelEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t InvisibilityEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t WallWalkingEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t FireEatingEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t LineOfSightEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t ParalyzedEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    g_core.creatures.speed[id].negativeEffect = false;
    uint8_t chance = hardware.GetRandom_uint8_t(10, 100);
    if (chance <= 10)
        g_core.creatures.speed[id].negativeEffect = true;
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t SleepEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    g_core.creatures.speed[id].negativeEffect = false;
    uint8_t chance = hardware.GetRandom_uint8_t(10, 100);
    if (chance <= 10)
        g_core.creatures.speed[id].negativeEffect = true;
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t PoisonEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    Int999ApplyValue(hardware, &g_core.creatures.hp[id], -power);
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t FrozenEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    g_core.creatures.speed[id].negativeEffect = false;
    uint8_t chance = hardware.GetRandom_uint8_t(10, 100);
    if (chance <= 10)
        g_core.creatures.speed[id].negativeEffect = true;
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t DiseaseEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t CurseEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    Int999ApplyValue(hardware, &g_core.creatures.hp[id], -power);
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t HastedEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t FearEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    g_core.creatures.speed[id].negativeEffect = false;
    uint8_t chance = hardware.GetRandom_uint8_t(10, 100);
    if (chance <= 10)
        g_core.creatures.speed[id].negativeEffect = true;
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t BurnedEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    Int999ApplyValue(hardware, &g_core.creatures.hp[id], -power);
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t LightEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    return 0;
}

/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
uint8_t SlowedEffect(HardwareInterface hardware, uint8_t power, EntityId id)
{
    g_core.creatures.speed[id].negativeEffect = false;
    uint8_t chance = hardware.GetRandom_uint8_t(10, 100);
    if (chance <= 10)
        g_core.creatures.speed[id].negativeEffect = true;
    return 0;
}


/**********************************************************************************************************************
*
**********************************************************************************************************************/
SET_MEMORY(".map")
static inline void UpdateStatusEffect(HardwareInterface hardware, uint8_t* status_effect, TurnEffect turnEffect)
{
    for (uint8_t i = 0; i < ENTITY_COUNT / 2; ++i)
    {
        if (status_effect[i] == 0) continue;
        uint8_t lower_unit = GetBottomByte(status_effect[i]);
        if (lower_unit > 0)
            turnEffect(hardware, lower_unit, i * 2);
        lower_unit = UpdateStatus(hardware, lower_unit);

        uint8_t upper_unit = GetTopByte(status_effect[i]);
        if (upper_unit > 0)
            turnEffect(hardware, upper_unit, (i * 2) + 1);
        upper_unit = UpdateStatus(hardware, upper_unit);

        //save updated effect
        status_effect[i] = lower_unit | (upper_unit << 8);
    }
}


SET_MEMORY(".map")
void UpdateObjectStatusEffects(HardwareInterface hardware)
{
    UpdateStatusEffect(hardware, g_core.creatures.status.hovering, HoveringEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.waterWalk, WaterWalkEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.waterBreathing, WaterBreathingEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.repel, RepelEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.invisibility, InvisibilityEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.wallWalking, WallWalkingEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.fireEating, FireEatingEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.lineOfSight, LineOfSightEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.paralyzed, ParalyzedEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.sleep, SleepEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.poison, PoisonEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.frozen, FrozenEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.disease, DiseaseEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.curse, CurseEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.hasted, HastedEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.fear, FearEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.burned, BurnedEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.light, LightEffect);
    UpdateStatusEffect(hardware, g_core.creatures.status.slowed, SlowedEffect);
}
