#ifndef PRIMITIVES_C
#define PRIMITIVES_C
PRIMITIVES_C

#include <stdbool.h>
#include <stddef.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef size_t usize;

typedef struct {
    const char* const chars;
    const usize len;
} str;

// vec2
// vec3

// #define tostr(cstr) ((str) { .len = (sizeof("" cstr "")-1), .chars = (cstr) })
//
// static inline str substr(str src, const usize start, const usize len)
// {
//     return (str){.chars = src.chars + start, .len = len};
// }

#endif // PRIMITIVES_C
