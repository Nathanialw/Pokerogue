import sqlite3
import datetime
from collections import namedtuple

from python.config import constants

DB_FILE = constants.DB_FILE


def init_database():
    """Create the database and tables if they don't exist"""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Descriptions table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS creature_descriptions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,
            description TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, used, description, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS trainer_descriptions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,
            description TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, used, description, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS item_descriptions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,
            description TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, used, description, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS spell_descriptions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,
            description TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, used, description, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS skill_descriptions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,
            description TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, used, description, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS object_descriptions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,
            description TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, used, description, model_name)
        )
    ''')

    # Image prompts table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS creature_img_prompts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            prompt TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, prompt, model_name)
        )
    ''')

    # Image prompts table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS trainer_img_prompts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            prompt TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, prompt, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS item_img_prompts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            prompt TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, prompt, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS spell_img_prompts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            prompt TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, prompt, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS skill_img_prompts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            prompt TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, prompt, model_name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS object_img_prompts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            prompt TEXT NOT NULL,
            word_count INTEGER,
            char_count INTEGER,
            attempt_number INTEGER,
            timestamp TEXT,
            validation_matches INTEGER,
            model_name TEXT,
            prompt_version TEXT,
            temperature REAL,
            UNIQUE(name, prompt, model_name)
        )
    ''')

    # Add indexes
    cursor.execute('''
        CREATE INDEX IF NOT EXISTS idx_creature_model 
        ON creature_descriptions(name, model_name)
    ''')

    cursor.execute('''
        CREATE INDEX IF NOT EXISTS idx_img_creature_model 
        ON creature_img_prompts(name, model_name)
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS creatures (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            formatted TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,            
            type_0 TEXT,
            type_1 TEXT,
            sprite_idx TEXT,
            sprite_color_idx TEXT
            notes TEXT
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS trainers (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            formatted TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,
            sprite_idx TEXT,
            sprite_color_idx TEXT            
            notes TEXT
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS skills (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            used INTEGER NOT NULL DEFAULT 0,            
            formatted TEXT,
            function_name TEXT,
            power INTEGER NOT NULL DEFAULT 5,          
            mana_cost INTEGER NOT NULL DEFAULT 5,
            accuracy INTEGER DEFAULT 100,            
            notes TEXT
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS spells (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            used INTEGER NOT NULL DEFAULT 0,            
            name TEXT NOT NULL,
            formatted TEXT,
            function_name TEXT,
            item_level INTEGER NOT NULL DEFAULT 0,            
            power INTEGER NOT NULL DEFAULT 5,
            power_points INTEGER NOT NULL DEFAULT 5,
            duration INTEGER NOT NULL DEFAULT 5,
            item_type TEXT,
            accuracy INTEGER DEFAULT 100,
            notes TEXT
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS items (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            formatted TEXT,
            function_name TEXT,
            used INTEGER NOT NULL DEFAULT 0,
            power INTEGER NOT NULL DEFAULT 5,
            item_level INTEGER NOT NULL DEFAULT 0,            
            item_type TEXT,
            accuracy INTEGER DEFAULT 100,            
            notes TEXT
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS objects (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            formatted TEXT,
            function_name TEXT,
            used INTEGER NOT NULL DEFAULT 0,
            power INTEGER NOT NULL DEFAULT 5,
            object_type TEXT,
            notes TEXT,
            sprite_idx TEXT,
            sprite_color_idx TEXT
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS creature_skills (
            creature_name TEXT,
            skill_name TEXT,
            learnable BOOLEAN NOT NULL DEFAULT 0,
            PRIMARY KEY (creature_name, skill_name),
            FOREIGN KEY (creature_name) REFERENCES creatures(name),
            FOREIGN KEY (skill_name) REFERENCES skills(name)
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS trainer_spells (
            creature_name TEXT,
            skill_name TEXT,
            learnable BOOLEAN NOT NULL DEFAULT 0,
            PRIMARY KEY (creature_name, skill_name),
            FOREIGN KEY (creature_name) REFERENCES creatures(name),
            FOREIGN KEY (skill_name) REFERENCES skills(name)
        )
    ''')

    cursor.execute('''
        INSERT OR IGNORE INTO  creature_skills (creature_name, skill_name)
        SELECT c.name, s.name
        FROM creatures c, skills s;
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS creature_level_up_skills (
            creature_name TEXT,
            skill_name TEXT,
            level INTEGER NOT NULL DEFAULT 1,
            PRIMARY KEY (creature_name, skill_name),
            FOREIGN KEY (creature_name) REFERENCES creatures(name),
            FOREIGN KEY (skill_name) REFERENCES skills(name)
        )
    ''')

    # I just use this to populate the table, not every creature will learn TACKLE
    # cursor.execute('''
    #         INSERT OR IGNORE INTO  creature_level_up_skills (creature_name, skill_name)
    #         SELECT c.name, 'TACKLE'
    #         FROM creatures c, skills s;
    #     ''')

    conn.commit()
    conn.close()
    print(f"📦 Database initialized: {DB_FILE}")


def save_to_db_description(table, creature_name, description, attempt_num, matches=0,
                           model_name="unknown", prompt_version="default", temperature=0.0):
    """Save a successful description to the database"""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    word_count = len(description.split())
    char_count = len(description)
    timestamp = datetime.datetime.now().isoformat()

    try:
        cursor.execute(f'''
            INSERT INTO {table} 
            (name, description, word_count, char_count, 
             attempt_number, timestamp, validation_matches, 
             model_name, prompt_version, temperature)
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ''', (creature_name, description, word_count, char_count,
              attempt_num, timestamp, matches,
              model_name, prompt_version, temperature))

        conn.commit()
        print(f"  💾 Saved to DB: {creature_name} (using {model_name})")
        return True
    except sqlite3.IntegrityError:
        print(f"  ⏩ Already exists in DB: {creature_name}")
        return False
    finally:
        conn.close()


def save_to_db_img_prompt(table, creature_name, description, attempt_num, matches=0,
                          model_name="unknown", prompt_version="default", temperature=0.0):
    """Save a successful description to the database"""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    word_count = len(description.split())
    char_count = len(description)
    timestamp = datetime.datetime.now().isoformat()

    try:
        cursor.execute(f'''
            INSERT INTO {table} 
            (creature_name, description, word_count, char_count, 
             attempt_number, timestamp, validation_matches, 
             model_name, prompt_version, temperature)
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ''', (creature_name, description, word_count, char_count,
              attempt_num, timestamp, matches,
              model_name, prompt_version, temperature))

        conn.commit()
        print(f"  💾 Saved to DB: {creature_name} (using {model_name})")
        return True
    except sqlite3.IntegrityError:
        print(f"  ⏩ Already exists in DB: {creature_name}")
        return False
    finally:
        conn.close()


def save_to_db(table, creature_name, description, attempt_num, matches=0, model_name="unknown", prompt_version="default", temperature=0.0):
    if table == constants.IMG_PROMPT:
        save_to_db_img_prompt(table, creature_name, description, attempt_num, matches, model_name, prompt_version, temperature)
    else:
        save_to_db_description(table, creature_name, description, attempt_num, matches, model_name, prompt_version, temperature)


def get_existing_used(creature_name, table):
    """Return how many descriptions already exist for a creature"""

    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute(
        f'SELECT COUNT(*) FROM "{table}" WHERE name = ? AND used = 1', (creature_name,))

    count = cursor.fetchone()[0]
    conn.close()
    return count


def get_existing_count(name, table, model_name=None):
    """Return how many descriptions already exist for a creature"""

    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    if model_name:
        cursor.execute(
            f'SELECT COUNT(*) FROM "{table}" WHERE name = ? AND model_name = ?',
            (name, model_name)
        )
    else:
        cursor.execute(
            f'SELECT COUNT(*) FROM "{table}" WHERE name = ?',
            (name,)
        )

    count = cursor.fetchone()[0]
    conn.close()
    return count


def needs_generation(creature_name, table, model_name=None, desired_count=1):
    """Check if we need to generate more descriptions for this creature/model"""
    # Determine which table to use based on whether we're generating image prompts
    current = get_existing_count(creature_name, table, model_name)
    used = get_existing_used(creature_name, table)
    return used == 0 and current < desired_count


def get_all_descriptions_for_creature(creature_name, model_name=None):
    """Retrieve all descriptions for a specific creature, useful for curation."""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    if model_name:
        cursor.execute(
            'SELECT creature_name, description, model_name FROM creature_descriptions WHERE name = ? AND model_name = ? ORDER BY LENGTH(description) DESC',
            (creature_name, model_name)
        )
    else:
        cursor.execute(
            'SELECT creature_name, description, model_name FROM creature_descriptions WHERE name = ? ORDER BY LENGTH(description) DESC',
            (creature_name,)
        )

    results = cursor.fetchall()
    conn.close()
    return results


def get_all_descriptions(entity):
    entity_table = f"{entity}s"
    desc_table = f"{entity}_descriptions"

    """Retrieve all saved descriptions.
    Returns: (success, results) where success is boolean indicating if all used entities have descriptions"""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute(f'SELECT COUNT(*) FROM {entity_table} WHERE used = 1')
    used_count = cursor.fetchone()[0]
    print(f"DEBUG: Used {entity_table} count = {used_count}")

    has_errors = False

    # Check for multiple 'used' descriptions
    cursor.execute(f'''
        SELECT cn.name, COUNT(d.id)
        FROM {entity_table} cn
        INNER JOIN {desc_table} d ON cn.name = d.name
        WHERE cn.used = 1 
        AND d.used = 1
        GROUP BY cn.name
        HAVING COUNT(d.id) > 1
    ''')

    duplicates = cursor.fetchall()
    if duplicates:
        has_errors = True
        print("=" * 60)
        print("ERROR: Multiple 'used' descriptions found:")
        for name, count in duplicates:
            print(f"  {name}: {count} used descriptions")
        print("=" * 60)

    # Get all used creatures and their descriptions
    # For creatures with a used=1 description, use that
    # For creatures without, generate placeholder text
    cursor.execute(f'''
        SELECT cn.name
        FROM {entity_table} cn
        WHERE cn.used = 1
        ORDER BY cn.name
    ''')

    used_creatures = cursor.fetchall()

    results = []
    missing_count = 0

    for (creature_name,) in used_creatures:
        # Check if this creature has a used=1 description
        cursor.execute(f'''
            SELECT d.description, d.model_name
            FROM {desc_table} d
            WHERE d.name = ? AND d.used = 1
            ORDER BY LENGTH(d.description) DESC
            LIMIT 1
        ''', (creature_name,))

        used_desc = cursor.fetchone()

        if used_desc:
            # Use the used description
            results.append((creature_name, used_desc[0], used_desc[1]))
        else:
            # No used description found, create placeholder
            missing_count += 1
            placeholder = f"{creature_name} -- PLACEHOLDER TEXT - A mysterious {entity} awaiting discovery."
            results.append((creature_name, placeholder, "PLACEHOLDER"))

            # Print warning for missing descriptions (but don't treat as error)
            print(f"⚠️  WARNING: No 'used' description found for {creature_name}, using placeholder")

    # Also check for creatures that have used descriptions but weren't marked as used in creature_names
    cursor.execute(f'''
        SELECT DISTINCT d.name
        FROM {desc_table} d
        WHERE d.used = 1
        AND d.name NOT IN (
            SELECT name FROM {entity_table} WHERE used = 1
        )
    ''')

    orphaned_descriptions = cursor.fetchall()
    # if orphaned_descriptions:
    #     has_errors = True
    #     print("=" * 60)
    #     print("ERROR: Descriptions marked as 'used' for creatures not marked as 'used' in creature_names:")
    #     for (name,) in orphaned_descriptions:
    #         print(f"  {name}")
    #     print("=" * 60)

    print(f"DEBUG: Results returned = {len(results)}")
    print(f"DEBUG: Used descriptions found = {len(results) - missing_count}")
    print(f"DEBUG: Placeholders added = {missing_count}")

    if missing_count > 0:
        print(f"⚠️  WARNING: {missing_count} creatures are using placeholder text")
        print("   To fix, run: UPDATE descriptions SET used = 1 WHERE name = 'NAME' AND id = ID;")

    conn.close()
    return not has_errors, results


def purge_unused_descriptions(dry_run=True):
    """Delete descriptions with used=0 when a used=1 description exists for the same creature.

    Args:
        dry_run (bool): If True, only show what would be deleted without actually deleting.
                        If False, perform the deletion.

    Returns:
        tuple: (deleted_count, creatures_affected) where deleted_count is total rows deleted,
               and creatures_affected is list of creatures that had unused descriptions purged.
    """
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Find all creatures that have at least one used=1 description
    cursor.execute('''
        SELECT DISTINCT creature_name
        FROM creature_descriptions
        WHERE used = 1
    ''')

    creatures_with_used = [row[0] for row in cursor.fetchall()]

    if not creatures_with_used:
        print("No creatures with used=1 descriptions found.")
        conn.close()
        return 0, []

    # For each creature, find descriptions with used=0 to delete
    deleted_count = 0
    creatures_affected = []

    for creature_name in creatures_with_used:
        # Check if there are unused descriptions for this creature
        cursor.execute('''
            SELECT id, description, model_name
            FROM creature_descriptions
            WHERE name = ? AND used = 0
        ''', (creature_name,))

        unused_descs = cursor.fetchall()

        if unused_descs:
            creatures_affected.append(creature_name)

            if dry_run:
                print(f"\n📋 DRY RUN: Would delete {len(unused_descs)} unused description(s) for {creature_name}:")
                for desc_id, description, model in unused_descs:
                    preview = description[:80] + "..." if len(description) > 80 else description
                    print(f"   - ID {desc_id} ({model}): {preview}")
            else:
                # Delete the unused descriptions
                cursor.execute('''
                    DELETE FROM creature_descriptions
                    WHERE name = ? AND used = 0
                ''', (creature_name,))

                deleted_count += cursor.rowcount
                print(f"🗑️  Deleted {cursor.rowcount} unused description(s) for {creature_name}")

    if dry_run and creatures_affected:
        print(f"\n📋 DRY RUN SUMMARY: Would delete unused descriptions for {len(creatures_affected)} creatures")
        print(f"   Total unused descriptions: {deleted_count}")
    elif not dry_run and deleted_count > 0:
        print(f"\n✅ Deleted {deleted_count} unused description(s) across {len(creatures_affected)} creatures")
        conn.commit()
    elif not dry_run and deleted_count == 0:
        print("No unused descriptions found to delete.")

    conn.close()
    return deleted_count, creatures_affected


def purge_all_unused_descriptions(confirm=False):
    """Delete ALL descriptions with used=0 regardless of whether a used=1 exists.

    Args:
        confirm (bool): If True, requires explicit confirmation before deleting.

    Returns:
        int: Number of rows deleted.
    """
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Count how many would be deleted
    cursor.execute('SELECT COUNT(*) FROM creature_descriptions WHERE used = 0')
    count = cursor.fetchone()[0]

    if count == 0:
        print("No unused descriptions found.")
        conn.close()
        return 0

    if confirm:
        print(f"⚠️  WARNING: This will delete {count} unused description(s).")
        response = input("Are you sure? Type 'yes' to confirm: ")
        if response.lower() != 'yes':
            print("Operation cancelled.")
            conn.close()
            return 0

    # Delete all unused descriptions
    cursor.execute('DELETE FROM creature_descriptions WHERE used = 0')
    deleted_count = cursor.rowcount
    conn.commit()

    print(f"✅ Deleted {deleted_count} unused description(s)")
    conn.close()
    return deleted_count


def cleanup_descriptions(dry_run=True, purge_all=False):
    """Main cleanup function that purges unused descriptions.

    Args:
        dry_run (bool): If True, preview changes without committing.
        purge_all (bool): If True, delete ALL unused descriptions.
                         If False, only delete when a used=1 exists.
    """
    if purge_all:
        print("=" * 60)
        print("PURGING ALL UNUSED DESCRIPTIONS")
        print("=" * 60)
        return purge_all_unused_descriptions(confirm=not dry_run)
    else:
        print("=" * 60)
        print("PURGING UNUSED DESCRIPTIONS (where used=1 exists)")
        print("=" * 60)
        return purge_unused_descriptions(dry_run=dry_run)


CreatureType = namedtuple('CreatureType', ['formatted', 'type_0', 'type_1'])
CreatureData = namedtuple('CreatureData', ['type_0', 'type_1'])
SpellData = namedtuple('SpellData', ['power', 'mana_cost', 'type_0', 'power_points'])
SkillData = namedtuple('AbilityData', ['power', 'mana_cost', 'type_0'])
ItemData = namedtuple('ItemData', ['power', 'item_level', 'item_type'])
ObjectData = namedtuple('ObjectData', ['power', 'object_type'])
MapSpriteData = namedtuple('MapSpriteData', ['sprite_idx', 'sprite_color_idx'])


def get_creature_types():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT enum, type FROM enum_types')
    # Swap: make type_string the key, enum the value
    type_to_enum = {type_str: enum for enum, type_str in cursor.fetchall()}

    cursor.execute('SELECT formatted, type_0, type_1 FROM creatures WHERE used = 1 ORDER BY name ASC')

    formatted_results = [
        CreatureType(formatted, type_to_enum.get(type_0, 0), type_to_enum.get(type_1, 0))
        for formatted, type_0, type_1 in cursor.fetchall()
    ]

    conn.close()
    return formatted_results


def get_creatures_data():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT type_0, type_1 FROM creatures WHERE used = 1 ORDER BY name ASC')

    formatted_results = [
        CreatureData(type_0, type_1)
        for type_0, type_1 in cursor.fetchall()
    ]

    conn.close()
    return formatted_results


def get_creatures_skills():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT name FROM creatures WHERE used = 1 ORDER BY name ASC')
    used_creatures = []
    for name in cursor.fetchall():
        used_creatures.append(name[0])

    cursor.execute('SELECT name FROM skills WHERE used = 1 ORDER BY name ASC')
    used_skills = []
    for name in cursor.fetchall():
        used_skills.append(name[0])

    formatted_results = []
    for name in used_creatures:
        valid_skills = []
        creature_name = "'" + name + "'"
        for skill in used_skills:
            skill_name = "'" + skill + "'"
            cursor.execute(f"SELECT learnable FROM creature_skills WHERE creature_name = {creature_name} AND skill_name = {skill_name}")
            b = cursor.fetchall()[0][0]
            valid_skills.append(b)
        formatted_results.append(valid_skills)

    conn.close()
    return formatted_results


def get_creatures_level_up_skills():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT name FROM creatures WHERE used = 1 ORDER BY name ASC')
    used_creatures = []
    for name in cursor.fetchall():
        used_creatures.append(name[0])

    formatted_results = []
    for name in used_creatures:
        creature_name = "'" + name + "'"
        cursor.execute(f'SELECT skill_name, level FROM creature_level_up_skills WHERE creature_name = {creature_name} ORDER BY level ASC')
        s = cursor.fetchall()
        formatted_results.append(s)

    conn.close()
    return formatted_results


def get_names(table):
    formatted_results = []  # Initialize the list
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Use a placeholder for the value; table name must be validated separately
    cursor.execute(f"SELECT formatted FROM {table}s WHERE used = ? ORDER BY name ASC", (1,))

    for row in cursor.fetchall():
        formatted_results.append(row[0])  # Extract the first column from the tuple

    conn.close()
    return formatted_results


def get_enums(table):
    formatted_results = []  # Initialize the list
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Use a placeholder for the value; table name must be validated separately
    cursor.execute(f"SELECT name FROM {table}s WHERE used = ? ORDER BY name ASC", (1,))

    for row in cursor.fetchall():
        formatted_results.append(row[0])  # Extract the first column from the tuple

    conn.close()
    return formatted_results


def get_folders(table):
    formatted_results = []  # Initialize the list
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Use a placeholder for the value; table name must be validated separately
    cursor.execute(f"SELECT name FROM {table}s WHERE used = ? ORDER BY name ASC", (1,))

    for row in cursor.fetchall():
        formatted_results.append(row[0])  # Extract the first column from the tuple

    conn.close()
    return formatted_results


def get_funcs(table):
    formatted_results = []  # Initialize the list
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Use a placeholder for the value; table name must be validated separately
    cursor.execute(f"SELECT function_name FROM {table}s WHERE used = ? ORDER BY name ASC", (1,))

    for row in cursor.fetchall():
        formatted_results.append(row[0])  # Extract the first column from the tuple

    conn.close()
    return formatted_results


def get_map_sprites(table):
    formatted_results = []  # Initialize the list
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # Use a placeholder for the value; table name must be validated separately
    cursor.execute(f"SELECT sprite_idx, sprite_color_idx FROM {table}s WHERE used = ? ORDER BY name ASC", (1,))

    formatted_results = [
        MapSpriteData(sprite_idx, sprite_color_idx)
        for sprite_idx, sprite_color_idx in cursor.fetchall()
    ]

    for row in cursor.fetchall():
        formatted_results.append(row[0])  # Extract the first column from the tuple

    conn.close()
    return formatted_results


def get_spells_data():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT enum, type FROM enum_types')
    # Swap: make type_string the key, enum the value
    type_to_enum = {type_str: enum for enum, type_str in cursor.fetchall()}

    cursor.execute('SELECT power, mana_cost, type, power_points FROM spells WHERE used = 1 ORDER BY name ASC')

    formatted_results = [
        SpellData(power, mana_cost, type_0, power_points)
        for power, mana_cost, type_0, power_points in cursor.fetchall()
    ]

    conn.close()
    return formatted_results


def get_items_data():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT enum, type FROM enum_types')
    # Swap: make type_string the key, enum the value
    type_to_enum = {type_str: enum for enum, type_str in cursor.fetchall()}

    cursor.execute('SELECT power, item_level, item_type FROM items WHERE used = 1 ORDER BY name ASC')

    formatted_results = [
        ItemData(power, item_level, item_type)
        for power, item_level, item_type in cursor.fetchall()
    ]

    conn.close()
    return formatted_results


def get_abilities_data():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT enum, type FROM enum_types')
    # Swap: make type_string the key, enum the value
    type_to_enum = {type_str: enum for enum, type_str in cursor.fetchall()}

    cursor.execute('SELECT power, mana_cost, type FROM skills WHERE used = 1 ORDER BY name ASC')

    formatted_results = [
        SkillData(power, mana_cost, type_0)
        for power, mana_cost, type_0 in cursor.fetchall()
    ]

    conn.close()
    return formatted_results


def get_objects_data():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('SELECT enum, type FROM enum_types')
    # Swap: make type_string the key, enum the value
    type_to_enum = {type_str: enum for enum, type_str in cursor.fetchall()}

    cursor.execute('SELECT power, object_type FROM objects WHERE used = 1 ORDER BY name ASC')

    formatted_results = [
        ObjectData(power, object_type)
        for power, object_type in cursor.fetchall()
    ]

    conn.close()
    return formatted_results


def get_type_strings():
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()
    cursor.execute('SELECT type FROM enum_types ORDER BY enum')
    types = cursor.fetchall()
    conn.close()
    return types


def get_model_stats(model_name=None):
    """Get statistics about generated descriptions"""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    if model_name:
        cursor.execute('''
            SELECT 
                COUNT(*) as total,
                AVG(word_count) as avg_words,
                AVG(char_count) as avg_chars,
                COUNT(DISTINCT name) as unique_creatures
            FROM creature_descriptions 
            WHERE model_name = ?
        ''', (model_name,))
    else:
        cursor.execute('''
            SELECT 
                model_name,
                COUNT(*) as total,
                AVG(word_count) as avg_words,
                COUNT(DISTINCT name) as unique_creatures
            FROM creature_descriptions 
            GROUP BY model_name
        ''')

    results = cursor.fetchall()
    conn.close()
    return results


def view_database(limit=20):
    """Print recent saved descriptions"""
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    cursor.execute('''
        SELECT creature_name, description, word_count, model_name, timestamp 
        FROM creature_descriptions 
        ORDER BY timestamp DESC
        LIMIT ?
    ''', (limit,))

    for row in cursor.fetchall():
        print(f"{row[0]} ({row[3]}): {row[1][:60]}... ({row[2]} words, {row[4][:10]})")

    conn.close()
