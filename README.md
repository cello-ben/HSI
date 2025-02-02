# HSI
Hash sets are tremendously useful data structures, and it's easy to take them for granted in languages like Python or C#. After all, we don't usually need to be overly concerned about exactly how they are implemented. However, I figured a great exercise would be to create my own in pure C. This implementation uses chaining to mitigate hash collisions. It is a work in progress that is not yet production-ready, but I am developing it actively.

## Example Usage
```
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
    //Will print "does not contain"
    
    hs_free(set);
    return 0;
}
```

## Generative AI Disclosure
Generative AI was not used in the creation of hsi.h. It was created by hand with occasional community help with debugging. driver.c WAS initially created by generative AI as a time-saving measure, just to test the implementation. Thereafter, I removed that implementation.