#ifndef TAP_CONTAINEROF_H
#define TAP_CONTAINEROF_H

#if defined(__clang__) || defined(__GNUC__)
#define tap_container_of(ptr, type, member)                                 \
    ((const type*)((const unsigned char*)(__typeof__(((const type*)0)       \
                                                         ->member)*){ptr} - \
                   offsetof(type, member)))
#else
#define tap_container_of(ptr, type, member)      \
    ((const type*)((const unsigned char*)(ptr) - \
                   ((size_t) & (((type*)0)->member))))
#endif

#endif /* TAP_CONTAINEROF_H */
