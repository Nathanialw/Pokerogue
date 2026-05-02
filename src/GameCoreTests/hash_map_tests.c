//
// Created by nathanial on 4/12/26.
//

#include "hash_map_tests.h"

#include "hash_map.h"
#include "lib_debugging.h"
#include "types.h"
#include "utils.h"

void TEST_HashMap()
{
    CompactHashMap h;
    HashmapInit(&h);

    for (uint8_t i = 0; i < ENTITY_COUNT; i++)
    {
        uint8_t x = GetRandom_uint8_t(0, 255);
        uint8_t y = GetRandom_uint8_t(0, 255);
        EntityId e = i;
        bool b = HashmapInsert(&h, x, y, e);
        ASSERT(b, "HashmapInsert failed %d %d", x, y);
        EntityId e2 = HashmapGet(&h, x, y);
        ASSERT(e2 == e, "HashmapGet failed %d %d %d %d", x, y, e, e2);
        bool b2 = HashmapRemove(&h, x, y);
        ASSERT(b2, "HashmapRemove failed %d %d", x, y);
    }
}
