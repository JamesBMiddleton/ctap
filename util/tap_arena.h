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
static void *tap_arena_alloc_aligned(TapArena *arena, size_t align, size_t size, size_t count);
static void tap_arena_destroy(TapArena *arena);

#define tap_arena_alloc(arena, type, count) (type *)tap_arena_alloc_aligned(arena, tap_alignof(type), sizeof(type), count)

TapArena tap_arena_create(const size_t initial_capacity)
{
    TapArena arena = {0};
    arena.capacity = initial_capacity;
    #ifdef DEBUG
        arena.data = (unsigned char *)malloc(1);
        arena.head = arena.data; /* ensure overflow on first allocation */
    #else
        arena.data = (unsigned char *)malloc(arena.capacity);
        arena.head = arena.data + arena.capacity;
    #endif
    return arena;
}

static void *tap_arena_alloc_aligned(TapArena *arena, const size_t align, const size_t size, const size_t count)
{
    unsigned char *new_head;
    TapArena *overflow;
    
    if (size == 0 || count == 0 || arena == NULL || arena->data == NULL || arena->head == NULL || arena->capacity == 0)
        return NULL;

    new_head = arena->head - (size * count);
    new_head -= (size_t)new_head % align;
    if (arena->data > new_head)
    {
        overflow = (TapArena *)malloc(sizeof(TapArena));
        *overflow = *arena;
        arena->overflow = overflow;
        #ifdef DEBUG
            arena->capacity = size * count;
        #else
            arena->capacity *= 2;
        #endif
        arena->data = (unsigned char *)malloc(arena->capacity);
        arena->head = arena->data + arena->capacity;
        return tap_arena_alloc_aligned(arena, size, align, count);
    }
    arena->head = new_head;
    return arena->head;
}

static void tap_arena_destroy(TapArena *arena)
{
    TapArena *tmp = arena;

    if (arena == NULL)
        return;

    arena = arena->overflow;
    free(tmp->data);
    tmp->data = NULL;
    tmp->overflow = NULL;
    while (arena != NULL)
    {
        tmp = arena;
        arena = arena->overflow;
        free(tmp->data);
        tmp->data = NULL;
        tmp->overflow = NULL;
        free(tmp);
    } 
}

#endif /* TAP_ARENA_H */
