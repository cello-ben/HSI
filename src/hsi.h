/*
MIT License


Copyright 2025 Benjamin Fryxell

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef HSI_H
#define HSI_H

#define HS_DEBUG

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef HS_DEBUG
    #include <stdarg.h> //TODO figure out if these should be indented or not per standard practice.
    #include <stdio.h>
#endif

#define HS_INITIAL_LENGTH 10000 //Right now, we're keeping this set at one value throughout. However, this can be a starting point if we later move to dynamically resizing.

typedef enum HSResult {
    HS_SUCCESS = 0,
    HS_FALSE = 0,
    HS_TRUE,
    HS_MEMORY_ERR,
    HS_NONMEMBER_ERR,
    HS_NULL_REFERENCE_ERR,
    HS_CAPACITY_ERR,
    HS_INIT_ERR,
    HS_OVERFLOW_ERR,
    #ifdef HS_DEBUG
        HS_DEBUG_PRINT_ERR
    #endif
} HSResult;

typedef struct ChainNode {
    int num;
    struct ChainNode *next;
} ChainNode;

typedef struct HS {
    size_t capacity;
    ChainNode **nodes;
} HS;

//TODO figure out if I need to check capacity in null checks.

//TODO add MingW support (Z instead of z).

//TODO figure out if we want "inline" or not.

//TODO add resizing based upon some maximum number of nodes per bucket.

/**
 * Takes in a format string and variadic arguments, but only does something with them if HS_DEBUG is defined (just returns 0 otherwise).
 */

static HSResult _hs_debug_printf(const char *fmt, ...)
{
    #ifdef HS_DEBUG //TODO figure out if null checks are necessary.
        if (fmt == NULL)
        {
            return HS_NULL_REFERENCE_ERR;
        }
        va_list args;
        va_start(args, fmt);
        int vprintf_res = vprintf(fmt, args);
        va_end(args);
        if (vprintf_res < 0) //vprintf returns a negative number if there is an error (positive error codes won't exist, since it returns the number of characters written, not including null terminator).
        {
            return HS_DEBUG_PRINT_ERR;
        }
        return HS_SUCCESS;
    #endif
    return HS_SUCCESS; //TODO figure out whether or not to return from the enum here, as opposed to just 0, since we're not actually doing any operations besides returning.
}

/**
 * Prints all occupied buckets.
 */

 static HSResult _hs_debug_print_all_occupied_buckets(HS *set)
 {
    #ifdef HS_DEBUG
        int printf_res; //TODO figure out whether I actually need to initialize this.
        if (set == NULL || set->nodes == NULL)
        {
            return HS_NULL_REFERENCE_ERR;
        }

        printf_res = printf("Printing all occupied buckets.\n");
        if (printf_res < 0)
        {
            return HS_DEBUG_PRINT_ERR;
        }

        for (size_t i = 0; i < set->capacity; i++)
        {
            if (set->nodes[i] != NULL)
            {
                printf_res = printf("%zu\n", i);
                if (printf_res < 0)
                {
                    return HS_DEBUG_PRINT_ERR;
                }
            }
        }
    #endif
    return HS_SUCCESS; //TODO figure out whether or not to return from the enum here, as opposed to just 0, since we're not actually doing any operations besides returning.
 }
 
/**
 * Prints the contents of all occupied buckets for debugging.
 */

 static HSResult _hs_debug_print_occupied_bucket_contents(HS *set)
 {
    #ifdef HS_DEBUG
        int num_elements = 0;
        if (set == NULL || set->nodes == NULL)
        {
            return HS_NULL_REFERENCE_ERR;
        }
        int printf_res = 0; //TODO figure out whether I actually need to initialize this.
        for (size_t i = 0; i < set->capacity; i++)
        {
            if (set->nodes[i] != NULL)
            {
                ChainNode *tmp = set->nodes[i];
                while (tmp != NULL)
                {
                    printf_res = printf("%d", tmp->num);
                    num_elements++;
                    if (printf_res < 0)
                    {
                        return HS_DEBUG_PRINT_ERR;
                    }
                    if (tmp->next)
                    {
                        printf_res = printf("->");
                        if (printf_res < 0)
                        {
                            return HS_DEBUG_PRINT_ERR;
                        }
                    }
                    else
                    {
                        printf_res = printf("\n");
                        if (printf_res < 0)
                        {
                            return HS_DEBUG_PRINT_ERR;
                        }
                    }
                    tmp = tmp->next;
                }
            }
        }
        printf_res = _hs_debug_printf("Total elements: %d\n", num_elements);
        if (printf_res < 0)
        {
            return HS_DEBUG_PRINT_ERR;
        }
    #endif
    return HS_SUCCESS; //TODO figure out whether or not to return from the enum here, as opposed to just 0, since we're not actually doing any operations besides returning.
 }

 /**
 * Prints the contents of a linked list of ChainNode structure pointers.
 */

 static HSResult _hs_debug_print_list(ChainNode *node)
 {
    #ifdef HS_DEBUG
    int printf_res;
        while (node)
        {
            printf_res = printf("%d ", node->num);
            if (printf_res < 0)
            {
                return HS_DEBUG_PRINT_ERR;
            }
            node = node->next;
        }
    printf_res = printf("\n\n");
    if (printf_res < 0)
    {
        return HS_DEBUG_PRINT_ERR;
    }
    #endif
    return HS_SUCCESS; //TODO figure out whether or not to return from the enum here, as opposed to just 0, since we're not actually doing any operations besides returning.
 }

/**
 * Simple hash function; this one was essentially just me playing around with random operations, but I'll look into a more robust one later.
 */

static size_t hash(int num)
{
    num &= 0xF31;
    num *= (9 >> 3);
    num ^= 12;
    num <<= 8;
    return abs(num % HS_INITIAL_LENGTH) - 1; //TODO figure out if this has to call "abs" or not (not sure if it's possible for a negative number to come out of the hashing logic).
}

/**
 * Initializes our hash set and its members. Returns null if there's any issue, and frees the set container if the nodes cannot be allocated.
 */

static HS *hs_init(void)
{
    HS *set = malloc(sizeof(HS));
    if (set == NULL)
    {
        return NULL;
    }
    set->capacity = HS_INITIAL_LENGTH;
    set->nodes = malloc(sizeof(ChainNode) * HS_INITIAL_LENGTH);
    if (set->nodes == NULL)
    {
        free(set); //We know it's not NULL, since if we have gotten to this point, the previous null check will have been passed.
        return NULL;
    }
    for (size_t i = 0; i < set->capacity; i++)
    {
        set->nodes[i] = NULL;
    }
    return set;
}

/**
 * Frees the hash set and all of its constituent components.
 */

static HSResult hs_free(HS *set)
{
    if (set == NULL)
    {
        return HS_NULL_REFERENCE_ERR;
    }
    if (set->nodes == NULL)
    {
        free(set);
        return HS_SUCCESS; //TODO figure out if we should return a different code if nodes were null.
    }
    for (size_t i = 0; i < set->capacity; i++)
    {
        ChainNode *tmp;
        while (set->nodes[i] != NULL)
        {
            tmp = set->nodes[i]->next;
            free(set->nodes[i]);
            set->nodes[i] = tmp;
        }
        free(set->nodes[i]);
    }
    free(set->nodes);
    free(set);
    return HS_SUCCESS;
}

/**
 * Adds an int to the given set.
 */

static HSResult hs_add(HS *set, long _num) //h/t to u/joshbadams (Reddit) for their debugging help. TODO find out if using a long is an idiomatic way to catch overflows.
{
    if (_num > INT_MAX || _num < INT_MIN)
    {
        return HS_OVERFLOW_ERR;
    }
    int num = (int)_num;
    if (set == NULL || set->nodes == NULL)
    {
        return HS_NULL_REFERENCE_ERR;
    }

    size_t idx = hash(num);

    if (set->capacity <= 0 || idx >= set->capacity)
    {
        return HS_CAPACITY_ERR;
    }
    
    ChainNode *node = malloc(sizeof(ChainNode));
    if (node == NULL)
    {
        return HS_MEMORY_ERR;
    }
    node->num = num;
    node->next = set->nodes[idx];
    set->nodes[idx] = node;
    return HS_SUCCESS;
}

/**
 * Checks if a given int is in a given set.
 */

static HSResult hs_contains(HS *set, int num)
{
    if (set == NULL || set->nodes == NULL)
    {
        return HS_NULL_REFERENCE_ERR;
    }
    size_t idx = hash(num);
    if (set->nodes[idx] == NULL)
    {
        return HS_FALSE;
    }
    ChainNode *node = set->nodes[idx];
    while (node != NULL)
    {
        if (node->num == num)
        {
            return HS_TRUE;
        }
        if (node->next != NULL)
        {
            node = node->next;
        }
        else
        {
            break;
        }
    }
    return HS_FALSE;
}

/**
 * Removes a given int from a given set, and returns an error if it is not present (may change later).
 */

static HSResult hs_delete(HS *set, int num) //TODO figure out why this isn't working as expected with README example. Another h/t to u/joshbadams (Reddit) for some great...pointers (haHAAAAAA!).
{
    size_t idx = hash(num);
    ChainNode *tmp, *prev;
    tmp = set->nodes[idx];
    prev = NULL;
    while (tmp)
    {
        if (tmp->num == num)
        {
            if (prev != NULL)
            {
                prev->next = tmp->next;
                free(tmp);
                return HS_SUCCESS;
            }
            else
            {
                set->nodes[idx] = set->nodes[idx]->next;
                free(tmp);
                return HS_SUCCESS;
            }
        }
        prev = tmp;
        tmp = tmp->next;
    }

    return HS_NONMEMBER_ERR; //TODO Figure out if we are checking all possible conditions before getting here, and whether or not we want to fail silently if there is nothing to delete.
}

#endif //HSI_H
