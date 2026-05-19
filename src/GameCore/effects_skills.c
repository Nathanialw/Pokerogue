//
// Created by nathanial on 2/22/26.
//

#include "memory_ram.h"
#include "memory_rom.h"
#include "stats.h"
#include "types.h"
#include "effects.h"
#include "lib_debugging.h"
#include "player.h"

/**********************************************************************************************************************/
/*
 *  Initial implementation is simple 1 or 2 effects on each skill.
 *  Effect Functions make it easy to mix and match any effects to build unique skills
 *
**********************************************************************************************************************/



/*****************************************************************************************************
 *
 *  WORK IN PROGRESS - NEED TO ADD MORE EFFECTS TO MIX AND MATCH FOR MORE INTERESTING MOVES
 *  GOAL IS TO HAVE 4 - 8 EFFECTS PER SKILL WITH AS MANY CUSTOM 1-OFF EFFECTS AS I CAN THINK UP
 *
 ******************************************************************************************************
 */


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillNoSkill(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FIRE SKILLS

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlameBlast(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFireball(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillEmber(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIncinerate(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCombustion(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlare(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerAccuracy(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
//UNUSED
bool SkillHeatWave(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlamePunch(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPyre(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    DEBUG("Pyre");
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_core.player.partyID[i] == NO_ENTITY) continue;
            RaiseMagic(g_core.player.partyID[i]);
        }
    }
    else
    {
        RaiseMagic(attackerID);
    }
    DEBUG("Done Pyre");
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillScorch(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCinderStorm(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerSpeed(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMagmaBurst(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSunfury(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBlazingCharge(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    // ensure this attack happens before anything
    NoEffect();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAshCloud(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerAccuracy(defenderID);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillInferno(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWildfire(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ICE SKILLS

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBlizzard(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIceShard(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFrostBite(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFreeze(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillHailstorm(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillGlacialSpike(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillColdSnap(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NextAttackFreezes();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIceArmor(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    FreezeAttackers(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFrostNova(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(defenderID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_core.player.partyID[i] == NO_ENTITY) continue;
            Attack(hardware, memory, attackerID, defenderID, abilityData);
        }
    }
    else
    {
        Attack(hardware, memory, attackerID, defenderID, abilityData);
    }

    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPermafrost(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSnowBlind(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerAccuracy(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIceLance(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCryoBlast(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    ApplyFrozen(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillArcticWinds(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFrozenOrb(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPolarVortex(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(defenderID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_core.player.partyID[i] == NO_ENTITY) continue;
            Attack(hardware, memory, attackerID, defenderID, abilityData);
        }
    }
    else
    {
        Attack(hardware, memory, attackerID, defenderID, abilityData);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WATER SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCavitation(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillHypervision(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseAccuracy(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillShellbreaker(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerDefence(defenderID);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTorrent(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWhirlpool(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDrowningGrasp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTidalWave(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWaterWhip(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAquaJet(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillRiptide(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillHydroBlast(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDeepCurrent(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMaelstrom(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBrineSpray(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSurge(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPressureCrush(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PLANT SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillVineWhip(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPollenCloud(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillThornBarrage(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBloom(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillRootAnchor(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillForestWard(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillGravityRoot(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSporeExplosion(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillLeafStorm(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBrambleTrap(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSeedVolley(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillVerdantGrowth(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillToxicIvy(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCanopyShade(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPetalBlitz(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    // ensure this attack hits before anything
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillOvergrowth(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ABERRATION SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillConsume(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPsychicShock(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillRealitySlip(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMaddeningGaze(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    AbandonTeam(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillVoidRift(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAnomaly(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDissonance(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCorrupt(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMindSpike(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillEldritchBlast(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWarpPulse(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillThoughtLeech(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSanityBreak(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAstralChains(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

// UNUSED
bool SkillChaosNova(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}

// UNUSED
bool SkillDimensionalTear(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPetrifyingGaze(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    InstantKill(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPetrifyingTouch(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    InstantKill(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DRAGONS SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTailWhip(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWingBuffet(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDragonBreath(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFearRoar(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlyby(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillScalesHarden(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAncientWrath(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTreasureHunt(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSkyDive(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDraconicFury(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTailSweep(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillEmberRoar(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillScaleStorm(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWyrmCharge(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAncientGuard(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGoldenInstinct(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GIANTS SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBodySlam(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStomp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEarthquake(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMightyThrow(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGroundPound(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCrush(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTitanSwing(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMountainBreaker(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBoulderToss(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSeismicSlam(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillIronGrip(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTremorStep(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillColossalCharge(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLandslide(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBedrockShield(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTitanicRoar(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BEAST SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillScratch(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPounce(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFeralRage(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHuntingInstinct(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPackCall(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTerrifyingRoar(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBloodScent(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillViciousBite(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRipAndTear(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAmbushStrike(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    // only quick attack if the enemy has not attacked otherwise just a normal attack
    if (!QuickAttack(hardware, memory, attackerID, defenderID, abilityData))
        Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLunge(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHowlOfThePack(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_core.player.partyID[i] == NO_ENTITY) continue;
            RaiseStrength(g_core.player.partyID[i]);
        }
    }
    else
    {
        RaiseStrength(attackerID);
    }
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSavageMaul(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPredatorFocus(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillThroatClamp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStalk(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FEY SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPixieDust(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySleep(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGlamour(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTricksterTreat(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEnchantingMelody(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFaerieFire(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDreamDust(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySleep(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWildMagic(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGladeTeleport(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMoonbeam(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMirrorGlamour(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFaeWhisper(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPetalDance(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStarSprinkle(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMysticTangle(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLuminaBlessing(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTwilightShimmer(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEMIGOD SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPrayer(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_core.player.partyID[i] == NO_ENTITY) continue;
            HealTarget(g_core.player.partyID[i], abilityData.power);
        }
    }
    else
    {
        HealTarget(attackerID, abilityData.power);
    }
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDivineSmite(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBlessing(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMiracle(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillJudgement(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAuraOfReverence(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillChannelDivinity(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillApotheosis(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHolyLight(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSacredShield(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCelestialCall(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPurify(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RemoveDisease(attackerID);
    RemoveCurse(attackerID);
    RemovePoison(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDivineIntervention(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHaloOfGrace(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRighteousFury(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSeraphicWard(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HUMANOID SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTackle(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSlash(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBash(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPreciseStrike(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseAccuracy(attackerID);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillExorcism(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTaunt(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRally(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_core.player.partyID[i] == NO_ENTITY) continue;
            RaiseStrength(g_core.player.partyID[i]);
        }
    }
    else
    {
        RaiseStrength(attackerID);
    }
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDisarm(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerStrength(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCharge(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    QuickAttack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillParry(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Parry(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCripplingBlow(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGuardBreak(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerDefence(defenderID);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSecondWind(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RestoreMana(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillIronWill(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillShieldBash(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTacticalRetreat(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Flee();
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ANCIENT SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPrimordialForce(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTimeWarp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyHaste(attackerID, abilityData.power);
    ApplySlow(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFossilize(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCallOfTheVoid(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerMagic(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWorldTear(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSleepOfAges(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAncientWisdom(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCatastrophe(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillArcaneEruption(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStoneTide(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTemporalShift(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyHaste(attackerID, abilityData.power);
    ApplySlow(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPrimalScream(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCosmicRay(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEternalGrasp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVolcanicOutburst(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRunicAegis(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// UNDEAD SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPlague(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLifeDrain(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGraveTouch(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyCurse(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHaunt(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillNecroticBurst(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRaiseMinion(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Summon(SKELETON);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWither(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerStrength(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDeathsDoor(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWail(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    InstantKill(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

// UNUSED
bool SkillSoulSip(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBoneSpike(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCursedGround(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyCurse(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVampiricWave(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPhantomGrasp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTombShield(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDarkRitual(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    SacrificeHeal(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEpitaph(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEMONIC SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSultryLook(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillKiss(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHellfire(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCorruptingTouch(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    ApplyDisease(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDemonicPact(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSoulSiphon(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    DrainMana(defenderID, abilityData.power);
    RestoreMana(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTemptation(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFelblaze(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillChaosWhisper(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLustfulGlare(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAbyssalGrasp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCursedEmbrace(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillInfernalRush(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDiabolicCry(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHellscape(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TOXIC SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSting(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVenomSpit(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicCloud(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillParalyze(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAcidSpray(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    DestroyRandomPlayerItem();
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillNeurotoxin(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillContagion(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAntidote(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RemovePoison(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicFang(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVirulentBite(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicBarrage(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillNerveStrike(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicMist(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    PersistentPoisonCloud(abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVenomousTail(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillInfectiousTouch(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPurifyCure(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RemoveDisease(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EARTHEN SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSacrifice(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRockThrow(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMudSlide(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStoneSkin(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerDefence(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillQuicksand(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEarthenPrison(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLavaBurst(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTerraform(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBoulderCrush(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEarthquakeStomp(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTremorWave(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStoneSpike(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMudClaw(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGeoBarrier(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMagmaFlow(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLandShift(HardwareInterface hardware,  MemoryInterface memory, EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(hardware, memory, attackerID, defenderID, abilityData);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// NEW ADDED
