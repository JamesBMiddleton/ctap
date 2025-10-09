#ifndef TAP_ALIGNOF_H
#define TAP_ALIGNOF_H

#include <stddef.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define tap_alignof(type) alignof(type)
#else
    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        #include <stdalign.h>
        #define tap_alignof(type) _Alignof(type)
    #else
        #if defined(__clang__) || defined(__GNUC__)
            #define tap_alignof(type) __alignof__(type)
        #else
            #if defined(__MSC_VER)
                #define tap_alignof(type) __alignof(type)
            #else
                /* fallback to aligning on the largest primitive type in use */
                #define tap_alignof(type) sizeof(union { void *p; long l; double d; }) 
            #endif
        #endif
    #endif
#endif

#endif /* TAP_ALIGNOF_H */
