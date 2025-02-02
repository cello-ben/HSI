#include <stdio.h>
#include "hsi.h"

int main(void)
{
    HS *set = hs_init();
    if (set == NULL)
    {
        perror("Error: set is NULL.\n");
    }
    hs_add(set, 42);
    hs_add(set, 19);
    hs_add(set, -47);
    _hs_debug_printf("The set %s %d.\n", hs_contains(set, 19) ? "contains" : "does not contain", 19);
    //Will print "contains".

    hs_delete(set, 19);

    _hs_debug_printf("The set %s %d.\n", hs_contains(set, 19) ? "contains" : "does not contain", 19);
    //Will print "does not contain" (ONCE BUG IS FIXED!).
    
    hs_free(set);
    return 0;
}