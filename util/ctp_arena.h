#ifndef CTP_ARENA_H
#define CTP_ARENA_H

#include <stddef.h>

struct ctp_arena {};
static inline struct ctp_arena ctp_arena_create();
static inline void* ctp_arena_alloc(struct ctp_arena *arena, size_t num_bytes);
static inline void ctp_arena_destroy(struct ctp_arena *arena);

#endif // CTP_ARENA_H
