//
// Created by nathanial on 2/26/26.
//
#pragma once
#include "stdbool.h"
#include "types.h"
#include "enums.h"
#include "hash_map.h"
#include "lib_types.h"
#include "utils.h"

/**********************************************************************************************************************
*
*   RAM layout struct
*
**********************************************************************************************************************/

typedef struct
{
    /**********************************************************************************************************************
    *   Level data
    **********************************************************************************************************************/
    State state;
    uint16_t seed; // for reproducible RNG if desired
    uint16_t turn_count;
    uint16_t turn_final;
    uint8_t floor;
    Biomes biome;
    Themes theme;
    Camera camera;


    /**********************************************************************************************************************
    *   settings cache
    **********************************************************************************************************************/
    struct
    {
        FontSize fontSize;
    } settings;

    struct
    {
        uint8_t level;
        CharStr_uint16 volts;
    } hardware;

    /**********************************************************************************************************************
    *   input cache
    **********************************************************************************************************************/
    struct
    {
        int8_t gameSpeed;
        char gameSpeedStr[16];
    } btns;


    /**********************************************************************************************************************
    *   battle mode data
    **********************************************************************************************************************/
    struct
    {
        ObjectsTypes moveType;
        ObjectType moveID;
        EntityId playerMonsterID;
        EntityId enemyMonsterID;
        char combatLog[2][40]; // TODO: expand o <= 30 lines for a full screen combat log, dont bother with scrolling, use a wraparound ptr
        char hpStr[10];
    } battleMode;


    /**********************************************************************************************************************
    *   map data
    **********************************************************************************************************************/
    uint8_t map[(MAP_H * MAP_W) / 2];
    // EntityId objectMap[MAP_H][MAP_W];
    uint8_t fog[(MAP_H * MAP_W) / 8]; // TODO: not yet implemented
    uint8_t vision[40]; // TODO: not yet implemented
    // Node pathing[(MAP_H * MAP_W) / 2]; // TODO: not yet implemented

    /**********************************************************************************************************************
    *   Dirty tile rendering cache arrays
    **********************************************************************************************************************/
    uint8_t viewTiles[VIEW_TH][VIEW_TW];
    Creature viewObjects[VIEW_TH][VIEW_TW];
    bool dirtyTiles[VIEW_TH][VIEW_TW];
    Creature newSprites[VIEW_TH][VIEW_TW];

    /**********************************************************************************************************************
    *   graphics cache
    **********************************************************************************************************************/
    struct
    {
        // uint16_t frameBuffer[BUFFER_H*SCREEN_W];
        // PartialFrameBuffer frameBuffer;

        bool dirtyTiles[VIEW_TH][VIEW_TW];
        Creature newSprites[VIEW_TH][VIEW_TW];

        Glyph16x16 tilePixels; // 512 bytes
        Glyph16x16 spritePixels; // 512 bytes
        uint8_t tile_id; // Which tile is cached
        uint8_t sprite_id; // Which sprite is cached
        uint8_t dirty; // Flag if needs refresh
    } tileCache;

    /**********************************************************************************************************************
    *   player data
    **********************************************************************************************************************/
    struct
    {
        EntityId partyID[MAX_PARTY_SIZE];
        EntityId itemID[MAX_BAG_SIZE];
        SpellId spellID[MAX_SPELLBOOK_SIZE];

        uint16_t cur_xp[MAX_PARTY_SIZE];
        uint16_t tar_xp[MAX_PARTY_SIZE];

        // TODO: these are the "pokedexes" add checks to populate them
        BitFieldUint8 sacrificedCreatures;
        BitFieldUint8 knownCreatures;
        BitFieldUint8 knownSpells;
        BitFieldUint8 knownItems;
        BitFieldUint8 knownAbilities;
        BitFieldUint8 knownObjects;

        EntityId id;
        uint8_t currentBagSize;
        uint8_t currentSpellbookSize;
        uint16_t cur_mana;
        uint16_t max_mana;
        Delta d;
        Delta scroll;
    } player;


    /**********************************************************************************************************************
    *   object data arrays
    **********************************************************************************************************************/
    struct
    {
        Position position[ENTITY_COUNT];
        Position newPosition[ENTITY_COUNT]; // can be changed to hold delta, a uint8_t can hold up to 2
        uint8_t types[ENTITY_COUNT];
        ObjectType metaData[ENTITY_COUNT]; //any 8 bit data, Creature type, Item type, etc
        BitFieldUint8 onMap;
        BitFieldUint8 active;

        struct
        {
            uint8_t hovering[ENTITY_COUNT / 2];
            uint8_t waterWalk[ENTITY_COUNT / 2];
            uint8_t waterBreathing[ENTITY_COUNT / 2];
            uint8_t repel[ENTITY_COUNT / 2];
            uint8_t invisibility[ENTITY_COUNT / 2];
            uint8_t wallWalking[ENTITY_COUNT / 2];
            uint8_t fireEating[ENTITY_COUNT / 2];
            uint8_t lineOfSight[ENTITY_COUNT]; // controls whether terrain blocks L/2oS
            uint8_t paralyzed[ENTITY_COUNT / 2];
            uint8_t sleep[ENTITY_COUNT / 2];
            uint8_t poison[ENTITY_COUNT / 2];
            uint8_t frozen[ENTITY_COUNT / 2];
            uint8_t disease[ENTITY_COUNT / 2];
            uint8_t curse[ENTITY_COUNT / 2];
            uint8_t hasted[ENTITY_COUNT / 2];
            uint8_t fear[ENTITY_COUNT / 2];
            uint8_t burned[ENTITY_COUNT / 2];
            uint8_t light[ENTITY_COUNT / 2];
            uint8_t slowed[ENTITY_COUNT / 2];
        } status;

        struct
        {
            uint8_t toxic[ENTITY_COUNT / 2];
            uint8_t fire[ENTITY_COUNT / 2];
            uint8_t water[ENTITY_COUNT / 2];
            uint8_t ice[ENTITY_COUNT / 2];
            uint8_t earth[ENTITY_COUNT / 2];
            uint8_t magic[ENTITY_COUNT / 2];
            BitFieldUint8 toxicEating;
            BitFieldUint8 fireEating;
            BitFieldUint8 waterEating;
            BitFieldUint8 iceEating;
            BitFieldUint8 earthEating;
            BitFieldUint8 magicEating;
        } resist;

        struct
        {
            uint8_t strength[ENTITY_COUNT / 2];
            uint8_t defence[ENTITY_COUNT / 2];
            uint8_t magic[ENTITY_COUNT / 2];
            uint8_t speed[ENTITY_COUNT / 2];
        } stat;

        BitFieldUint8 alive;
        Stats stats[ENTITY_COUNT];
        Attacks attacks[ENTITY_COUNT];
        IntMax999 hp[ENTITY_COUNT];
        IntMax999 mp[ENTITY_COUNT];
        IntMax999 xp[ENTITY_COUNT]; // can be changed to hold delta, a uint8_t can hold up to 2
        IntMax99 speed[ENTITY_COUNT];
        Int99 level[ENTITY_COUNT];
        Senses senses[ENTITY_COUNT];
        Senses stealth[ENTITY_COUNT];
    } creatures;

    struct
    {
        Position position[ENTITY_COUNT];
        Position newPosition[ENTITY_COUNT]; // can be changed to hold delta, a uint8_t can hold up to 2
        uint8_t types[ENTITY_COUNT];
        ObjectType metaData[ENTITY_COUNT]; //any 8 bit data, Creature type, Item type, etc
        BitFieldUint8 onMap;
        BitFieldUint8 active;
    } objects;

    struct
    {
        Position position[ENTITY_COUNT];
        Position newPosition[ENTITY_COUNT]; // can be changed to hold delta, a uint8_t can hold up to 2
        uint8_t types[ENTITY_COUNT];
        ObjectType metaData[ENTITY_COUNT]; //any 8 bit data, Creature type, Item type, etc
        BitFieldUint8 onMap;
        BitFieldUint8 active;
    } items;


    /**********************************************************************************************************************
    *   ui cache data
    **********************************************************************************************************************/
    struct
    {
        const char* text[MAX_MENU_SIZE];
        uint8_t depth;
        Vec_8 eraseSel;
        Vec_8_Signed sel[MAX_MENU_DEPTH]; //TODO: placeholder until I know the depth
        Vec_8 menuScrollOffsetCache; //TODO: placeholder until I know the depth
        Vec_8 menuScrollOffset[MAX_MENU_DEPTH]; //TODO: placeholder until I know the depth
        uint8_t selectedMenu;
        uint8_t lineHeight;
        uint8_t displayedMenu;
        uint8_t visibleMenuOptions;
        uint8_t totalMenuOptions;
        bool forceRedraw;
        UseFrameBack useOnPartyMember;
        uint16_t colorCache;
        uint8_t x; // in tiles
        uint8_t y;
        uint8_t h;
        SubMenu* subMenus;

        struct
        {
            bool open;
            EntityId displayId;
            EntityId id;
            // ObjectType  objectType;
        } gameMenu;
    } menu;

    /**********************************************************************************************************************
    *   sound cache data
    **********************************************************************************************************************/
    struct
    {
        bool w;
        uint16_t master_volume;
        uint16_t master_dimmer;
        Note notes[GENERATED_MELODY_LENGTH * 2];
    } music;
} GameRunState;

extern GameRunState g_run; // Declaration only
void GameRunInit();


void GameRun_Reset(void); // call at new game / death
