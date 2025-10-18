#ifndef TAP_ALLOC_H
#define TAP_ALLOC_H

#include <stddef.h>

typedef struct {
    void *(*malloc) (size_t size);
    void (*free) (void *ptr);
} TapAllocAllocator;
static void tap_alloc_set_allocator(TapAllocAllocator allocator);

static void *tap_alloc_malloc(size_t size);
static void tap_alloc_free(void *ptr);

#ifdef TAP_ALLOC_IMPLEMENTATION
static TapAllocAllocator alloc_allocator;
void tap_alloc_set_allocator(TapAllocAllocator allocator) { alloc_allocator = allocator; }
void *tap_alloc_malloc(size_t size) { return alloc_allocator.malloc(size); }
void tap_alloc_free(void *ptr) { alloc_allocator.free(ptr); }
#endif /* TAP_ALLOC_IMPLEMENTATION */

#endif /* TAP_ALLOC_H */
