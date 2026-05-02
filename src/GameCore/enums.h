//
// Created by nathanial on 2/27/26.
//
#pragma once
#include "constants.h"

/**********************************************************************************************************************/
/** Sets the logic path for the game in the main loop
**********************************************************************************************************************/
typedef enum
{
    TITLE_SCREEN = 0,
    GAME_RUNNING = 1,
    NEW_MAP = 2,
    SHUTDOWN = 3,
    GAME_STATE_SIZE,
} GameState;

_Static_assert((GAME_STATE_SIZE == 4), "update State struct to reflect new size");

/**********************************************************************************************************************/
/** Sets the logic path for the game in the main loop
**********************************************************************************************************************/
typedef enum
{
    BATTLE_ATTACK,
    BATTLE_MENUS,
    BATTLE_INIT,
    BATTLE_NONE,
    BATTLE_INPUT_STATE_SIZE,
} BattleState;

_Static_assert((BATTLE_INPUT_STATE_SIZE == 4), "update State struct to reflect new size");

/**********************************************************************************************************************/
/** Sets input logic path for game state branching
**********************************************************************************************************************/
typedef enum
{
    // IDLE,
    MOVING,
    MENU,
    REBOOT,
    ENTER_BATTLE,
    BATTLE,
    INPUT_STATE_SIZE,
} InputState;

_Static_assert((INPUT_STATE_SIZE == 5), "update State struct to reflect new size");

/**********************************************************************************************************************/
/** Options menu list in the main menu
**********************************************************************************************************************/
typedef enum OptionsMenu
{
    OPTIONS_MENU_FONT_SIZE,
    VOLTS,
    GAME_SPEED,
    OPTIONS_MENU_SIZE,
} OptionsMenu;

_Static_assert((OPTIONS_MENU_SIZE == 3), "OptionsMenu state changed");

/**********************************************************************************************************************/
/** Main menu list
**********************************************************************************************************************/
typedef enum
{
    MINIMAP,
    MONSTER_DATA,
    OBJECT_DATA,
    ITEM_DATA,
    SPELL_DATA,
    ABILITY_DATA,
    PARTY,
    BAG,
    SPELLS,
    OPTIONS,
    EXIT,
    MAIN_MENUS_SIZE,
} MainMenu;

_Static_assert((MAIN_MENUS_SIZE == 11), "MainMenu state changed");


/**********************************************************************************************************************/
/** game Biome list
**********************************************************************************************************************/
typedef enum
{
    DESERT,
    SAVANNA,
    STEPPE,
    BADLANDS,
    FOREST,
    TUNDRA,
    SWAMP,
    BOG,
    JUNGLE,
    RAINFOREST,
    BEACH,
    MOUNTAIN,
    VOLCANIC,
    MYSTICAL_GROVE,
    DEEP_CAVERN,
    MAGMA_CORE,
    BIOME_COUNT,
} Biomes;

_Static_assert((BIOME_COUNT == 16), "BIOME_COUNT changed");

/**********************************************************************************************************************/
/** game creature spawning theme list
 *  TODO: add more themes
**********************************************************************************************************************/
typedef enum
{
    THEME_DEMONS,
    THEME_MAGIC,
    THEME_FLAME,
    THEME_FROST,
    THEME_UNDEAD,
    THEME_DRAGONS,
    THEME_BEASTS,
    THEME_GHOSTS,
    THEME_GOBLINS,
    THEME_ORCS,
    THEME_AVIAN,
    THEME_temp03,
    THEME_temp04,
    THEME_temp05,
    THEME_temp06,
    THEME_temp07,
} Themes;

/**********************************************************************************************************************/
/** map tile type list
 *  TODO: finish up the tile types
**********************************************************************************************************************/
typedef enum
{
    GROUND,
    WALL_BRICK,
    WALL_STONE,
    FOLIAGE,
    FOLIAGE3,
    FOLIAGE4,
    FOLIAGE5,
    FOLIAGE6,
    PIT,
    ENEMY1,
    ENEMY2,
    ENEMY3,
    ENEMY4,
    WATER,
    ACID,
    LAVA,
} TileType;


/**********************************************************************************************************************/
/** STAT ENUMS
 *  TODO: more flag enums
**********************************************************************************************************************/

/**********************************************************************************************************************/
/** Haste state
**********************************************************************************************************************/
typedef enum
{
    HASTED = 0,
    SLOWED = 1,
} Speed;

/**********************************************************************************************************************/
/**status effects state
 * stored in the extra bits of the hp struct
**********************************************************************************************************************/
typedef enum
{
    POISONED = 0,
    BURNED = 1,
    LEECH = 2,
    CONFUSION = 3,
} HPStatus;

/**********************************************************************************************************************/
/**status effects state
 * stored in the extra bits of the mp struct
**********************************************************************************************************************/
typedef enum
{
    SLEEP = 0,
    SILENCED = 1,
    PARALYZED = 2,
    CURSED = 3,
} MPStatus;

/**********************************************************************************************************************/
/**status effects state
 * TODO: needs a storage location
**********************************************************************************************************************/
typedef enum
{
    FROZEN = 0,
    DISARMED = 1,
    BLINDED = 2,
    CHARM = 3,
} sStatus;

/**********************************************************************************************************************/
/** Entity types used as an index to the entity creation function array
**********************************************************************************************************************/
typedef enum
{
    CREATURE,
    ITEM,
    OBJECT,
    TOTAL_SPAWNABLE_OBJECT_TYPES,
    SPELL,
    SKILL,
    NO_OBJECT_TYPE,
} ObjectsTypes;

/**********************************************************************************************************************/
/** Items types
 *  TODO: add more item types - 16 probably
**********************************************************************************************************************/
typedef enum
{
    POTION = 0,
    FOOD,
    TRINKET,
    COMMON,
    MAGICAL,
} ItemClass;


/**********************************************************************************************************************/
/** list of all item types in the game
 *  TODO: fill out item list to 255
**********************************************************************************************************************/
typedef enum
{
#include "inc/data_enum_items.inc"
} ItemTypes;

_Static_assert((NO_ITEM <= 255), "FINAL INDEX MUST BE LESS THAN OR EQUAL TO 255");

/**********************************************************************************************************************/
/** list of all object types in the game
 *  TODO: fill out list to 255
**********************************************************************************************************************/

typedef enum
{
#include "inc/data_enum_objects.inc"
} Object;

_Static_assert((NO_OBJECT <= 255), "FINAL INDEX MUST BE LESS THAN OR EQUAL TO 255");

/**********************************************************************************************************************/
/** list of all skill types in the game
 *  TODO: fill out list to 255
**********************************************************************************************************************/
typedef enum
{
#include "inc/data_enum_skills.inc"
} Ability;

_Static_assert((NO_ABILITY <= 255), "FINAL INDEX MUST BE LESS THAN OR EQUAL TO 255");

/**********************************************************************************************************************/
/** list of all creature types in the game
**********************************************************************************************************************/
typedef enum
{
#include "inc/data_enum_creatures.inc"
} Creature;

_Static_assert((NO_CREATURE <= 255), "FINAL INDEX MUST BE LESS THAN OR EQUAL TO 255");

/**********************************************************************************************************************/
/** list of all spell types in the game
 *  TODO: fill out list to 255
**********************************************************************************************************************/
typedef enum
{
#include "inc/data_enum_spells.inc"
} Spell;

_Static_assert(NO_SPELL <= 255, "FINAL INDEX MUST BE LESS THAN OR EQUAL TO 255");

/**********************************************************************************************************************/
/** used to return to the list when an item or spell is used
**********************************************************************************************************************/
typedef enum
{
    BACK_NONE = 0,
    BACK_ITEM,
    BACK_SPELL,
} UseFrameBack;

/**********************************************************************************************************************/
/** Types for creatures, skill and spells
**********************************************************************************************************************/
typedef enum
{
    PLANT,
    FLAME,
    ICE,
    AQUATIC,
    ABERRATION,
    DRAGONS,
    GIANTS,
    BEAST,
    FEY,
    DEMIGOD,
    HUMANOID,
    ANCIENT,
    UNDEAD,
    DEMONIC,
    TOXIC,
    EARTHEN,
    NUM_TYPES,
} Type;

_Static_assert(NUM_TYPES == 16, "max must be 16 - 4 bits");

/**********************************************************************************************************************/
/** Font array indexes as the char ar that index
 *  Needs to be offset by +0x20 to convert to ascii
**********************************************************************************************************************/
typedef enum
{
    CH_SPACE = 0, // ' ' 0x20
    CH_EXCLAMATION, // '!' 0x21
    CH_QUOTE, // '"' 0x22
    CH_HASH, // '#' 0x23
    CH_DOLLAR, // '$' 0x24
    CH_PERCENT, // '%' 0x25
    CH_AMPERSAND, // '&' 0x26
    CH_APOSTROPHE, // ''' 0x27
    CH_LPAREN, // '(' 0x28
    CH_RPAREN, // ')' 0x29
    CH_ASTERISK, // '*' 0x2A
    CH_PLUS, // '+' 0x2B
    CH_COMMA, // ',' 0x2C
    CH_MINUS, // '-' 0x2D
    CH_PERIOD, // '.' 0x2E
    CH_SLASH, // '/' 0x2F
    CH_0, // '0' 0x30
    CH_1, // '1' 0x31
    CH_2, // '2' 0x32
    CH_3, // '3' 0x33
    CH_4, // '4' 0x34
    CH_5, // '5' 0x35
    CH_6, // '6' 0x36
    CH_7, // '7' 0x37
    CH_8, // '8' 0x38
    CH_9, // '9' 0x39
    CH_COLON, // ':' 0x3A
    CH_SEMICOLON, // ';' 0x3B
    CH_LT, // '<' 0x3C
    CH_EQUAL, // '=' 0x3D
    CH_GT, // '>' 0x3E
    CH_QUESTION, // '?' 0x3F
    CH_AT, // '@' 0x40
    CH_A, // 'A' 0x41
    CH_B, // 'B' 0x42
    CH_C, // 'C' 0x43
    CH_D, // 'D' 0x44
    CH_E, // 'E' 0x45
    CH_F, // 'F' 0x46
    CH_G, // 'G' 0x47
    CH_H, // 'H' 0x48
    CH_I, // 'I' 0x49
    CH_J, // 'J' 0x4A
    CH_K, // 'K' 0x4B
    CH_L, // 'L' 0x4C
    CH_M, // 'M' 0x4D
    CH_N, // 'N' 0x4E
    CH_O, // 'O' 0x4F
    CH_P, // 'P' 0x50
    CH_Q, // 'Q' 0x51
    CH_R, // 'R' 0x52
    CH_S, // 'S' 0x53
    CH_T, // 'T' 0x54
    CH_U, // 'U' 0x55
    CH_V, // 'V' 0x56
    CH_W, // 'W' 0x57
    CH_X, // 'X' 0x58
    CH_Y, // 'Y' 0x59
    CH_Z, // 'Z' 0x5A
    CH_LBRACKET, // '[' 0x5B
    CH_BACKSLASH, // '\' 0x5C
    CH_RBRACKET, // ']' 0x5D
    CH_CARET, // '^' 0x5E
    CH_UNDERSCORE, // '_' 0x5F
    CH_BACKTICK, // '`' 0x60
    CH_a, // 'a' 0x61
    CH_b, // 'b' 0x62
    CH_c, // 'c' 0x63
    CH_d, // 'd' 0x64
    CH_e, // 'e' 0x65
    CH_f, // 'f' 0x66
    CH_g, // 'g' 0x67
    CH_h, // 'h' 0x68
    CH_i, // 'i' 0x69
    CH_j, // 'j' 0x6A
    CH_k, // 'k' 0x6B
    CH_l, // 'l' 0x6C
    CH_m, // 'm' 0x6D
    CH_n, // 'n' 0x6E
    CH_o, // 'o' 0x6F
    CH_p, // 'p' 0x70
    CH_q, // 'q' 0x71
    CH_r, // 'r' 0x72
    CH_s, // 's' 0x73
    CH_t, // 't' 0x74
    CH_u, // 'u' 0x75
    CH_v, // 'v' 0x76
    CH_w, // 'w' 0x77
    CH_x, // 'x' 0x78
    CH_y, // 'y' 0x79
    CH_z, // 'z' 0x7A
    CH_LBRACE, // '{' 0x7B
    CH_PIPE, // '|' 0x7C
    CH_RBRACE, // '}' 0x7D
    CH_TILDE, // '~' 0x7E
    CH_NONE,
} Character;

_Static_assert(CHARACTER_COUNT <= 256, "max must be 256 - one byte");

/**********************************************************************************************************************/
/** indexes into the pallet of colours used
**********************************************************************************************************************/
enum
{
    // 0–15: neutral / humanoid / common tones
    PAL_KEY = 0, //magenta trans key
    PAL_PEACH_SKIN,
    PAL_MEDIUM_BROWN,
    PAL_PALE_GREEN,
    PAL_GRAYISH_GREEN,
    PAL_DIRTY_YELLOW_GRN,
    PAL_BROWNISH_RED,
    PAL_LIGHT_TAN,
    PAL_GRAY_BLUE,
    PAL_DULL_ORANGE,
    PAL_DARK_BROWN,
    PAL_MUTED_GREEN,
    PAL_DARK_GRAY_GREEN,
    PAL_GRAY_BROWN,
    PAL_LIGHT_GRAY,
    PAL_TAN_BROWN,

    PAL_BRIGHT_LIGHT_GRN,
    PAL_DEEP_FOREST_GRN,
    PAL_VIVID_GREEN,
    PAL_LIME_GREEN,
    PAL_PASTEL_GREEN,
    PAL_VERY_DARK_GREEN,
    PAL_DARK_OLIVE,
    PAL_GRAY_GREEN,
    PAL_BROWN_GREEN,
    PAL_TAN_GREEN,
    PAL_EMERALD_GREEN,
    PAL_DARK_GRN_BLACK,
    PAL_SICKLY_GREEN,
    PAL_PALE_MINT,
    PAL_BRIGHT_VINE_GRN,
    PAL_PURPLE_GREEN,
    PAL_ORANGISH_RED,
    PAL_BRIGHT_ORANGE,
    PAL_RED_ORANGE,
    PAL_DEEP_RED,
    PAL_FIERY_MAGENTA,
    PAL_DARK_RED,
    PAL_BRIGHT_RED,
    PAL_DARK_BLUE_GRAY_RED,
    PAL_REDDISH_BROWN,
    PAL_SANDY_ORANGE,
    PAL_DARK_RED_ORANGE,
    PAL_TAN_RED,
    PAL_BRIGHT_ORN_RED,
    PAL_GLOWING_ORANGE,
    PAL_ORANGISH_RED_ORN_RED,
    PAL_DARK_ORANGE,
    PAL_OFF_WHITE_GRAY,
    PAL_SICKLY_GRN_GRAY,
    PAL_PALE_BLU_PURP,
    PAL_DARK_PURP_RED,
    PAL_OFF_WHITE_GRAY_BLUE,
    PAL_DARK_BLUE_GRAY,
    PAL_PALE_CYAN,
    PAL_ICE_BLUE,
    PAL_GOLD,
    PAL_BRIGHT_CYAN,
    PAL_DEEP_BLUE,
    PAL_DARK_BLU_PURP,
    PAL_TEAL,
    PAL_PURPLE_BLUE,
    PAL_MEDIUM_PURPLE,
    PAL_DEEP_VIOLET,
};

_Static_assert(PALETTE_COUNT == 64, "Palette must be 64 colors for 6-bit fg");

/**********************************************************************************************************************/
/** font size state
**********************************************************************************************************************/
typedef enum
{
    FONT16x16 = 1,
    FONT8x8 = 2,
    FONT96x96 = 3,
} FontSize;


/**********************************************************************************************************************/
/*
*   SOUNDS
*
**********************************************************************************************************************/

/**********************************************************************************************************************/
/** Instrument types
**********************************************************************************************************************/
typedef enum
{
    VIOLIN = 0,
    PIANO = 1,
    FLUTE = 2,
    DRUM = 3,
} Instruments;

/**********************************************************************************************************************/
/** Articulation types for notes
**********************************************************************************************************************/
typedef enum
{
    ARTICULATION_NONE = 0,
    ARTICULATION_STACCATO = 1,
    ARTICULATION_LEGATO = 2,
    ARTICULATION_SIZE = 3,
} Articulation;

/**********************************************************************************************************************/
/** Indexes into the note palette array as the name of the notes
**********************************************************************************************************************/
typedef enum
{
    NOTE_C_NEG1 = 0,
    NOTE_CS_NEG1,
    NOTE_D_NEG1,
    NOTE_DS_NEG1,
    NOTE_E_NEG1,
    NOTE_F_NEG1,
    NOTE_FS_NEG1,
    NOTE_G_NEG1,
    NOTE_GS_NEG1,
    NOTE_A_NEG1,
    NOTE_AS_NEG1,
    NOTE_B_NEG1,

    NOTE_C0,
    NOTE_CS0,
    NOTE_D0,
    NOTE_DS0,
    NOTE_E0,
    NOTE_F0,
    NOTE_FS0,
    NOTE_G0,
    NOTE_GS0,
    NOTE_A0,
    NOTE_AS0,
    NOTE_B0,

    NOTE_C1,
    NOTE_CS1,
    NOTE_D1,
    NOTE_DS1,
    NOTE_E1,
    NOTE_F1,
    NOTE_FS1,
    NOTE_G1,
    NOTE_GS1,
    NOTE_A1,
    NOTE_AS1,
    NOTE_B1,

    NOTE_C2,
    NOTE_CS2,
    NOTE_D2,
    NOTE_DS2,
    NOTE_E2,
    NOTE_F2,
    NOTE_FS2,
    NOTE_G2,
    NOTE_GS2,
    NOTE_A2,
    NOTE_AS2,
    NOTE_B2,

    NOTE_C3,
    NOTE_CS3,
    NOTE_D3,
    NOTE_DS3,
    NOTE_E3,
    NOTE_F3,
    NOTE_FS3,
    NOTE_G3,
    NOTE_GS3,
    NOTE_A3,
    NOTE_AS3,
    NOTE_B3,

    NOTE_C4,
    NOTE_CS4,
    NOTE_D4,
    NOTE_DS4,
    NOTE_E4,
    NOTE_F4,
    NOTE_FS4,
    NOTE_G4,
    NOTE_GS4,
    NOTE_A4,
    NOTE_AS4,
    NOTE_B4,

    NOTE_C5,
    NOTE_CS5,
    NOTE_D5,
    NOTE_DS5,
    NOTE_E5,
    NOTE_F5,
    NOTE_FS5,
    NOTE_G5,
    NOTE_GS5,
    NOTE_A5,
    NOTE_AS5,
    NOTE_B5,

    NOTE_C6,
    NOTE_CS6,
    NOTE_D6,
    NOTE_DS6,
    NOTE_E6,
    NOTE_F6,
    NOTE_FS6,
    NOTE_G6,
    NOTE_GS6,
    NOTE_A6,
    NOTE_AS6,
    NOTE_B6,

    NOTE_C7,
    NOTE_CS7,
    NOTE_D7,
    NOTE_DS7,
    NOTE_E7,
    NOTE_F7,
    NOTE_FS7,
    NOTE_G7,
    NOTE_GS7,
    NOTE_A7,
    NOTE_AS7,
    NOTE_B7,

    NOTE_C8,
    NOTE_CS8,
    NOTE_D8,
    NOTE_DS8,
    NOTE_E8,
    NOTE_F8,
    NOTE_FS8,
    NOTE_G8,
    NOTE_GS8,
    NOTE_A8,
    NOTE_AS8,
    NOTE_B8,

    NOTE_C9,
    NOTE_CS9,
    NOTE_D9,
    NOTE_DS9,
    NOTE_E9,
    NOTE_F9,
    NOTE_FS9,
    NOTE_G9
} NoteIndex;


/**********************************************************************************************************************/
/** menu window state enum
**********************************************************************************************************************/
typedef enum
{
    MAP_SUBMENU,
    MONSTER_DATA_SUBMENU,
    OBJECT_DATA_SUBMENU,
    ITEM_DATA_SUBMENU,
    SPELL_DATA_SUBMENU,
    ABILITY_DATA_SUBMENU,
    MONSTERS_SUBMENU,
    BAG_SUBMENU,
    SPELLS_SUBMENU,
    OPTIONS_SUBMENU,
    MONSTER_DATA_LIST_SUBMENU,
    OBJECT_DATA_LIST_SUBMENU,
    ITEM_DATA_LIST_SUBMENU,
    SPELL_DATA_LIST_SUBMENU,
    ABILITY_DATA_LIST_SUBMENU,
    MAIN_MENU,
    MENU_NONE,
    NUM_SUBMENUS,
} SubMainMenuWindow;


typedef enum
{
    MAP_LEVEL_UP,
    MAP_LEVEL_DOWN,
    MAP_LEVEL_LATERAL,
} MapLevelChange;
