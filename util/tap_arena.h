#ifndef TAP_ARENA_H
#define TAP_ARENA_H

#include "tap_alignof.h"

typedef struct {} TapArena; 
static TapArena tap_arena_create(size_t initial_bytes);
static void *tap_arena_alloc_raw(TapArena *arena, size_t size, size_t align, size_t count);
static void tap_arena_destroy(TapArena *arena);

#define tap_arena_alloc(arena, type, count) (type *)tap_arena_alloc_raw(arena, sizeof(type), tap_alignof(type), count)

#endif /* TAP_ARENA_H */
