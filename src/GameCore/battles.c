//
// Created by nathanial on 4/13/26.
//

#include "battles.h"

#include "entities.h"
#include "lib_debugging.h"
#include "memory_access.h"
#include "memory_ram.h"
#include "memory_rom.h"


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
void GetSkills(MemoryInterface memory, EntityId id, Type type)
{
    uint8_t i = 0;
    while (i < MAX_ABILITIES)
    {
        g_core.creatures.attacks[id][i] = NO_ABILITY;
        i++;
    }

    CreatureSkillLearnLevels skills = {0};
    Flash_GetSkill(memory, skills, type, i);

    for (uint8_t i = 0; i < MAX_ABILITIES; ++i)
        g_core.creatures.attacks[id][i] = skills.c[i].skillID;
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".battle")
bool UseSkill(HardwareInterface hardware, MemoryInterface memory, bool player)
{
    EntityId player_creature_id = g_core.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_core.battleMode.enemyMonsterID;
    Ability ability;

    if (player)
    {
        g_core.battleMode.combatLog[0][0] = '\0';
        ability = g_core.creatures.attacks[player_creature_id][g_core.menu.sel->y];
        SkillData ability_data = Flash_GetSkillData(memory, ability);
        Flash_GetSkillEffect(memory, ability, player_creature_id, ai_creature_id, ability_data);
    }
    else
    {
        g_core.battleMode.combatLog[1][0] = '\0';
        uint8_t num_abilities = 0;
        for (uint8_t i = 0; i < MAX_ABILITIES; ++i)
            if (g_core.creatures.attacks[ai_creature_id][i] != NO_ABILITY)
                num_abilities++;
        if (num_abilities == 0) return false;
        uint8_t idx = hardware.GetRandom_uint8_t(0, num_abilities);
        ability = g_core.creatures.attacks[ai_creature_id][idx];
        SkillData ability_data = Flash_GetSkillData(memory, ability);
        Flash_GetSkillEffect(memory, ability, ai_creature_id, player_creature_id, ability_data);
    }

    //  set move animation cache
    g_core.battleMode.moveID.AbilityId = ability;
    g_core.battleMode.moveType = SKILL;
    return true;
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".core")
bool CastSpell(HardwareInterface hardware, MemoryInterface memory, SpellId spellID, EntityId partyID, EntityId enemyID)
{
    // DEBUG("spell data %s id: %d  partyid: %d enemyid: %d", g_gameFlash.text.names.spells[spellID], spellID, partyID, enemyID);
    SpellData spellData = Flash_GetSpellData(memory, spellID);;
    return Flash_GetSpellEffect(hardware, memory, spellID, partyID, enemyID, spellData);
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".core")
bool UseItem(MemoryInterface memory, EntityId item_id, EntityId e_id)
{
    if (item_id == NO_ENTITY) return false;
    ItemTypes itemType = GetItemType(item_id);
    ItemData itemData = Flash_GetItemData(memory, item_id);
    return Flash_GetItemEffect(memory, itemType, item_id, e_id, itemData);
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
SET_MEMORY(".map")
bool InteractObject(MemoryInterface memory, HardwareInterface hardware, EntityId object_e_id, EntityId e_id)
{
    if (object_e_id == NO_ENTITY) return false;
    Object object_type = GetObjectType(object_e_id);
    ObjectData object_data = Flash_GetObjectData(memory, object_type);
    return Flash_GetObjectEffect(memory, hardware, object_type, object_e_id, e_id, object_data);
}
