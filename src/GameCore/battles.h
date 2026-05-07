//
// Created by nathanial on 4/13/26.
//
#pragma once

#include "types.h"


void GetSkills(EntityId id, Type type);
bool UseSkill(bool player);

bool CastSpell(SpellId spellID, EntityId partyID, EntityId enemyID);


bool UseItem(EntityId item_id, EntityId e_id);

bool InteractObject(EntityId object_id, EntityId e_id);
