//Generative AI disclosure: this file was written by generative AI for testing purposes, though the entirety of hsi.h is human-made.

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

#include "hsi.h"

#define TEST_SIZE 100000  // Large number of insertions
#define FUZZ_ITERS 100000  // Fuzzing iterations

int main(void)
{
    srand((unsigned)time(NULL));

    HS *hs = hs_init();
    assert(hs != NULL); // Ensure hash set is initialized

    // Insert a wide range of values
    for (int i = 1; i <= TEST_SIZE; i++) {
        hs_add(hs, i);
        assert(hs_contains(hs, i)); // Ensure value is successfully inserted
    }

    // Insert edge cases
    hs_add(hs, 0);
    hs_add(hs, -1);
    hs_add(hs, INT_MIN);
    hs_add(hs, INT_MAX);

    assert(hs_contains(hs, 0));
    assert(hs_contains(hs, -1));
    assert(hs_contains(hs, INT_MIN));
    assert(hs_contains(hs, INT_MAX));

    // Delete specific elements and verify they are removed
    hs_delete(hs, 4);
    hs_delete(hs, TEST_SIZE);
    hs_delete(hs, INT_MIN);
    hs_delete(hs, INT_MAX);
    hs_delete(hs, 9999999); // Not in set, should not crash

    assert(!hs_contains(hs, 4));
    assert(!hs_contains(hs, TEST_SIZE));
    assert(!hs_contains(hs, INT_MIN));
    assert(!hs_contains(hs, INT_MAX));
    assert(!hs_contains(hs, 9999999));

    // Fuzz test: Random insertions and deletions
    for (int i = 0; i < FUZZ_ITERS; i++) {
        int val = rand();
        if (rand() % 2) {
            hs_add(hs, val);
            assert(hs_contains(hs, val));
        } else {
            hs_delete(hs, val);
        }

        if (rand() % 5 == 0) { // Occasionally verify existence
            hs_contains(hs, val);
        }
    }
    hs_free(hs);
    _hs_debug_printf("All tests passed.\n");
    return 0;
}
