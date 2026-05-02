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
bool SkillNoSkill(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FIRE SKILLS

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlameBlast(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFireball(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillEmber(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIncinerate(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCombustion(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlare(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerAccuracy(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
//UNUSED
bool SkillHeatWave(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlamePunch(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPyre(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    DEBUG("Pyre");
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_run.player.partyID[i] == NO_ENTITY) continue;
            RaiseMagic(g_run.player.partyID[i]);
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
bool SkillScorch(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCinderStorm(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerSpeed(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMagmaBurst(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSunfury(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBlazingCharge(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    // ensure this attack happens before anything
    NoEffect();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAshCloud(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerAccuracy(defenderID);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillInferno(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWildfire(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyBurn(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ICE SKILLS

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBlizzard(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIceShard(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFrostBite(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFreeze(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillHailstorm(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillGlacialSpike(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillColdSnap(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NextAttackFreezes();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIceArmor(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    FreezeAttackers(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFrostNova(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(defenderID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_run.player.partyID[i] == NO_ENTITY) continue;
            Attack(attackerID, defenderID, abilityData);
        }
    }
    else
    {
        Attack(attackerID, defenderID, abilityData);
    }

    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPermafrost(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSnowBlind(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerAccuracy(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillIceLance(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCryoBlast(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    ApplyFrozen(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillArcticWinds(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFrozen(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFrozenOrb(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPolarVortex(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(defenderID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_run.player.partyID[i] == NO_ENTITY) continue;
            Attack(attackerID, defenderID, abilityData);
        }
    }
    else
    {
        Attack(attackerID, defenderID, abilityData);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WATER SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCavitation(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillHypervision(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseAccuracy(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillShellbreaker(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerDefence(defenderID);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTorrent(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWhirlpool(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDrowningGrasp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTidalWave(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWaterWhip(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAquaJet(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillRiptide(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillHydroBlast(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDeepCurrent(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMaelstrom(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBrineSpray(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSurge(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPressureCrush(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PLANT SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillVineWhip(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPollenCloud(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillThornBarrage(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBloom(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillRootAnchor(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillForestWard(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillGravityRoot(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSporeExplosion(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillLeafStorm(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillBrambleTrap(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSeedVolley(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillVerdantGrowth(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillToxicIvy(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCanopyShade(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPetalBlitz(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    // ensure this attack hits before anything
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillOvergrowth(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ABERRATION SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillConsume(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPsychicShock(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillRealitySlip(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMaddeningGaze(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    AbandonTeam(defenderID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillVoidRift(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAnomaly(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDissonance(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillCorrupt(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillMindSpike(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillEldritchBlast(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWarpPulse(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillThoughtLeech(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSanityBreak(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAstralChains(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

// UNUSED
bool SkillChaosNova(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}

// UNUSED
bool SkillDimensionalTear(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    NoEffect();
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPetrifyingGaze(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    InstantKill(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillPetrifyingTouch(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    InstantKill(attackerID, defenderID, abilityData);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DRAGONS SKILLS
/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTailWhip(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWingBuffet(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDragonBreath(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFearRoar(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillFlyby(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillScalesHarden(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillAncientWrath(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTreasureHunt(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillSkyDive(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillDraconicFury(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillTailSweep(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillEmberRoar(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillScaleStorm(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool SkillWyrmCharge(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAncientGuard(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGoldenInstinct(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// GIANTS SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBodySlam(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStomp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEarthquake(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMightyThrow(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGroundPound(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCrush(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTitanSwing(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMountainBreaker(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBoulderToss(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSeismicSlam(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillIronGrip(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTremorStep(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillColossalCharge(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLandslide(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBedrockShield(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTitanicRoar(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BEAST SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillScratch(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPounce(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFeralRage(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHuntingInstinct(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPackCall(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTerrifyingRoar(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBloodScent(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillViciousBite(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRipAndTear(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAmbushStrike(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    // only quick attack if the enemy has not attacked otherwise just a normal attack
    if (!QuickAttack(attackerID, defenderID, abilityData))
        Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLunge(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHowlOfThePack(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_run.player.partyID[i] == NO_ENTITY) continue;
            RaiseStrength(g_run.player.partyID[i]);
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
bool SkillSavageMaul(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPredatorFocus(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillThroatClamp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStalk(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FEY SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPixieDust(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySleep(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGlamour(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTricksterTreat(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEnchantingMelody(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFaerieFire(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDreamDust(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySleep(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWildMagic(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGladeTeleport(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMoonbeam(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMirrorGlamour(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFaeWhisper(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPetalDance(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStarSprinkle(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMysticTangle(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLuminaBlessing(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTwilightShimmer(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEMIGOD SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPrayer(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_run.player.partyID[i] == NO_ENTITY) continue;
            HealTarget(g_run.player.partyID[i], abilityData.power);
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
bool SkillDivineSmite(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBlessing(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMiracle(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillJudgement(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAuraOfReverence(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillChannelDivinity(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillApotheosis(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHolyLight(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSacredShield(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCelestialCall(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPurify(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RemoveDisease(attackerID);
    RemoveCurse(attackerID);
    RemovePoison(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDivineIntervention(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHaloOfGrace(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRighteousFury(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSeraphicWard(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// HUMANOID SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTackle(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSlash(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBash(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPreciseStrike(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseAccuracy(attackerID);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillExorcism(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTaunt(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRally(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    if (IsInParty(attackerID))
    {
        for (uint8_t i = 0; i < MAX_PARTY_SIZE; ++i)
        {
            if (g_run.player.partyID[i] == NO_ENTITY) continue;
            RaiseStrength(g_run.player.partyID[i]);
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
bool SkillDisarm(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerStrength(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCharge(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    QuickAttack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillParry(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Parry(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCripplingBlow(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGuardBreak(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerDefence(defenderID);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSecondWind(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RestoreMana(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillIronWill(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillShieldBash(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTacticalRetreat(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Flee();
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ANCIENT SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPrimordialForce(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTimeWarp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyHaste(attackerID, abilityData.power);
    ApplySlow(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFossilize(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCallOfTheVoid(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerMagic(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWorldTear(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSleepOfAges(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAncientWisdom(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseMagic(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCatastrophe(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillArcaneEruption(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStoneTide(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTemporalShift(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyHaste(attackerID, abilityData.power);
    ApplySlow(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPrimalScream(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCosmicRay(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEternalGrasp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVolcanicOutburst(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRunicAegis(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// UNDEAD SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPlague(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLifeDrain(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGraveTouch(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyCurse(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHaunt(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyFear(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillNecroticBurst(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRaiseMinion(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Summon(SKELETON);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWither(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerStrength(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDeathsDoor(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillWail(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    InstantKill(attackerID, defenderID, abilityData);
    return true;
}

// UNUSED
bool SkillSoulSip(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBoneSpike(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCursedGround(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyCurse(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVampiricWave(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    HealTarget(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPhantomGrasp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTombShield(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDarkRitual(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    SacrificeHeal(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEpitaph(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEMONIC SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSultryLook(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillKiss(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHellfire(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCorruptingTouch(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    ApplyDisease(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDemonicPact(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSoulSiphon(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    DrainMana(defenderID, abilityData.power);
    RestoreMana(attackerID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTemptation(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillFelblaze(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillChaosWhisper(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLustfulGlare(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAbyssalGrasp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    Capture(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillCursedEmbrace(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillInfernalRush(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillDiabolicCry(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseStrength(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillHellscape(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TOXIC SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSting(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVenomSpit(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicCloud(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillParalyze(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAcidSpray(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    DestroyRandomPlayerItem();
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillNeurotoxin(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillContagion(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillAntidote(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RemovePoison(attackerID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicFang(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyPoison(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVirulentBite(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicBarrage(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillNerveStrike(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillToxicMist(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    PersistentPoisonCloud(abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillVenomousTail(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillInfectiousTouch(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyDisease(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillPurifyCure(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RemoveDisease(attackerID);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EARTHEN SKILLS
/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillSacrifice(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillRockThrow(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMudSlide(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStoneSkin(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    LowerDefence(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillQuicksand(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEarthenPrison(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplyParalyze(defenderID, abilityData.power);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLavaBurst(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTerraform(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillBoulderCrush(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillEarthquakeStomp(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillTremorWave(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillStoneSpike(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMudClaw(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    ApplySlow(defenderID, abilityData.power);
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillGeoBarrier(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    RaiseDefence(defenderID);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillMagmaFlow(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

/**********************************************************************************************************************
/*
**********************************************************************************************************************/
bool SkillLandShift(EntityId attackerID, EntityId defenderID, SkillData abilityData)
{
    Attack(attackerID, defenderID, abilityData);
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// NEW ADDED
