//
// Created by nathanial on 4/13/26.
//
#pragma once

#include "types.h"


void GetSkills(MemoryInterface memory, EntityId id, Type type);
bool UseSkill(HardwareInterface hardware, MemoryInterface memory, bool player);

bool CastSpell(HardwareInterface hardware, MemoryInterface memory, SpellId spellID, EntityId partyID, EntityId enemyID);


bool UseItem(MemoryInterface memory, EntityId item_id, EntityId e_id);

bool InteractObject(MemoryInterface memory, HardwareInterface hardware, EntityId object_id, EntityId e_id);
