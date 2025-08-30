#ifndef PRIMITIVES_C
#define PRIMITIVES_C
PRIMITIVES_C


// ----------- freestanding compliant / header-only libc includes -------------

#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ------------------ s2n-tls style result error handling pattern ------------
#define CTP_SUCCESS 0
#define CTP_FAILURE (-1)
typedef struct {
    int retcode;
} ctp_result;

#if defined(__clang__) || defined(__GNUC__)
    #define CTP_RESULT __attribute__((warn_unused_result)) ctp_result
#else
    #define CTP_RESULT ctp_result
#endif
// #define CTP_RESULT_OK
// #define CTP_RESULT_ERROR
// #define RESULT_ENSURE()
// #define RESULT_ENSURE_PTR_VALID()
// #define RESULT_BAIL()
// ...


// --------------- linux kernel 'safe' instrusive containers pattern ----------

#if defined(__clang__) || defined(__GNUC__)
#define container_of_const(ptr, type, member)                                  \
    ((const type*)((const char*)(__typeof__(((const type*)0)->member)*){ptr} - \
                   ((size_t) & (((type*)0)->member))))
#else
#define container_of_const(ptr, type, member) \
    ((const type*)((const char*)(ptr) - ((size_t) & (((type*)0)->member))))
#endif
// struct ctp_list_head {};
// ctp_list_init()
// ctp_list_move()
// ctp_list_swap()
// ctp_list_for_each()
// ...

// ------------------ arena allocation pattern ------------------------

// struct ctp_arena {};
// ctp_arena_create()
// ctp_arena_alloc()
// ctp_arena_destroy()

// ---------------- sokol inspired per module centralised data ownership -------

// static struct { ctp_pool all_the_data; } ctp_audio_state;

// ------------------ primitive types ---------------------------------

// vec2
// vec3

// typedef struct {
//     const char* const chars;
//     const size_t len;
// } str;

// #define tostr(cstr) ((str) { .len = (sizeof("" cstr "")-1), .chars = (cstr) })
//
// static inline str substr(str src, const usize start, const usize len)
// {
//     return (str){.chars = src.chars + start, .len = len};
// }

#endif // PRIMITIVES_C
