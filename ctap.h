/*
    Author: James Middleton
    Repo: https://github.com/JamesBMiddleton/ctap
    License: MIT
    Docs: EOF
*/

// #define ctp_IMPL

#include <stdio.h>
typedef float f32;
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef signed char i8;
typedef signed short int i16;
typedef signed int i32;
#define bool _Bool
#define true 1
#define false 0
#define NULL ((void*)0)
typedef __SIZE_TYPE__ usize; //! compiler dependent
// typedef typeof(sizeof(0)) usize; // C23

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CTAP API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define MAX_LOG_SZ 256

typedef enum {
    ctp_loglvl_ASSERT,
    ctp_loglvl_DEBUG,
    ctp_loglvl_WARN,
    ctp_loglvl_ERROR,
    ctp_loglvl_PANIC
} ctp_loglvl_e;

typedef struct {
    const ctp_loglvl_e lvl;
    const u32 line_num;
    const char* func_name;
    char message[MAX_LOG_SZ];
} ctp_log_t;

typedef enum { ctp_retcode_MAP_INVALID } ctp_retcode_e;

ctp_retcode_e ctp_load_map(void);

static void (*ctp_log_cb)(const ctp_log_t log);
static void (*ctp_panic_cb)(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CORE API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum { cor_retcode_MAP_INVALID } cor_retcode_e;

static cor_retcode_e cor_start_the_engines(void);

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// GRAPHICS API ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PHYSICS API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// INPUT API //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// AUDIO API //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UTILS API //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define MAX_FMT_ARGS 5

typedef union {
    const char* s;
    const i32 d;
    const u32 u;
    const f32 f;
} utl_fmt_u;

typedef struct {
    utl_fmt_u arr[MAX_FMT_ARGS];
} utl_fmts_t;

static char* utl_sprintf(char* buf, const usize bufsz, const char* format,
                         const utl_fmts_t values);

static void* utl_memcpy(void* dest, const void* src, const usize count);

static inline usize utl_strlen(const char* s);

static inline i32 utl_powi(i32 x, u32 y);
static inline u32 utl_powu(u32 x, u32 y);
static inline f32 utl_powf(f32 x, i32 y);

static inline i32 utl_abs(i32 x);
static inline f32 utl_fabs(f32 x);

static inline bool utl_isnan(f32 x);
static inline bool utl_isinf(f32 x);

static char* utl_u32tostr(u32 value, char* buf);
static char* utl_i32tostr(i32 value, char* buf);
static char* utl_f32tostr(f32 value, char* buf, u8 decimals);

/* MACROS 
 *
 * LOG_D(format, ...)
 * LOG_W(format, ...)
 * LOG_E(format, ...)
 * PANIC(format, ...)
 * ASSIGN_IF_ZERO(val, def)
 * ASSERT(cond)
 * STATIC_ASSERT(cond, msg)
 *
 * */

////////////////////////////////////////////////////////////////////////////////
//////////////////////// UTILS IMPLEMENTATION //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static ctp_log_t new_log(ctp_loglvl_e lvl, u32 line_num, const char* func_name,
                         const char* format, const utl_fmts_t values)
{
    ctp_log_t log = {.lvl = lvl, .line_num = line_num, .func_name = func_name};
    utl_sprintf(log.message, sizeof(log.message), format, values);
    usize msg_len = utl_strlen(log.message);
    log.message[msg_len] = '\n';
    log.message[msg_len+1] = '\0';
    return log;
}

#ifdef DEBUG
#define LOG_D(format, ...)                                                   \
    do                                                                       \
    {                                                                        \
        if (ctp_log_cb)                                                      \
            ctp_log_cb(new_log(ctp_loglvl_DEBUG, __LINE__, __func__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));         \
    } while (0)
#else
#define LOG_D(...)
#endif

#define LOG_W(format, ...)                                                  \
    do                                                                      \
    {                                                                       \
        if (ctp_log_cb)                                                     \
            ctp_log_cb(new_log(ctp_loglvl_WARN, __LINE__, __func__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));        \
    } while (0)

#define LOG_E(format, ...)                                                   \
    do                                                                       \
    {                                                                        \
        if (ctp_log_cb)                                                      \
            ctp_log_cb(new_log(ctp_loglvl_ERROR, __LINE__, __func__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));         \
    } while (0)

#define PANIC(format, ...)                                                   \
    do                                                                       \
    {                                                                        \
        if (ctp_log_cb)                                                      \
            ctp_log_cb(new_log(ctp_loglvl_PANIC, __LINE__, __func__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));         \
        ctp_panic_cb();                                                      \
    } while (0)

#ifdef DEBUG
#define ASSERT(cond)                                                      \
    do                                                                    \
    {                                                                     \
        if (!(cond))                                                      \
        {                                                                 \
            if (ctp_log_cb)                                               \
                ctp_log_cb(new_log(ctp_loglvl_ASSERT, __LINE__, __func__, \
                                   "%s",                                  \
                                   (utl_fmts_t){.arr = {{.s = #cond}}})); \
            ctp_panic_cb();                                               \
        }                                                                 \
    } while (0)
#else
#define ASSERT(...)
#endif

#define ASSIGN_IF_ZERO(val, def)               \
    do                                         \
    {                                          \
        ((val) = ((val) == 0) ? (def) : (val)) \
    } while (0)

#define STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

/*
 * If aligned copy 32bit chunks from dest to src, else copy bytes. 
 * Assumes no overlap. Redundant void cast to silence -Wcast-align
 *
 * @param dest - copy destination 
 * @param src  - copy source
 * @param count - number of bytes to copy
 * @return copy destinatiion
*/
static void* utl_memcpy(void* dest, const void* src, const usize count)
{
    ASSERT(dest);
    ASSERT(src);
    if (((usize)src | (usize)dest | count) & sizeof(u32) - 1)
    {
        const u8* src_byte = (const u8*)src;
        const u8* end_byte = src_byte + count;
        u8* dest_byte = (u8*)dest;
        while (src_byte != end_byte)
            *(dest_byte++) = *(src_byte++);
    }
    else
    {
        const u32* src_word = (const u32*)src;
        u32* dest_word = (u32*)dest;
        const u32* end_word =
            (const u32*)((const void*)((const u8*)src_word + count));
        while (src_word != end_word)
            *(dest_word++) = *(src_word++);
    }
    return dest;
}

/* Raise x to the power of y. */
static inline i32 utl_powi(i32 x, u32 y)
{
    i32 z = 1;
    while (true)
    {
        if (y & 1)
            z *= x;
        y >>= 1;
        if (!y)
            break;
        x *= x;
    }
    return z;
}

/* Raise x to the power of y. */
static inline u32 utl_powu(u32 x, u32 y)
{
    u32 z = 1;
    while (true)
    {
        if (y & 1)
            z *= x;
        y >>= 1;
        if (!y)
            break;
        x *= x;
    }
    return z;
}

/* Raise x to the power of y. Can handle negative powers. */
static inline f32 utl_powf(f32 x, i32 y)
{
    if (y == 0)
        return 1;
    f32 z = utl_powf(x, y / 2);
    if ((y % 2) == 0)
        return z * z;
    else if (y > 0)
        return x * z * z;
    else
        return (z * z) / x;
}

/* Absolute integer value */
static inline i32 utl_abs(i32 x)
{
    return (x < 0) ? -x : x;
}

/* Absolute float value */
static inline f32 utl_fabs(f32 x)
{
    return (x < 0) ? -x : x;
}

/* Requires IEEE 754 compliant floats. */
static inline bool utl_isnan(f32 x)
{
    return x != x;
}

/* Requires IEEE 754 compliant floats. */
static inline bool utl_isinf(f32 x)
{
    return !utl_isnan(x) && utl_isnan(x - x);
}

/* Length of a string, not including null terminator */
static usize utl_strlen(const char* s)
{
    ASSERT(s);
    usize len = 0;
    while (*s++ != '\0')
        ++len;
    return len;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param value - value to convert
 * @param buf - destination string buffer
 * @return buf
*/
static char* utl_u32tostr(u32 value, char* buf)
{
    ASSERT(buf);
    if (value >= 1000000000)
        buf += 9;
    else
        for (u32 digits = 10; digits <= value; digits *= 10)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = '0' + (value % 10);
        value /= 10;
    } while (value != 0);
    return buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param value - value to convert
 * @param buf - destination string buffer
 * @return buf
*/
static char* utl_i32tostr(i32 value, char* buf)
{
    ASSERT(buf);
    u32 abs = (u32)value;
    if (value < 0)
    {
        abs = (u32)-value;
        *buf++ = '-';
    }
    if (abs >= 1000000000)
        buf += 9;
    else
        for (u32 digits = 10; digits <= abs; digits *= 10)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = '0' + (abs % 10);
        abs /= 10;
    } while (abs != 0);
    return (value < 0) ? --buf : buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 * We're checking for inf/nan before float casts, so silencing UB sanitizer.
 *
 * @param value - value to convert
 * @param buf - destination string buffer
 * @param decimals - number of decimal places 
 * @return buf
*/
__attribute__((no_sanitize("undefined"))) static char*
utl_f32tostr(f32 value, char* buf, u8 decimals)
{
    ASSERT(buf);
    if (utl_isnan(value))
    {
        static const char* nan = "NaN";
        utl_memcpy(buf, nan, utl_strlen(nan) + 1);
        return buf;
    }

    if (utl_isinf(value))
    {
        static const char* inf = "Inf";
        utl_memcpy(buf, inf, utl_strlen(inf) + 1);
        return buf;
    }

    i32 whole = (i32)value;
    f32 fraction = utl_fabs((value - (f32)whole) * 10);
    char* start = utl_i32tostr(whole, buf);

    while (*++buf != '\0')
        ;

    *buf++ = '.';

    if (decimals--)
    {
        while ((i32)fraction == 0 && decimals--)
        {
            *buf++ = '0';
            fraction *= 10;
        }
        while (decimals--)
            fraction *= 10;
        utl_u32tostr((u32)fraction, buf);
    }
    else
        *buf = '\0';
    return start;
}

/*
 * Takes a single format specifier of type %d, %u, %f or %s.
 * e.g. utl_sprintf(buf, sizeof(buf), "hello %s", (utl_fmt_u){.s="world"});
 * Chain calls together to construct a string with multiple format specifiers.
 * 
 * @param buf - the destination string buffer
 * @param bufsz - size of destination string buffer
 * @param format - the string format
 * @param values - the format specifier value
 * @return - the destination string buffer
*/
static char* utl_sprintf(char* buf, const usize bufsz, const char* format,
                         const utl_fmts_t values)
{
    ASSERT(buf);
    ASSERT(format);

    char* start = buf;

    usize i_fmt = 0;
    while (*format != '\0')
    {
        if (buf == (start + bufsz))
        {
            LOG_E("Returning early, buffer too small!", 0);
            return start;
        }
        if (*format != '%')
        {
            *buf++ = *format++;
        }
        else
        {
            ++format;
            switch (*format)
            {
                // if buf short than max length spat out, error, return
                case 'd':
                    utl_i32tostr(values.arr[i_fmt++].d, buf);
                    break;
                case 'u':
                    utl_u32tostr(values.arr[i_fmt++].u, buf);
                    break;
                case 'f':
                    utl_f32tostr(values.arr[i_fmt++].f, buf, 3);
                    break;
                case 's':
                {
                    const char* s = values.arr[i_fmt].s;
                    do {
                        *buf++ = *s;
                    } while (*s++ != '\0');
                    break;
                }
                default:
                    continue;
            }
            ++format;

            while (*buf != '\0')
                ++buf;
        }
    }
    *buf = '\0';
    return start;
}

STATIC_ASSERT(sizeof(u8) == 1, u8_1_byte);
STATIC_ASSERT(sizeof(u16) == 2, u16_2_bytes);
STATIC_ASSERT(sizeof(u32) == 4, u32_4_bytes);
STATIC_ASSERT(sizeof(i8) == 1, i8_1_byte);
STATIC_ASSERT(sizeof(i16) == 2, i16_2_bytes);
STATIC_ASSERT(sizeof(i32) == 4, i32_4_bytes);
STATIC_ASSERT(sizeof(f32) == 4, f32_4_bytes);
STATIC_ASSERT(sizeof(void*) == sizeof(usize), ptr_usize_bytes);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CTAP IMPLEMENTATION //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ctp_retcode_e ctp_load_map(void)
{
    cor_start_the_engines();
    return ctp_retcode_MAP_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CORE IMPLEMENTATION //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static cor_retcode_e cor_start_the_engines(void)
{
    bool i = true;
    if (i)
    {
    }
    // bool* p = NULL;
    i = false;
    // LOG_D("Hello world! %d", {.d = i});
    // LOG_D("Hello world!", 0);
    // LOG_E("Invalid map format", 0);
    // char* s = "hello";
    // PANIC("Invalid map format", .s = s);

    char arr[10] = "hello";
    char arr2[10] = "";
    // utl_u32tostr(115, arr2);

    utl_memcpy(arr2, arr, sizeof(arr));
    // ctp_log_cb("hello %s", "world");
    // ASSERT(p);
    return cor_retcode_MAP_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// GRAPHICS IMPLEMENTATION ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// PHYSICS IMPLEMENTATION /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INPUT IMPLEMENTATION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// AUDIO IMPLEMENTATION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// LICENSE //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 
MIT License

Copyright (c) 2024 JamesBMiddleton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// DOCUMENTATION //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*
 
    __Hello world!__

*/
