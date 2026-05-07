from python.config import constants
from . import export_creatures
from . import export_core_data
from . import export_battlers
from . import export
from . import export_structs
from . import export_enums
from . import export_img_prompts


def run():
    # CORE DATA
    bytes_count = []
    counts = []

    # update enums
    export_enums.enum("creature")
    export_enums.enum("skill")
    export_enums.enum("spell")
    export_enums.enum("item")
    export_enums.enum("object")
    export_enums.enum("trainer")

    # image generation strings
    export_img_prompts.generate_data_from_db('creature')
    export_img_prompts.generate_data_from_db('skill')
    export_img_prompts.generate_data_from_db('spell')
    export_img_prompts.generate_data_from_db('item')
    export_img_prompts.generate_data_from_db('object')
    export_img_prompts.generate_data_from_db('trainer')

    # type strings
    export_core_data.write_types_inc()

    # CREATURES
    export_structs.creatures_skills("creature")
    export_structs.creatures_level_up_skills("creature")
    # creature name strings
    counts.append(export.name_to_c_array("creature"))
    # creature description strings
    export.desc_to_c_array("creature")
    # creature map sprites
    export.export_map_sprites_char("creature")
    export.export_map_sprites("creature")
    # creature type data
    export_creatures.export_types_to_c_array()
    # creature battlers front
    bytes_count.append(export_battlers.export_image_data("creature", "front"))
    # creature battlers back
    bytes_count.append(export_battlers.export_image_data("creature", "back"))
    # TODO Map sprites

    # ABILITIES
    # ability struct data
    export_structs.abilities("skill")
    # ability functions headers
    export.func_c_headers("skill", "Skill", "bool", "EntityId attackerID, EntityId defenderID, SkillData skillData")
    # ability functions
    export.funcs_to_c_array("skill", "Skill")
    # ability animation functions
    export.funcs_to_c_animations_array("skill", "attack")
    export.funcs_to_c_animations_array("skill", "struck")
    # ability animation functions header
    export.func_c_animation_headers("skill", "attack", "bool", "bool onAttacker")
    export.func_c_animation_headers("skill", "struck", "bool", "bool onAttacker")
    # ability name strings
    counts.append(export.name_to_c_array("skill"))
    # ability description strings
    export.desc_to_c_array("skill")
    # ability icons
    # TODO: export images
    bytes_count.append(export_battlers.export_image_data("skill"))
    # TODO Map sprites

    # SPELLS
    # spell struct data
    export_structs.spells("spell")
    # spell functions headers
    export.func_c_headers("spell", "Cast", "bool", "EntityId partyID, EntityId enemyID, SpellData spellData")
    # spell functions
    export.funcs_to_c_array("spell", "Cast")
    # spell animation functions
    export.funcs_to_c_animations_array("spell", "attack")
    export.funcs_to_c_animations_array("spell", "struck")
    # spell animation functions header
    export.func_c_animation_headers("spell", "attack", "bool", "bool onAttacker")
    export.func_c_animation_headers("spell", "struck", "bool", "bool onAttacker")
    # spell name strings
    counts.append(export.name_to_c_array("spell"))
    # spell description strings
    export.desc_to_c_array("spell")
    # spell icons
    # TODO: export images
    bytes_count.append(export_battlers.export_image_data("spell"))
    # TODO Map sprites

    # ITEMS
    # item struct data
    export_structs.items("item")
    # item map sprites header
    export.export_map_sprites_char("item")
    export.export_map_sprites("item")
    # item functions header
    export.func_c_headers("item", "Use", "bool", "EntityId item_id, EntityId e_id, ItemData itemData")
    # item functions
    export.funcs_to_c_array("item", "Use")
    # item animation functions
    export.funcs_to_c_animations_array("item", "attack")
    export.funcs_to_c_animations_array("item", "struck")
    # item animation functions header
    export.func_c_animation_headers("item", "attack", "bool", "bool onAttacker")
    export.func_c_animation_headers("item", "struck", "bool", "bool onAttacker")
    # item name strings
    counts.append(export.name_to_c_array("item"))
    # item description strings
    export.desc_to_c_array("item")
    # item icons
    # TODO: export images
    bytes_count.append(export_battlers.export_image_data("item"))
    # TODO Map sprites

    # OBJECTS
    export_structs.objects("object")
    # object map sprites header
    export.export_map_sprites_char("object")
    export.export_map_sprites("object")
    # object functions header
    export.func_c_headers("object", "Interact", "bool", "EntityId object_id, EntityId e_id, ObjectData objectData")
    # object functions
    export.funcs_to_c_array("object", "Interact")
    # object name strings
    counts.append(export.name_to_c_array("object"))
    # object description strings
    export.desc_to_c_array("object")
    # object icons
    # TODO: export images
    bytes_count.append(export_battlers.export_image_data("object"))
    # TODO Map sprites

    # TRAINERS
    export_structs.creatures_skills("trainer")
    export_structs.creatures_level_up_skills("trainer")
    # trainer name strings
    counts.append(export.name_to_c_array("trainer"))
    # trainer description strings
    export.desc_to_c_array("trainer")
    # trainer map sprites
    export.export_map_sprites_char("trainer")
    export.export_map_sprites("trainer")
    bytes_count.append(export_battlers.export_image_data("trainer"))
    # TODO Map sprites

    #TILES
    export.export_map_sprites("tile")

    export.export_constants(bytes_count, counts)


run()
