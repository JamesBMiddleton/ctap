#include "util/tap_arena.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    int i;
    short j;
    long k;
    char c;
} ArenaStructTest;

static void tap_arena_alloc_aligned_test(void)
{
    /* create specific size, alloc the size, check no overflow */
    /* ... */

}

static void tap_arena_alloc_create_test(void)
{
}

static void tap_arena_alloc_destroy_test(void)
{
}

static void tap_arena_alloc_test(void)
{
    int *integer = NULL;
    ArenaStructTest *structure = NULL;
    char *array = NULL;
    TapArena arena = tap_arena_create(4096);

    integer = tap_arena_alloc(&arena, int, 1);
    assert(integer != NULL);
    structure = tap_arena_alloc(&arena, ArenaStructTest, 1);
    assert(structure != NULL);
    array = tap_arena_alloc(&arena, char, 10);
    assert(array != NULL);
    array[9] = 42;

    tap_arena_destroy(&arena);
}

int main(void)
{
    tap_arena_alloc_create_test();
    tap_arena_alloc_aligned_test();
    tap_arena_alloc_test();
    tap_arena_alloc_destroy_test();
    return 0;
}
