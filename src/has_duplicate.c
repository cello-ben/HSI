#include <assert.h>
#include <stdio.h>
#include "has_duplicate.h"

//From LeetCode #217
HSResult has_duplicate(int *nums, int nums_size)
{
    HS *hs = hs_init();
    if (hs == NULL)
    {
        return HS_MEMORY_ERR;
    }
    for (int i = 0; i < nums_size; i++)
    {
        if (hs_contains(hs, nums[i]))
        {
            return HS_TRUE;
        }
        hs_add(hs, nums[i]);
    }
    return HS_FALSE;
}

void test_has_duplicate(void)
{
    int nums[][7] = {
                    {1, 2, 7, -4, 7, 32, 0},
                    {1, 2, 3, 4, 5},
                    {-4, -4},
                    {0},
                    {-1},
                    {0, 1}
                };
    assert(has_duplicate(nums[0], 9) == HS_TRUE);
    assert(has_duplicate(nums[1], 5) == HS_FALSE);
    assert(has_duplicate(nums[2], 2) == HS_TRUE);
    assert(has_duplicate(nums[3], 1) == HS_FALSE);
    assert(has_duplicate(nums[4], 1) == HS_FALSE);
    assert(has_duplicate(nums[5], 2) == HS_FALSE);
}
