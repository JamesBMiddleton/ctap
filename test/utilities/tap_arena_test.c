#include "util/tap_arena.h"
#include <assert.h>

typedef struct {
    int i;
    short j;
    long k;
    char c;
} ArenaStructTest;

static void tap_arena_alloc_create_test(void)
{
    TapArena arena = tap_arena_create(42);
    assert(arena.capacity == 42);
    assert(arena.data != NULL);
    assert(arena.head != NULL);
    tap_arena_destroy(&arena);
}

static void tap_arena_alloc_aligned_test(void)
{
    TapArena arena = {0};
    TapArena *arena_ptr = NULL;

    /* Unitialized arenas cannot be allocated from. */
    assert(tap_arena_alloc_aligned(&arena, 42, 42, 42) == NULL);
    assert(tap_arena_alloc_aligned(arena_ptr, 42, 42, 42) == NULL);

    /* Zero capacity arenas are valid, but can't be allocated from. */
    arena = tap_arena_create(0);
    assert(tap_arena_alloc_aligned(&arena, 0, sizeof(int), 42) == NULL);
    tap_arena_destroy(&arena);

    /* Standard allocation. */
    arena = tap_arena_create(4096);
    assert(tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 42) != NULL);
    tap_arena_destroy(&arena);

    /* No overflow if capacity if sufficient for allocation. */
    arena = tap_arena_create(sizeof(int));
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 1);
    assert(arena.overflow == NULL);
    tap_arena_destroy(&arena);

    /* We overflow correctly to maintain correct alignment of allocated types. */
    arena = tap_arena_create(sizeof(int) + sizeof(char) + sizeof(int));
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 1);
    tap_arena_alloc_aligned(&arena, tap_alignof(char), sizeof(char), 1);
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 1);
    assert(arena.overflow != NULL);
    tap_arena_destroy(&arena);

    /* No overflow if initial capacity accounts for alignment requirements. */
    arena = tap_arena_create(sizeof(int) + sizeof(char) + (tap_alignof(int) - sizeof(char)) + sizeof(int));
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 1);
    tap_arena_alloc_aligned(&arena, tap_alignof(char), sizeof(char), 1);
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 1);
    assert(arena.overflow == NULL);
    tap_arena_destroy(&arena);

    /* Overflow capacities increase predictably. */
    arena = tap_arena_create(sizeof(int));
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 1);
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 2);
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 4);
    tap_arena_alloc_aligned(&arena, tap_alignof(int), sizeof(int), 8);
    assert(arena.overflow != NULL);
    assert(arena.overflow->overflow != NULL);
    assert(arena.overflow->overflow->overflow != NULL);
    assert(arena.overflow->overflow->overflow->overflow == NULL);
    tap_arena_destroy(&arena);

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

static void tap_arena_alloc_destroy_test(void)
{
    TapArena arena = tap_arena_create(10);
    tap_arena_alloc(&arena, char, 11);
    assert(arena.data != NULL);
    assert(arena.overflow != NULL);
    tap_arena_destroy(&arena);
    assert(arena.data == NULL);
    assert(arena.overflow == NULL);
}

int main(void)
{
    tap_arena_alloc_create_test();
    #ifndef DEBUG /* Debug mode changes arena allocation behaviour for sanitizer support, invaliding this test. */
    tap_arena_alloc_aligned_test();
    #endif
    tap_arena_alloc_test();
    tap_arena_alloc_destroy_test();
    return 0;
}
