#include "hsi.h"
#include "has_duplicate.h"

int main(void)
{
    //Let's take this hash set implementation for a test drive using some LeetCode problems.
    test_has_duplicate();

    return printf("All tests passed!\n") >= 0 ? 0 : -1;
}
