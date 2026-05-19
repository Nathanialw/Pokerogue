from python.data import db_manager
from python.config import constants


def creatures_skills(entity):
    filename = f"{constants.INC_FOLDER}/data_{entity}_skills.inc"
    creatures_skills_arr = db_manager.get_creatures_skills()

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} data structs\n")
        f.write(f"// Database contains {len(creatures_skills_arr)} total used {entity}s\n")
        f.write(f"// Database contains {len(creatures_skills_arr[0])} total used skills\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s learnable skill data\n")
        for skills in creatures_skills_arr:
            f.write(f"{{ ")
            v = ''
            i = 0
            for s in skills:
                v = v + str(s)
                i += 1
                if i % 8 == 0:
                    f.write(f"0b{v}, ")
                    v = ''
            f.write(f"0b{v}0 }},\n")

        f.write("\n")
        f.write(f"//ALL_CREATURES_COUNT = {len(creatures_skills_arr)};\n")
        f.write(f"//ALL_SKILLS_COUNT = {len(creatures_skills_arr[0])};\n")


def creatures_level_up_skills(entity):
    filename = f"{constants.INC_FOLDER}/data_{entity}_level_up_skills.inc"
    names = db_manager.get_creatures_level_up_skills()

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} data structs\n")
        f.write(f"// Database contains {len(names)} total used {entity}s\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s data\n")
        # for b in names:
        for b in names:
            f.write(f"{{ .c = {{ ")
            for i in range(16):
                if i < len(b):
                    f.write(f"{{ .skillID = {b[i][0]}, .level = {b[i][1]}, }}, ")
                else:
                    f.write(f"{{ .skillID = NO_ABILITY, .level = 0, }}, ")
            f.write(f"}} }},\n")

        f.write("\n")
        f.write(f"//ALL_NAMES_COUNT = {len(names)};\n")


def spells(entity):
    filename = f"{constants.INC_FOLDER}/data_{entity}s.inc"
    names = db_manager.get_spells_data()

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} data structs\n")
        f.write(f"// Database contains {len(names)} total {entity}s\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s data\n")
        desc_vars = []
        for i, (power, mana_cost, type_0, pp) in enumerate(names):
            # Clean the types for C string
            f.write("{" + f" .power = {power}, .type = {type_0}, .pp = {pp} " + "},\n")

        f.write("\n")
        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")


def items(entity):
    filename = f"{constants.INC_FOLDER}/data_{entity}s.inc"
    names = db_manager.get_items_data()

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} data structs\n")
        f.write(f"// Database contains {len(names)} total {entity}s\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s data\n")
        desc_vars = []
        for i, (power, item_level, item_type) in enumerate(names):
            # Clean the types for C string
            f.write("{" + f" .power = {power}, .level = {item_level}, .type = {item_type} " + "},\n")

        f.write("\n")
        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")


def abilities(entity):
    filename = f"{constants.INC_FOLDER}/data_{entity}s.inc"
    names = db_manager.get_abilities_data()

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} data structs\n")
        f.write(f"// Database contains {len(names)} total {entity}s\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s data\n")
        desc_vars = []
        for i, (power, mana_cost, type_0) in enumerate(names):
            # Clean the types for C string
            f.write("{" + f" .power = {power}, .manaCost = {mana_cost}, .type = {type_0} " + "},\n")

        f.write("\n")
        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")


def objects(entity):
    filename = f"{constants.INC_FOLDER}/data_{entity}s.inc"
    names = db_manager.get_objects_data()

    with open(filename, 'w', encoding='utf-8') as f:
        f.write(f"// Generated {entity} data structs\n")
        f.write(f"// Database contains {len(names)} total {entity}s\n\n")

        # Write individual constants
        f.write(f"// Individual {entity}s data\n")
        desc_vars = []
        for i, (power, object_type) in enumerate(names):
            # Clean the types for C string
            f.write("{" + f" .power = {power}, .type = {object_type} " + "},\n")

        f.write("\n")
        f.write(f"//ALL_NAMES_COUNT = {len(desc_vars)};\n")
