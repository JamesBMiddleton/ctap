#ifndef TAP_DEF_H
#define TAP_DEF_H

#include <stddef.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #ifndef tap_def_alignof
        #define tap_def_alignof(type) alignof(type)
    #endif
    #ifndef tap_def_typeof
        #define tap_def_typeof(type) typeof(type)
    #endif
    #ifndef tap_def_sametype
        #define tap_def_sametype(a, b) _Generic((tap_def_typeof(a) *)0, tap_def_typeof(b) *: 1, default: 0)
    #endif
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #ifndef tap_def_alignof
        #include <stdalign.h>
        #define tap_def_alignof(type) _Alignof(type)
    #endif
#endif

#if defined(__clang__) || defined(__GNUC__)
    #ifndef tap_def_alignof
        #define tap_def_alignof(type) __alignof__(type)
    #endif
    #ifndef tap_def_typeof
        #define tap_def_typeof(type) __typeof__(type)
    #endif
    #ifndef tap_def_sametype
        #define tap_def_sametype(a, b) __builtin_types_compatible_p(tap_def_typeof(a), tap_def_typeof(b))
    #endif
#endif

#if defined(__MSC_VER)
    #ifndef tap_def_alignof
        #define tap_def_alignof(type) __alignof(type)
    #endif
#endif

#ifndef tap_def_alignof
    /* fallback to aligning on the largest primitive type in use */
    #define tap_def_alignof(type) \
        sizeof(union {            \
            void *p;              \
            long l;               \
            double d;             \
        })
#endif
#ifndef tap_def_typeof
    #define tap_def_typeof(type) (void)
#endif
#ifndef tap_def_sametype
    #define tap_def_sametype(a, b) (1)
#endif
#ifndef tap_def_containerof
    #define tap_def_containerof(ptr, type, member)                                        \
        ((type *)(void *)((unsigned char *)(ptr) - offsetof(type, member))); /* NOLINT */ \
        tap_def_static_assert(tap_def_sametype(((type *)0)->member, *(ptr)), ptr_member_type_mismatch)
#endif
#ifndef tap_def_static_assert
    #define tap_def_static_assert(cond, msg) typedef char tap_def_static_assert_##msg[(cond) ? 1 : -1]
#endif

#endif /* TAP_DEF_H */
