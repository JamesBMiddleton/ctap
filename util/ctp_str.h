#ifndef CTP_STR_H
#define CTP_STR_H

#include <stddef.h>

struct ctp_str_printf_va_list {
    union {
        const char* s;
        char c;
        int d;
        unsigned u;
        float f;
    } elems[5];
};

static inline const void* ctp_str_memcpy(void* dest, const void* src, size_t count);
static inline const void* ctp_str_memset(void);
static inline const void* ctp_str_memmove(void);
static inline size_t ctp_str_len(const char* str);
static char* ctp_str_from_uint(unsigned val, char* buf);
static char* ctp_str_from_int(int val, char* buf);
static char* ctp_str_from_float(float val, char* buf, unsigned char decimals);
static const char* ctp_str_printf(char* buf, size_t bufsz,
                                const char* format, struct ctp_str_printf_va_list va_list);

#define CTP_STR_FROM_(s) #s
#define CTP_STR_FROM(s) CTP_STR_FROM_(s)

#endif // CTP_STR_H
