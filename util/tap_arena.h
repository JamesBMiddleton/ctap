#ifndef TAP_ARENA_H
#define TAP_ARENA_H

#include <stddef.h>

typedef struct {} TapArena; 
static TapArena tap_arena_create();
static void* tap_arena_alloc(TapArena *arena, size_t num_bytes);
static void tap_arena_destroy(TapArena *arena);

#endif /* TAP_ARENA_H */
