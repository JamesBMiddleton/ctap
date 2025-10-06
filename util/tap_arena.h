#ifndef TAP_ARENA_H
#define TAP_ARENA_H

#include "tap_alignof.h"
#include <stdlib.h>

typedef struct TapArena_ {
    size_t capacity;
    unsigned char *data;
    unsigned char *head;
    struct TapArena_ *overflow;
} TapArena; 

static TapArena tap_arena_create(size_t initial_capacity);
static void *tap_arena_alloc_unsafe(TapArena *arena, size_t size, size_t align, size_t count);
static void tap_arena_destroy(TapArena *arena);

#define tap_arena_alloc(arena, type, count) (type *)tap_arena_alloc_raw(arena, sizeof(type), tap_alignof(type), count)

TapArena arena_create(const size_t initial_capacity)
{
    TapArena arena = {0};
    arena.capacity = initial_capacity;
    #ifdef DEBUG
        arena.data = (unsigned char *)malloc(1);
        arena.head = arena.data; /* ensure overflow on first allocation */
    #else
        arena.data = (unsigned char *)malloc(initial_capacity);
        arena.head = arena.data + initial_capacity;
    #endif
    return arena;
}

static void *tap_arena_alloc_unsafe(TapArena *arena, const size_t size, const size_t align, const size_t count)
{
    unsigned int offset = (size_t)arena->head % align;
    if (size == 0 || arena->data == NULL || arena->head == NULL)
        return NULL;
    if (arena->data >= (arena->head - ((size * count) + offset))) /* this alignment check is wrong */
    {
        /* swap to next TapArena, put this one in 'overflow' */
        #ifdef DEBUG
            /* malloc(size) */
        #else
            /* malloc(next->capacity * 2) */
        #endif
    }
    arena->head -= size + offset; /* this alignment addition is wrong */
    return arena->head;
}

static void tap_arena_destroy(TapArena *arena)
{
    TapArena *tmp = {0};

    if (arena == NULL)
        return;

    do {
        tmp = arena;
        arena = arena->overflow;
        free(tmp->data);
        free(tmp); /* this is wrong on first loop, still pointing to function param !!! */
    } while (arena->overflow != NULL);
}

#endif /* TAP_ARENA_H */
