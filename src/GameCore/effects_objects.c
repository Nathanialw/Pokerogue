//
// Created by nathanial on 2/22/26.
//

#include "effects.h"
#include "memory_rom.h"

#include "entities.h"
#include "lib_debugging.h"
#include "memory_ram.h"

/**********************************************************************************************************************/
/*
 *  OBJECT INTERACT EFFECTS
 *
**********************************************************************************************************************/

/**********************************************************************************************************************/
/*  TODO run the effect stored as the index in the metadata field
**********************************************************************************************************************/
bool InteractAltar(HardwareInterface hardware, EntityId item_id, EntityId e_id, ObjectData itemData)
{
    DEBUG("InteractAltar");
    return false;
}

/**********************************************************************************************************************/
/*  TODO keeps the player on the boat when moved on water
 *  TODO chance of random movement
**********************************************************************************************************************/
bool InteractRaft(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractRaft");
    return false;
}

/**********************************************************************************************************************/
/*  TODO keeps the player on the boat when moved on water
**********************************************************************************************************************/
bool InteractRowBoat(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractRowBoat");
    return false;
}

/**********************************************************************************************************************/
/*  TODO chance of breaking when walked on
**********************************************************************************************************************/
bool InteractRopeBridge(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractRopeBridge");
    NoEffect();
    return false;
}

/**********************************************************************************************************************/
/*  TODO chance of spawning a troll
**********************************************************************************************************************/
bool InteractStoneBridge(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractStoneBridge");
    NoEffect();
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractWoodenBridge(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWoodenBridge");
    NoEffect();
    return false;
}

/**********************************************************************************************************************/
/*  TODO generates a spellbook or skillbook item
**********************************************************************************************************************/
bool InteractBookCase(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractBookCase");
    return false;
}

/**********************************************************************************************************************/
/*  TODO generates a ring or amulet item
**********************************************************************************************************************/
bool InteractJewelleryCase(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractJewelleryCase");
    return false;
}

/**********************************************************************************************************************/
/*  TODO generates a random scroll
**********************************************************************************************************************/
bool InteractScrollCase(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractScrollCase");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractCauldren(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractCauldren");
    return false;
}

/**********************************************************************************************************************/
/*  TODO chance to do damage
/*  TODO chance to melt items in bag
**********************************************************************************************************************/
bool InteractAcidCloud(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractAcidCloud");
    return false;
}

/**********************************************************************************************************************/
/*  TODO chance to apply poison
**********************************************************************************************************************/
bool InteractPoisonGas(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractPoisonGas");
    return false;
}

/**********************************************************************************************************************/
/*  TODO chance to do damage
**********************************************************************************************************************/
bool InteractSmokeCloud(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractSmokeCloud");
    return false;
}

/**********************************************************************************************************************/
/*  TODO chance to spawn an undead
/*  TODO chance to generate an item
**********************************************************************************************************************/
bool InteractCoffin(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractCoffin");
    return false;
}

/**********************************************************************************************************************/
/*  TODO chance to spawn an undead
/*  TODO chance to generate an item
**********************************************************************************************************************/
bool InteractSarcophagus(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractSarcophagus");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractHangingCorpse(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractHangingCorpse");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractLaidCorpse(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractLaidCorpse");
    return false;
}

/**********************************************************************************************************************/
/*  TODO has collision
/*  TODO chance to break when the player moves onto
/*  TODO chance to to damage when the player moves onto
**********************************************************************************************************************/
bool InteractBarredDoor(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractBarredDoor");
    return false;
}

/**********************************************************************************************************************/
/*  TODO has collision
/*  TODO chance to break when the player moves onto
/*  TODO chance to to damage when the player moves onto
**********************************************************************************************************************/
bool InteractIronDoor(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractIronDoor");
    return false;
}

/**********************************************************************************************************************/
/*  TODO has collision
/*  TODO chance to break when the player moves onto
/*  TODO chance to to damage when the player moves onto
**********************************************************************************************************************/
bool InteractStoneDoor(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractStoneDoor");
    return false;
}

/**********************************************************************************************************************/
/*  TODO has collision
/*  TODO chance to break when the player moves onto
/*  TODO chance to to damage when the player moves onto
**********************************************************************************************************************/
bool InteractWickerDoor(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWickerDoor");
    return false;
}

/**********************************************************************************************************************/
/*  TODO has collision
/*  TODO chance to break when the player moves onto
/*  TODO chance to to damage when the player moves onto
**********************************************************************************************************************/
bool InteractWoodDoor(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWoodDoor");
    return false;
}

/**********************************************************************************************************************/
/*  TODO add a success chance
**********************************************************************************************************************/
bool InteractLadder(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractLadder");
    MapAscend(hardware, memory, g_core.player.id);
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractStairs(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractStairs");
    MapDescend(hardware, memory, g_core.player.id);
    return true;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractCave(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractCave");
    MapLateral(hardware, memory, g_core.player.id);
    return true;
}

/**********************************************************************************************************************/
/*  TODO add light radius
**********************************************************************************************************************/
bool InteractBonfire(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractBonfire");
    return false;
}

/**********************************************************************************************************************/
/*  TODO add light radius
**********************************************************************************************************************/
bool InteractBrazier(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractBrazier");
    return false;
}

/**********************************************************************************************************************/
/*  TODO add light radius
**********************************************************************************************************************/
bool InteractCampfire(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractCampfire");
    return false;
}

/**********************************************************************************************************************/
/*  TODO add light radius
**********************************************************************************************************************/
bool InteractStandingTorch(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractStandingTorch");
    return false;
}

/**********************************************************************************************************************/
/*  TODO add light radius
**********************************************************************************************************************/
bool InteractWallTorch(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWallTorch");
    return false;
}

/**********************************************************************************************************************/
/*  TODO add chacne to descend when walked on
/*  TODO deals damage
**********************************************************************************************************************/
bool InteractCrumblingFloor(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractCrumblingFloor");
    MapDescend(hardware, memory, g_core.player.id);
    return false;
}

/**********************************************************************************************************************/
/*  TODO deals damage
/*  TODO melts items
**********************************************************************************************************************/
bool InteractAcidFountain(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractAcidFountain");
    return false;
}

/**********************************************************************************************************************/
/*  TODO heals demons
**********************************************************************************************************************/
bool InteractBloodFountain(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractBloodFountain");
    return false;
}

/**********************************************************************************************************************/
/*  TODO heals
**********************************************************************************************************************/
bool InteractWaterFountain(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWaterFountain");
    return false;
}

/**********************************************************************************************************************/
/*  TODO no sprite
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractHiddenCompartment(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractHiddenCompartment");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractAcidPit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractAcidPit");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractLavaPit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractLavaPit");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractNarowPit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractNarowPit");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractShallowPit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractShallowPit");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractSnakesPit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractSnakesPit");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractSpkiedPit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractSpkiedPit");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractPitStandard(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractPitStandard");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractWaterPit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWaterPit");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractWidePit(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWidePit");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractArmorRack(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractArmorRack");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractWeaponRack(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWeaponRack");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractBarrel(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractBarrel");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractBasket(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractBasket");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractChest(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractChest");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractCoffer(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractCoffer");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractCrate(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractCrate");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractTrunk(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractTrunk");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractUrn(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractUrn");
    return false;
}

/**********************************************************************************************************************/
/*  TODO spawns an item
**********************************************************************************************************************/
bool InteractVault(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractVault");
    return false;
}

/**********************************************************************************************************************/
/*  TODO collision for random creatures
/*  TODO no collision for trainers
**********************************************************************************************************************/
bool InteractWard(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWard");
    return false;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractWell(HardwareInterface hardware, MemoryInterface memory,EntityId object_id, EntityId e_id, ObjectData objectData)
{
    DEBUG("InteractWell");
    return false;
}
