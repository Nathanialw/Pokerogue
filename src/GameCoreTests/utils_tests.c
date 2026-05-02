//
// Created by nathanial on 4/16/26.
//

#include "utils.h"
#include "utils_tests.h"

#include "lib_debugging.h"
#include "types.h"

#define n 10



void SortEntityArray_Test()
{
    EntityId sorted[n];
    EntityId unsorted[n] = {12, 56, 3, 185, NO_ENTITY, 64, 65,78,99, 123};

    SortEntityArray(sorted, unsorted, n);

    bool success = true;
    for (uint8_t i = 0; i < n-1; i++)
    {
        EntityId n1 = sorted[i];
        EntityId n2 = sorted[i + 1];

        if (n1 > n2)
        {
            success = false;
            break;
        }
    }

    if (success)
    {
        DEBUG("SortEntityArray_Test - SUCCESS");
    }
    else
    {
        DEBUG("SortEntityArray_Test - FAILED");
    }

}