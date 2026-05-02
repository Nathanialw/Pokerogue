//
// Created by nathanial on 2/22/26.
//

#include "map.h"

#include "camera.h"
#include "entities.h"
#include "lib_debugging.h"
#include "player.h"

#include "utils.h"

/**********************************************************************************************************************/
/**Map constant values
**********************************************************************************************************************/

#define MAP_MIN_X 8
#define MAP_MIN_Y 6
#define MAP_MAX_X (MAP_W - MAP_MIN_X - 2)
#define MAP_MAX_Y (MAP_H - MAP_MIN_Y - 1)

/**********************************************************************************************************************/
/**MAP GENERATION
**********************************************************************************************************************/
//
typedef void (*DungeonLayout)(void);

#define TOTAL_DUNGEON_GEN_ALGOS 3
void DungeonBasic(void);
void DungeonGraph(void);
void DungeonCave(void);

// TODO: add more dungeon layouts
const DungeonLayout GenerateMap[TOTAL_DUNGEON_GEN_ALGOS] =
{
    DungeonBasic, // trad roguelike layout
    DungeonGraph, // almost the same as above
    DungeonCave, // natural looking cave
};


/**********************************************************************************************************************/
/** Map Generation entry point
 *  Calls a given map generation type and populates the map cells
**********************************************************************************************************************/
void GenerateDungeon(uint8_t type)
{
    GenerateMap[type]();
}

/**********************************************************************************************************************/
/*
 *  MAP
 *
**********************************************************************************************************************/

/**********************************************************************************************************************/
/**Sets the cached level data
**********************************************************************************************************************/
void UpdateLevel(uint8_t floor, Biomes biome)
{
    g_run.floor = floor;
    g_run.biome = biome;
    g_run.turn_count = 0;
    g_run.turn_final = 300; //TODO: Generate a random number of turns
}

/**********************************************************************************************************************/
/** Returns the position of a random map tile
 *  if emptyOnly == true will only return an empty tile
**********************************************************************************************************************/
Position GetRandomMapTile(ObjectsTypes type, bool emptyOnly)
{
    Position* positions = GetEntityPositions(type);

    do
    {
        uint8_t x = GetRandom_uint8_t(0, MAP_W - 1);
        uint8_t y = GetRandom_uint8_t(0, MAP_H - 1);
        Position pos = {x, y};

        if (emptyOnly)
        {
            if (GetMapTile(x, y) == GROUND && CheckTileForEntity(type, NO_ENTITY, pos, positions, ENTITY_COUNT) == NO_ENTITY)
            {
                return pos;
            }
        }
        else
        {
            return pos;
        }
    }
    while (1);
}

/**********************************************************************************************************************/
/** TODO: NOT YET IMPLEMENTED
 *  Returns the position of the tile selected by the player
 *  TODO: we need a way to have an unlocked camera
**********************************************************************************************************************/
Position GetSelectedTile(bool emptyOnly)
{
    uint8_t x = GetRandom_uint8_t(0, MAP_W - 1);
    uint8_t y = GetRandom_uint8_t(0, MAP_H - 1);
    Position pos = {x, y};
    return pos;
}

/**********************************************************************************************************************/
/** Takes in a map position and returns the index of the tile type
**********************************************************************************************************************/
uint8_t GetMapTile(uint8_t x, uint8_t y)
{
    uint16_t index = (y * MAP_W) + x;
    uint8_t byte = g_run.map[index / 2];
    if (index & 1)
        return byte >> 4; // High nibble for odd indices

    return byte & 0x0F; // Low nibble for even indices
}

/**********************************************************************************************************************/
/** TODO: NOT YET IMPLEMENTED
 *  Checks whether a given map cell position contains a corpse
 *  TODO: search objects for one in the current tile
**********************************************************************************************************************/
bool TileHasCorpse(Position pos)
{
    return GetMapTile(pos.x, pos.y) == GROUND;
}

/**********************************************************************************************************************/
/** Sets the map tile at the given position to the given tile index
**********************************************************************************************************************/
void SetMapTile(uint8_t x, uint8_t y, TileType tile)
{
    uint16_t index = (y * MAP_W) + x;
    uint8_t* byte = &g_run.map[index / 2];
    if (index & 1)
    {
        *byte = (*byte & 0x0F) | (tile << 4); // Set high nibble
    }
    else
    {
        *byte = (*byte & 0xF0) | (tile & 0x0F); // Set low nibble
    }
}

/**********************************************************************************************************************/
/*  increments level value
 *  generates a new map
 *  populates new map
 *  place player onto the map in an empty cell
**********************************************************************************************************************/
void GoNextLevel(MapLevelChange dir)
{
    if (dir == MAP_LEVEL_UP && g_run.floor > 1)
        g_run.floor--;
    else if (dir == MAP_LEVEL_DOWN && g_run.floor < MAX_LEVELS)
        g_run.floor++;

    UpdateLevel(g_run.floor, DESERT);
    GenerateDungeon(0);
    ResetEntities(true);
    PopulateLevelCreatures();
    PlacePlayerOnMap();
}

/**********************************************************************************************************************/
/*
 *  FOG
 *
**********************************************************************************************************************/

/**********************************************************************************************************************/
/** Takes in a given position and returns a bool
 *      if fog == true -> returns true
 *      if fog == false -> returns false
**********************************************************************************************************************/
bool IsFogged(uint16_t x, uint16_t y)
{
    uint16_t idx = (y * MAP_W) + x;
    return (g_run.fog[idx >> 3] >> (idx & 7)) & 1;
}

/**********************************************************************************************************************/
/** Sets the fog value at the given position to the given fog value
**********************************************************************************************************************/
void SetFog(uint16_t x, uint16_t y, bool fogged)
{
    uint16_t idx = y * MAP_W + x;
    uint8_t mask = 1 << (idx & 7);
    if (fogged)
        g_run.fog[idx >> 3] |= mask;
    else
        g_run.fog[idx >> 3] &= ~mask;
}

/**********************************************************************************************************************/
/** Sets the fog value of the entire map to the given fog value
**********************************************************************************************************************/
void SetMapFog(uint8_t set)
{
    for (uint16_t y = 0; y < MAP_H / 8; y++)
        for (uint16_t x = 0; x < MAP_W / 8; x++)
        {
            // SetFog(x, y, set);
            g_run.fog[y + (x * MAP_W / 8)] = set;
        }
}

/**********************************************************************************************************************/
/*
 *  INITIALIZATION
 *
**********************************************************************************************************************/

/**********************************************************************************************************************/
/** Generate map by index
**********************************************************************************************************************/
void InitMapLayout(void)
{
    GenerateDungeon(2);
}

/**********************************************************************************************************************/
/** Searches map for a random empty tile
 *  returns tile position when it is found
**********************************************************************************************************************/
Position FindOpenMapLocation(ObjectsTypes type)
{
    Position* positions = GetEntityPositions(type);

    while (1)
    {
        Position pos;
        pos.x = 16 + rand() % MAP_W - 16;
        pos.y = 16 + rand() % MAP_H - 16;
        if (GetMapTile(pos.x, pos.y) == GROUND && CheckTileForEntity(type, NO_ENTITY, pos, positions, ENTITY_COUNT) == NO_ENTITY)
            return pos;
    }
}


/**********************************************************************************************************************/
/** Increments floor level
 *  Gets random map biome
 *  Sets the map data to defaults
 *  Sets the fog map to defaults
**********************************************************************************************************************/
void InitMap(void)
{
    g_run.floor++;
    g_run.biome = DESERT;
    InitCamera(0, 0, TILE_W * VIEW_TW, TILE_H * VIEW_TH);
    UpdateLevel(g_run.floor, g_run.biome);
    InitMapLayout();
    SetMapFog(0xFF);
}

/**********************************************************************************************************************/
/*
*   GENERATE BasicDungeon
*
**********************************************************************************************************************/

#define MAX_ROOMS 128

typedef struct
{
    uint16_t x, y, w, h;
} Room;

Room rooms[MAX_ROOMS];
uint8_t roomCount = 0;

/**********************************************************************************************************************/
/** Checks whether given room rect A intersects with room rect A
 *  ON SUCCESS - return true
 *  ON FAIL - return false
**********************************************************************************************************************/
int Intersects(Room a, Room b)
{
    return (a.x <= b.x + b.w + 1 &&
        a.x + a.w + 1 >= b.x &&
        a.y <= b.y + b.h + 1 &&
        a.y + a.h + 1 >= b.y);
}

/**********************************************************************************************************************/
/** Updates the tile map with ground tiles at the position and dimensions of the given room rect
**********************************************************************************************************************/
void CarveRoom(Room r)
{
    for (uint8_t y = r.y; y < r.y + r.h; y++)
        for (uint8_t x = r.x; x < r.x + r.w; x++)
            SetMapTile(x, y, GROUND);
}

/**********************************************************************************************************************/
/** Updates the tile map with ground tiles between given x1 and x2 position at the given y position
**********************************************************************************************************************/
void CarveHorizontal(uint8_t x1, uint8_t x2, uint8_t y)
{
    if (x2 < x1)
    {
        uint8_t t = x1;
        x1 = x2;
        x2 = t;
    }

    for (uint8_t x = x1; x <= x2; x++)
        SetMapTile(x, y, GROUND);
}

/**********************************************************************************************************************/
/** Updates the tile map with ground tiles between given y1 and y2 position at the given x position
**********************************************************************************************************************/
void CarveVertical(uint8_t y1, uint8_t y2, uint8_t x)
{
    if (y2 < y1)
    {
        uint8_t t = y1;
        y1 = y2;
        y2 = t;
    }

    for (uint8_t y = y1; y <= y2; y++)
        SetMapTile(x, y, GROUND);
}

/**********************************************************************************************************************/
/** Updates the tile map with ground tiles connecting the given room rect A and room rect B
**********************************************************************************************************************/
void ConnectRooms(Room a, Room b)
{
    uint8_t ax = a.x + a.w / 2;
    uint8_t ay = a.y + a.h / 2;

    uint8_t bx = b.x + b.w / 2;
    uint8_t by = b.y + b.h / 2;

    if (rand() & 1)
    {
        CarveHorizontal(ax, bx, ay);
        CarveVertical(ay, by, bx);
    }
    else
    {
        CarveVertical(ay, by, ax);
        CarveHorizontal(ax, bx, by);
    }
}

/**********************************************************************************************************************/
/** Core basic Map Generation algrithm
 *  Creates random position and dimention rooms of ground tiles around the map
 *  Connects those rooms with corridors of ground tiles
**********************************************************************************************************************/
void DungeonBasic(void)
{
    DEBUG("GenerateDungeon() BEGIN");

    roomCount = 0;

    for (uint16_t y = 0; y < MAP_H; y++)
        for (uint16_t x = 0; x < MAP_W; x++)
            SetMapTile(x, y, WALL_STONE);

    DEBUG("GenerateDungeon() Layout reset");

    for (uint8_t i = 0; i < MAX_ROOMS; i++)
    {
        Room r;

        r.w = 4 + rand() % 10;
        r.h = 4 + rand() % 10;

        r.x = rand() % (MAP_W - r.w - 2) + 1;
        r.y = rand() % (MAP_H - r.h - 2) + 1;

        uint8_t failed = 0;

        for (uint8_t j = 0; j < roomCount; j++)
        {
            if (Intersects(r, rooms[j]))
            {
                failed = 1;
                break;
            }
        }

        if (!failed)
        {
            CarveRoom(r);

            if (roomCount > 0)
                ConnectRooms(rooms[roomCount - 1], r);

            rooms[roomCount++] = r;
        }
    }

    DEBUG("NUM ROOMS CREATED %d", roomCount);
}

/**********************************************************************************************************************/
/*
 *  GENERATE DungeonGraph
 *
**********************************************************************************************************************/

#define MAX_ROOMS 128
#define MAX_EDGES (MAX_ROOMS * 6)

typedef struct
{
    uint8_t a;
    uint8_t b;
    uint16_t dist;
} Edge;

Edge edges[MAX_EDGES];
uint16_t edgeCount = 0;


/**********************************************************************************************************************/
/*  Returns the distance between a given x1,y1 and a given x2,y2 in number of cells to traverse axis aligned
**********************************************************************************************************************/
uint16_t Dist2(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int16_t dx = (int16_t)x1 - (int16_t)x2;
    int16_t dy = (int16_t)y1 - (int16_t)y2;

    return dx * dx + dy * dy;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void RoomCenter(Room r, uint8_t* cx, uint8_t* cy)
{
    *cx = r.x + r.w / 2;
    *cy = r.y + r.h / 2;
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void BuildEdges(void)
{
    edgeCount = 0;

    for (int i = 0; i < roomCount; i++)
    {
        uint8_t ax, ay;
        RoomCenter(rooms[i], &ax, &ay);

        uint8_t used[MAX_ROOMS] = {0};

        for (int k = 0; k < 6; k++) // 6 neighbors is safer
        {
            int best = -1;
            uint16_t bestDist = 0xFFFF;

            for (int j = 0; j < roomCount; j++)
            {
                if (i == j) continue;
                if (used[j]) continue;

                uint8_t bx, by;
                RoomCenter(rooms[j], &bx, &by);

                uint16_t d = Dist2(ax, ay, bx, by);

                if (d < bestDist)
                {
                    bestDist = d;
                    best = j;
                }
            }

            if (best >= 0 && edgeCount < MAX_EDGES)
            {
                edges[edgeCount].a = i;
                edges[edgeCount].b = best;
                edges[edgeCount].dist = bestDist;
                edgeCount++;

                used[best] = 1;
            }
        }
    }
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void CarveCorridorZigZag(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int16_t x = x1;
    int16_t y = y1;

    SetMapTile(x, y, GROUND);

    while (x != x2 || y != y2)
    {
        if (rand() & 1)
        {
            if (x < x2) x++;
            else if (x > x2) x--;
        }
        else
        {
            if (y < y2) y++;
            else if (y > y2) y--;
        }

        SetMapTile(x, y, GROUND);
    }
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void CarveCorridorLinear(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    if (rand() & 1)
    {
        while (x1 != x2)
        {
            SetMapTile(x1, y1, GROUND);
            x1 += (x2 > x1) ? 1 : -1;
        }

        while (y1 != y2)
        {
            SetMapTile(x1, y1, GROUND);
            y1 += (y2 > y1) ? 1 : -1;
        }
    }
    else
    {
        while (y1 != y2)
        {
            SetMapTile(x1, y1, GROUND);
            y1 += (y2 > y1) ? 1 : -1;
        }

        while (x1 != x2)
        {
            SetMapTile(x1, y1, GROUND);
            x1 += (x2 > x1) ? 1 : -1;
        }
    }

    SetMapTile(x2, y2, GROUND);
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void BuildMST(void)
{
    uint8_t visited[MAX_ROOMS] = {0};

    visited[0] = 1;
    uint8_t visitedCount = 1;

    while (visitedCount < roomCount)
    {
        int16_t bestEdge = -1;
        uint16_t bestDist = 0xFFFF;

        for (int i = 0; i < edgeCount; i++)
        {
            Edge e = edges[i];

            if (visited[e.a] && !visited[e.b] && e.dist < bestDist)
            {
                bestEdge = i;
                bestDist = e.dist;
            }

            if (visited[e.b] && !visited[e.a] && e.dist < bestDist)
            {
                bestEdge = i;
                bestDist = e.dist;
            }
        }

        if (bestEdge == -1)
        {
            // force connect nearest unvisited room
            for (int i = 0; i < roomCount; i++)
            {
                if (visited[i]) continue;

                uint8_t ax, ay;
                RoomCenter(rooms[i], &ax, &ay);

                int best = -1;
                uint16_t bestDist = 0xFFFF;

                for (int j = 0; j < roomCount; j++)
                {
                    if (!visited[j]) continue;

                    uint8_t bx, by;
                    RoomCenter(rooms[j], &bx, &by);

                    uint16_t d = Dist2(ax, ay, bx, by);

                    if (d < bestDist)
                    {
                        bestDist = d;
                        best = j;
                    }
                }

                if (best >= 0)
                {
                    uint8_t bx, by;
                    RoomCenter(rooms[best], &bx, &by);

                    CarveCorridorLinear(ax, ay, bx, by);

                    visited[i] = 1;
                    visitedCount++;
                    break;
                }
            }

            continue;
        }

        Edge e = edges[bestEdge];

        uint8_t ax, ay, bx, by;
        RoomCenter(rooms[e.a], &ax, &ay);
        RoomCenter(rooms[e.b], &bx, &by);

        CarveCorridorLinear(ax, ay, bx, by);

        if (!visited[e.a])
        {
            visited[e.a] = 1;
            visitedCount++;
        }
        if (!visited[e.b])
        {
            visited[e.b] = 1;
            visitedCount++;
        }
    }
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void AddExtraConnections(void)
{
    for (uint16_t i = 0; i < edgeCount; i++)
    {
        if (rand() % 100 < 15)
        {
            uint8_t ax, ay, bx, by;

            RoomCenter(rooms[edges[i].a], &ax, &ay);
            RoomCenter(rooms[edges[i].b], &bx, &by);

            CarveCorridorLinear(ax, ay, bx, by);
        }
    }
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void DebugPrintMap(void)
{
#ifdef ENABLE_DEBUG_OUTPUT
    for (uint16_t y = 0; y < MAP_H; y++)
    {
        for (uint16_t x = 0; x < MAP_W; x++)
        {
            TileType t = GetMapTile(x, y);

            char c = '?';

            switch (t)
            {
            case WALL_STONE: c = '#';
                break;
            case GROUND: c = '.';
                break;
            default: c = '?';
                break;
            }
            printf("%c", c);
        }
        printf("\n");
    }
#endif
}

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void DungeonGraph(void)
{
    roomCount = 0;

    for (uint16_t y = 0; y < MAP_H; y++)
        for (uint16_t x = 0; x < MAP_W; x++)
            SetMapTile(x, y, WALL_STONE);

    for (uint8_t i = 0; i < MAX_ROOMS; i++)
    {
        Room r;

        r.w = 5 + rand() % 10;
        r.h = 5 + rand() % 10;

        r.x = rand() % (MAP_W - r.w - 2) + 1;
        r.y = rand() % (MAP_H - r.h - 2) + 1;

        uint8_t failed = 0;

        for (uint8_t j = 0; j < roomCount; j++)
        {
            if (Intersects(r, rooms[j]))
            {
                failed = 1;
                break;
            }
        }

        if (!failed)
        {
            CarveRoom(r);
            rooms[roomCount++] = r;
        }
    }

    BuildEdges();
    BuildMST();
    AddExtraConnections();
    // DebugPrintMap();
    DEBUG("rooms %d", roomCount);
}


/**********************************************************************************************************************/
/** GENERATE DungeonDrunkard
**********************************************************************************************************************/

/**********************************************************************************************************************/
/*
**********************************************************************************************************************/
void DungeonCave(void)
{
    uint16_t x = MAP_W / 2;
    uint16_t y = MAP_H / 2;

    uint32_t carved = 0;
    uint32_t target = (MAP_W * MAP_H) / 3; // 33% open space

    // fill map with walls
    for (uint16_t yy = 0; yy < MAP_H; yy++)
        for (uint16_t xx = 0; xx < MAP_W; xx++)
            SetMapTile(xx, yy, WALL_STONE);

    while (carved < target)
    {
        if (GetMapTile(x, y) == WALL_STONE)
        {
            SetMapTile(x, y, GROUND);
            carved++;
        }

        switch (rand() % 4)
        {
        case 0: if (x > 1) x--;
            break;
        case 1: if (x < MAP_W - 2) x++;
            break;
        case 2: if (y > 1) y--;
            break;
        case 3: if (y < MAP_H - 2) y++;
            break;
        }
    }

    DebugPrintMap();
}
