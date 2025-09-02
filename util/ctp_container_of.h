#ifndef CTP_CONTAINER_OF_H
#define CTP_CONTAINER_OF_H

#if defined(__clang__) || defined(__GNUC__)
#define ctp_container_of(ptr, type, member)                                 \
    ((const type*)((const unsigned char*)(__typeof__(((const type*)0)       \
                                                         ->member)*){ptr} - \
                   offsetof(type, member)))
#else
#define ctp_container_of(ptr, type, member)      \
    ((const type*)((const unsigned char*)(ptr) - \
                   ((size_t) & (((type*)0)->member))))
#endif

#endif // CTP_CONTAINER_OF_H
