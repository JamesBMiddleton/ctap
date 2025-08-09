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

// #define tostr(cstr) ((str) { .len = (sizeof("" cstr "")-1), .chars = (cstr) })
//
// static inline str substr(str src, const usize start, const usize len)
// {
//     return (str){.chars = src.chars + start, .len = len};
// }

#ifdef UTEST_PRIMITIVES
#ifdef UTEST
int main(void)
{
    return 0;
}
#endif // UTEST
#endif // UTEST_PRIMITIVES
#endif // PRIMITIVES_C
