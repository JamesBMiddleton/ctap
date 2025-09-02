#include "ctp_guard.h"
#include "ctp_math.h"
#include "ctp_str.h"

#define NULL_TERMINATOR_SZ 1
#define UINT_MAX_CHARS (10) // '4294967295'
#define UINT_MAX_CHAR_THRESHOLD 1000000000
#define FLOAT_DECIMAL_CHARS (3)
#define NUMERIC_MAX_CHARS (1 + UINT_MAX_CHARS + 1 + FLOAT_DECIMAL_CHARS) // '-2147483648.123'

/*
 * If aligned copy 32bit chunks from dest to src, else copy bytes.
 * Assumes no overlap.
 *
 * @param dest - copy destination
 * @param src  - copy source
 * @param count - number of bytes to copy
 * @return copy destination
 */
inline const void* ctp_str_memcpy(void* dest, const void* src, size_t count)
{
    if (((size_t)src | (size_t)dest | count) & (sizeof(unsigned) - 1))
    {
        const unsigned char* src_byte = (const unsigned char*)src;
        unsigned char* dest_byte = (unsigned char*)dest;
        for (size_t i = 0; i < count; ++i)
            *(dest_byte++) = *(src_byte++);
    }
    else
    {
        const unsigned* src_word = (const unsigned*)src;
        unsigned* dest_word = (unsigned*)dest;
        for (size_t i = 0; i < count; i += sizeof(unsigned))
            *(dest_word++) = *(src_word++);
    }
    return dest;
}

/*
 * @param str - null terminated string.
 * @return length of str, null terminator not included.
 * */
inline size_t ctp_str_len(const char* str)
{
    size_t len = 0;
    while (*str++ != '\0')
        ++len;
    return len;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @return buf
 */
char* ctp_str_from_uint(unsigned val, char* buf)
{
    static const unsigned char base = 10;

    if (val >= UINT_MAX_CHAR_THRESHOLD)
        buf += UINT_MAX_CHARS - 1;
    else
        for (unsigned digits = base; digits <= val; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (char)('0' + (val % base));
        val /= base;
    } while (val != 0);
    return buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @return buf
 */
char* ctp_str_from_int(int val, char* buf)
{
    static const unsigned char base = 10;

    unsigned abs = (unsigned)val;
    if (val < 0)
    {
        abs = -(unsigned)val;
        *buf++ = '-';
    }
    if (abs >= UINT_MAX_CHAR_THRESHOLD)
        buf += UINT_MAX_CHARS - 1;
    else
        for (unsigned digits = base; digits <= abs; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (char)('0' + (abs % base));
        abs /= base;
    } while (abs != 0);
    return (val < 0) ? --buf : buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 * We're checking for inf/nan before float casts, so silencing UB sanitizer.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @param decimals - number of decimal places
 * @return buf
 */
__attribute__((no_sanitize("undefined"))) char* ctp_str_from_float(const float val, char* buf,
                                                                   unsigned char decimals)
{
    static const float base = 10;

    if (ctp_math_isnan(val))
    {
        static const char* nan = "NaN";
        ctp_str_memcpy(buf, nan, ctp_str_len(nan) + NULL_TERMINATOR_SZ);
        return buf;
    }

    if (ctp_math_isinf(val))
    {
        static const char* inf = "Inf";
        ctp_str_memcpy(buf, inf, ctp_str_len(inf) + NULL_TERMINATOR_SZ);
        return buf;
    }

    int whole = (int)val;
    float fraction = ctp_math_fabs((val - (float)whole));
    char* start = ctp_str_from_int(whole, buf);

    buf += ctp_str_len(buf);
    *buf++ = '.';

    if (decimals--)
    {
        fraction *= base;
        while ((int)fraction == 0 && decimals--)
        {
            *buf++ = '0';
            fraction *= base;
        }
        while (decimals--)
            fraction *= base;
        ctp_str_from_uint((unsigned)fraction, buf);
    }
    else
        *buf = '\0';
    return start;
}

/*
 * Takes format specifiers of type %d, %u, %f, %s or %c.
 * e.g. util_sprintf(buf, bufsz, "%f", (util_Fmts){.arr = {{.f = 123.456f}}})
 *
 * @param buf - the destination string buffer
 * @param bufsz - size of destination string buffer
 * @param format - the string format
 * @param vals - struct wrapped array of format specifier values
 * @return - the destination string buffer
 */
const char* ctp_str_printf(char* buf, size_t bufsz,
                                const char* format, struct ctp_str_printf_va_list va_list)
{
    const char* first = buf;
    const char* last = buf + bufsz - 1;
    size_t i_vals = 0;

    while (*format != '\0')
    {
        if (buf == last)
        {
            util_LOG_ERROR("Destination buffer too small.");
            break;
        }

        if (*format != '%')
        {
            *buf++ = *format++;
        }
        else
        {
            if (i_vals == util_MAX_FMT_ARGS)
            {
                util_LOG_ERROR("Too many specifiers in format string.");
                break;
            }

            ++format;
            const char specifier = *format++;
            const util_Fmt val = vals.arr[i_vals++];

            if (((specifier == 'd' || specifier == 'u' || specifier == 'f') &&
                 (buf + NUMERIC_MAX_CHARS) > last) ||
                ((specifier == 's') && (buf + util_Strlen(val.s) > last)))
            {
                util_LOG_ERROR("Destination buffer too small.");
                break;
            }

            switch (specifier)
            {
                case 'c': *buf++ = val.c; continue;
                case 'd': util_Inttostr(val.d, buf); break;
                case 'u': util_Uinttostr(val.u, buf); break;
                case 'f': util_Floattostr(val.f, buf, 3); break;
                case 's': {
                    const char* str = val.s;
                    do
                    {
                        *buf = *str;
                    } while (*str++ != '\0' && ++buf);
                    break;
                }
                default:
                    util_LOG_ERROR("Invalid specifier used.");
                    *buf = '\0';
                    return first;
            }
            buf += util_Strlen(buf);
        }
    }
    *buf = '\0';
    return first;
}
