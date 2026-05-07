//
// Created by nathanial on 4/13/26.
//

#include "battles.h"

#include "entities.h"
#include "lib_debugging.h"
#include "memory_ram.h"
#include "memory_rom.h"



/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void GetSkills(EntityId id, Type type)
{
    uint8_t i = 0;
    while (i < MAX_ABILITIES)
    {
        g_run.creatures.attacks[id][i] = NO_ABILITY;
        i++;
    }

    for (uint8_t i = 0; i < MAX_ABILITIES; ++i)
        g_run.creatures.attacks[id][i] = g_gameFlash.gameData.levelUpSkills[type][i].skillID;
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool UseSkill(bool player)
{
    EntityId player_creature_id = g_run.battleMode.playerMonsterID;
    EntityId ai_creature_id = g_run.battleMode.enemyMonsterID;
    Ability ability;

    if (player)
    {
        g_run.battleMode.combatLog[0][0] = '\0';
        ability = g_run.creatures.attacks[player_creature_id][g_run.menu.sel->y];
        SkillData ability_data = g_gameFlash.gameData.abilityData[ability];
        g_gameFlash.funcs.abilityFunctions[ability](player_creature_id, ai_creature_id, ability_data);
    }
    else
    {
        g_run.battleMode.combatLog[1][0] = '\0';
        uint8_t num_abilities = 0;
        for (uint8_t i = 0; i < MAX_ABILITIES; ++i)
            if (g_run.creatures.attacks[ai_creature_id][i] != NO_ABILITY)
                num_abilities++;
        if (num_abilities == 0) return false;
        uint8_t idx = 0 + rand() % num_abilities;
        ability = g_run.creatures.attacks[ai_creature_id][idx];
        SkillData ability_data = g_gameFlash.gameData.abilityData[ability];
        g_gameFlash.funcs.abilityFunctions[ability](ai_creature_id, player_creature_id, ability_data);
    }

    //  set move animation cache
    g_run.battleMode.moveID.AbilityId = ability;
    g_run.battleMode.moveType = SKILL;
    return true;
}



/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool CastSpell(SpellId spellID, EntityId partyID, EntityId enemyID)
{
    DEBUG("spell data %s id: %d  partyid: %d enemyid: %d", g_gameFlash.text.names.spells[spellID], spellID, partyID, enemyID);
    SpellData spellData = g_gameFlash.gameData.spellData[spellID];
    return g_gameFlash.funcs.spellFunctions[spellID](partyID, enemyID, spellData);
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool UseItem(EntityId item_id, EntityId e_id)
{
    if (item_id == NO_ENTITY) return false;
    ItemTypes itemType = GetItemType(item_id);
    ItemData itemData = g_gameFlash.gameData.itemData[itemType];
    return g_gameFlash.funcs.itemFunctions[itemType](item_id, e_id, itemData);
}


/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
bool InteractObject(EntityId object_e_id, EntityId e_id)
{
    DEBUG("interacting with object %d", object_e_id);
    if (object_e_id == NO_ENTITY) return false;
    Object object_type = GetObjectType(object_e_id);
    ObjectData object_data = g_gameFlash.gameData.objectData[object_type];
    DEBUG("%d object type: %d", object_type, object_data.type);
    return g_gameFlash.funcs.objectFunctions[object_type](object_e_id, e_id, object_data);
}
