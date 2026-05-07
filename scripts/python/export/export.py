from IPython.utils import capture
from numpy.core.defchararray import capitalize

from python.data import db_manager
from python.config import constants


def desc_to_c_array(entity):
    """Export the database contents to a C array file"""
    success, descriptions = db_manager.get_all_descriptions(entity)

    RED = '\033[91m'
    RESET = '\033[0m'

    if len(descriptions) < 255:
        print(f"\n{RED}❌")
        print(f"\n{RED}Export cancelled due to database integrity issues.{RESET}")
        print(f"{RED}Please fix the errors listed above and try again.{RESET}")

    # Group by creature name to handle multiple models
    creature_map = {}
    for creature, desc, model in descriptions:
        if creature not in creature_map:
            creature_map[creature] = []
        creature_map[creature].append((desc, model))

    filename = f"{constants.INC_FOLDER}/text_desc_{entity}s.inc"
    with open(filename, 'w', encoding='utf-8') as f:
        f.write("// Generated creature descriptions\n")
        f.write(f"// Database contains {len(descriptions)} total descriptions\n\n")

        # Write individual constants
        f.write("// Individual description strings\n")
        desc_vars = []
        for i, (creature, desc, model) in enumerate(descriptions):
            var_name = f"DESC_{creature}_{i}"
            # Clean the description for C string
            clean_desc = desc.replace('\\', '\\\\').replace('"', '\\"')
            f.write(f'"{clean_desc}\\0",\n')
            desc_vars.append((creature, var_name, model))
        f.write(f"\" ------ \\0\",\n")
        f.write("\n")

        f.write(f"//ALL_DESCRIPTIONS_COUNT = {len(desc_vars)};\n")

    # db_manager.cleanup_descriptions(dry_run=False)
    print(f"📄 Exported {len(descriptions)} descriptions to {filename}")


def name_to_c_array(entity):
    """Export the database contents to a C array file"""
    filename = f"{constants.INC_FOLDER}/text_names_{entity}s.inc"

    names = db_manager.get_names(entity)

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} name strings\n")
        f.write(f"// Database contains {len(names)} total {entity}\n\n")

        # Write individual constants
        f.write(f"// Individual {entity} strings\n")
        desc_vars = []
        for i, (formatted) in enumerate(names):
            # Clean the types for C string
            f.write(f"\"{formatted:.15}\\0\",\n")
        f.write(f"\" ------ \\0\",\n")
        f.write("\n")

        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")

    print(f"📄 Exported {len(names)} {entity} names to {filename}")
    return len(names)


def funcs_to_c_array(entity, prepend):
    """Export the database contents to a C array file"""
    filename = f"{constants.INC_FOLDER}/funcs_{entity}s.inc"

    names = db_manager.get_funcs(entity)

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} name strings\n")
        f.write(f"// Database contains {len(names)} total entries\n\n")

        # Write individual constants
        f.write("// Individual items strings\n")
        desc_vars = []
        for i, (function_name) in enumerate(names):
            # Clean the types for C string
            f.write(f"{prepend}{function_name},\n")

        f.write("\n")

        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")

    print(f"📄 Exported {len(names)} item names to {filename}")


def func_c_headers(entity, prepend, return_type, params):
    """Export the database contents to a C array file"""

    filename = f"{constants.INC_FOLDER}/decl_{entity}s.inc"
    names = db_manager.get_funcs(entity)

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} function headers\n")
        f.write(f"// Database contains {len(names)} total {entity}s\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s strings\n")
        desc_vars = []
        for i, (function_name) in enumerate(names):
            # Clean the types for C string
            f.write(f"{return_type} {prepend}{function_name}({params});\n")

        f.write("\n")

        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")

    print(f"📄 Exported {len(names)} spell headers to {filename}")


def funcs_to_c_animations_array(entity, p):
    """Export the database contents to a C array file"""
    filename = f"{constants.INC_FOLDER}/funcs_animation_{entity}s_{p}.inc"

    names = db_manager.get_funcs(entity)

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} name strings\n")
        f.write(f"// Database contains {len(names)} total entries\n\n")

        # Write individual constants
        f.write("// Individual items strings\n")
        desc_vars = []
        for i, (function_name) in enumerate(names):
            # Clean the types for C string
            f.write(f"{capitalize(entity)}{capitalize(p)}{function_name},\n")

        f.write("\n")

        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")

    print(f"📄 Exported {len(names)} item names to {filename}")


def func_c_animation_headers(entity, p, return_type, params):
    """Export the database contents to a C array file"""

    filename = f"{constants.INC_FOLDER}/decl_animation_{entity}s_{p}.inc"
    names = db_manager.get_funcs(entity)

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} animation function headers\n")
        f.write(f"// Database contains {len(names)} total {entity}s\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s strings\n")
        desc_vars = []
        for i, (function_name) in enumerate(names):
            # Clean the types for C string
            f.write(f"{return_type} {capitalize(entity)}{capitalize(p)}{function_name}({params});\n")

        f.write("\n")

        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")

    print(f"📄 Exported {len(names)} spell headers to {filename}")


def export_map_sprites_char(entity):
    """Export the database contents to a C array file"""
    filename = f"{constants.INC_FOLDER}/map_sprites_{entity}s.inc"
    sprites = db_manager.get_map_sprites(entity)
    count = len(sprites)

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} sprites and color indexes\n")
        f.write(f"// Database contains {count} total {entity} sprites\n\n")
        f.write("// Individual sprites\n")

        desc_vars = []
        for i, (sprite, color) in enumerate(sprites):
            # Clean the types for C string
            f.write(f"{{ .glyph_index = {sprite}, .fg = {color} }},\n")

        f.write("\n")
        f.write(f"//Sprites count = {count};\n")

    print(f"📄 Exported {count} {entity} sprites to {filename}")


def export_map_sprites(entity):
    """Export the database contents to a C array file"""
    filename = f"{constants.INC_FOLDER}/sprite_map_{entity}s.inc"
    sprites = []
    count = len(sprites)

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} sprites and color indexes\n")
        f.write(f"// Database contains {count} total {entity} sprites\n\n")
        f.write("// Individual sprites\n")

        desc_vars = []
        for i, (sprite, color) in enumerate(sprites):
            #
            f"{{ 0xFFFF 0xFFFF 0xFFFF}}"

        f.write("\n")
        f.write(f"//Sprites count = {count};\n")

    print(f"📄 Exported {count} {entity} sprites to {filename}")


def export_constants(byte_counts, counts):
    """Export the count constants to a C array file"""
    filename = f"{constants.INC_FOLDER}/data_constants.inc"
    with open(filename, 'w', encoding='utf-8') as f:
        f.write("#pragma once\n\n")
        f.write("// Generated constant values\n\n")

        f.write(f"#define SPRITE_CREATURE_FRONT_BYTES {byte_counts[0]}\n")
        f.write(f"#define SPRITE_CREATURE_BACK_BYTES {byte_counts[1]}\n")
        f.write(f"#define SPRITE_SKILL_BYTES {byte_counts[2]}\n")
        f.write(f"#define SPRITE_SPELL_BYTES {byte_counts[3]}\n")
        f.write(f"#define SPRITE_ITEM_BYTES {byte_counts[4]}\n")
        f.write(f"#define SPRITE_OBJECT_BYTES {byte_counts[5]}\n")
        f.write(f"#define SPRITE_TRAINER_BYTES {byte_counts[6]}\n")
        f.write(f"\n")

        f.write(f"#define CREATURE_COUNT {counts[0]}\n")
        f.write(f"#define ABILITY_COUNT {counts[1]}\n")
        f.write(f"#define SPELL_COUNT {counts[2]}\n")
        f.write(f"#define ITEM_COUNT {counts[3]}\n")
        f.write(f"#define OBJECT_COUNT {counts[4]}\n")
        f.write(f"#define TRAINER_COUNT {counts[5]}\n")
        f.write(f"\n")

        f.write(f"#define CREATURE_TEXT_ARRAY_SIZE {counts[0] + 1}\n")
        f.write(f"#define ABILITY_TEXT_ARRAY_SIZE {counts[1] + 1}\n")
        f.write(f"#define SPELL_TEXT_ARRAY_SIZE {counts[2] + 1}\n")
        f.write(f"#define ITEM_TEXT_ARRAY_SIZE {counts[3] + 1}\n")
        f.write(f"#define OBJECT_TEXT_ARRAY_SIZE {counts[4] + 1}\n")
        f.write(f"#define TRAINER_TEXT_ARRAY_SIZE {counts[5] + 1}\n")
        f.write(f"\n")

        f.write(f"#define NO_CREATURE {counts[0]}\n")
        f.write(f"#define NO_ABILITY {counts[1]}\n")
        f.write(f"#define NO_SPELL {counts[2]}\n")
        f.write(f"#define NO_ITEM {counts[3]}\n")
        f.write(f"#define NO_OBJECT {counts[4]}\n")
        f.write(f"#define NO_TRAINER {counts[5]}\n")

        f.write(f"\n// All values aligned to 2 bytes\n")
    print(f"📄 Exported to {filename}")
    print(f"#define SPRITE_CREATURE_FRONT_BYTES {byte_counts[0]}")
    print(f"#define SPRITE_CREATURE_BACK_BYTES {byte_counts[1]}")
    print(f"#define SPRITE_SKILL_BYTES {byte_counts[2]}")
    print(f"#define SPRITE_SPELL_BYTES {byte_counts[3]}")
    print(f"#define SPRITE_ITEM_BYTES {byte_counts[4]}")
    print(f"#define SPRITE_OBJECT_BYTES {byte_counts[5]}")
    print(f"#define SPRITE_TRAINER_BYTES {byte_counts[6]}")

    print(f"#define CREATURE_COUNT {counts[0]}\n")
    print(f"#define ABILITY_COUNT {counts[1]}\n")
    print(f"#define SPELL_COUNT {counts[2]}\n")
    print(f"#define ITEM_COUNT {counts[3]}\n")
    print(f"#define OBJECT_COUNT {counts[4]}\n")
    print(f"#define TRAINER_COUNT {counts[5]}\n")
