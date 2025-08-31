#ifndef CTP_ARENA_H
#define CTP_ARENA_H

#include "ctp_guard.h"

struct ctp_arena {};
static inline ctp_result ctp_arena_create(struct ctp_arena *arena);
static inline ctp_result ctp_arena_alloc(struct ctp_arena *arena, size_t num_bytes, void *out);
static inline ctp_result ctp_arena_destroy(struct ctp_arena *arena);

#endif // CTP_ARENA_H
