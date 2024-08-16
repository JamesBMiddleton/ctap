/*
    Author: James Middleton
    Repo: https://github.com/JamesBMiddleton/ctap
    License: MIT
    Docs: EOF
*/

// #define ctp_IMPL

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
typedef __SIZE_TYPE__ usize; //! GCC/Clang compiler dependent.

// #define U8_MIN (0U)
// #define U8_MAX (255U)
// #define U16_MIN (0U)
// #define U16_MAX (65535U)
// #define U32_MIN (0U)
// #define U32_MAX (4294967295U)
// #define I8_MIN (-128)
// #define I8_MAX (127)
// #define I16_MIN (-32767-1)
// #define I16_MAX (32767)
// #define I32_MIN (-2147483647-1)
// #define I32_MAX (2147483647)
//
// #define F32_MIN (1.17549435e-38F) //! Assumes IEEE-754 compliance.
// #define F32_MAX (3.40282347e+38F) //! Assumes IEEE-754 compliance.

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
    const char c;
    const char* s;
    const i32 d;
    const u32 u;
    const f32 f;
} utl_fmt_u;

typedef struct {
    utl_fmt_u arr[MAX_FMT_ARGS];
} utl_fmts_t;

static const char* utl_sprintf(char* buf, usize bufsz, const char* format,
                               utl_fmts_t vals);

static const void* utl_memcpy(void* dest, const void* src, usize count);

static inline usize utl_strlen(const char* str);

static inline i32 utl_powi(i32 base, u32 exp);
static inline u32 utl_powu(u32 base, u32 exp);
static inline f32 utl_powf(f32 base, i32 exp);

static inline u32 utl_abs(i32 val);
static inline f32 utl_fabs(f32 val);

static inline bool utl_isnan(f32 val);
static inline bool utl_isinf(f32 val);

static char* utl_u32tostr(u32 val, char* buf);
static char* utl_i32tostr(i32 val, char* buf);
static char* utl_f32tostr(f32 val, char* buf, u8 decimals);

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
*/

////////////////////////////////////////////////////////////////////////////////
//////////////////////// UTILS IMPLEMENTATION //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define U32_MAX_CHARS (10) // '4294967295'
#define U32_MAX_CHAR_THRESHOLD 1000000000
#define F32_DECIMAL_CHARS (3)
#define F32_MAX_CHARS \
    (1 + U32_MAX_CHARS + 1 + F32_DECIMAL_CHARS) // '-2147483648.123'

#define LOG(loglvl, msg)                                  \
    do                                                    \
    {                                                     \
        if (ctp_log_cb)                                   \
            ctp_log_cb((ctp_log_t){.lvl = (loglvl),       \
                                   .line_num = __LINE__,  \
                                   .func_name = __func__, \
                                   .message = #msg});     \
    } while (0)

static ctp_log_t new_log(const ctp_loglvl_e lvl, const char* func_name,
                         const u32 line_num, const char* format,
                         const utl_fmts_t values)
{
    ctp_log_t log = {.lvl = lvl, .line_num = line_num, .func_name = func_name};
    utl_sprintf(log.message, sizeof(log.message), format, values);
    return log;
}

#ifdef DEBUG
#define LOG_D(format, ...)                                                   \
    do                                                                       \
    {                                                                        \
        if (ctp_log_cb)                                                      \
            ctp_log_cb(new_log(ctp_loglvl_DEBUG, __func__, __LINE__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));         \
    } while (0)
#else
#define LOG_D(...)
#endif

#define LOG_W(format, ...)                                                  \
    do                                                                      \
    {                                                                       \
        if (ctp_log_cb)                                                     \
            ctp_log_cb(new_log(ctp_loglvl_WARN, __func__, __LINE__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));        \
    } while (0)

#define LOG_E(format, ...)                                                   \
    do                                                                       \
    {                                                                        \
        if (ctp_log_cb)                                                      \
            ctp_log_cb(new_log(ctp_loglvl_ERROR, __func__, __LINE__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));         \
    } while (0)

#define PANIC(format, ...)                                                   \
    do                                                                       \
    {                                                                        \
        if (ctp_log_cb)                                                      \
            ctp_log_cb(new_log(ctp_loglvl_PANIC, __func__, __LINE__, format, \
                               (utl_fmts_t){.arr = {__VA_ARGS__}}));         \
        ctp_panic_cb();                                                      \
    } while (0)

#ifdef DEBUG
#define ASSERT(cond)                                            \
    do                                                          \
    {                                                           \
        if (!(cond))                                            \
        {                                                       \
            if (ctp_log_cb)                                     \
                ctp_log_cb((ctp_log_t){.lvl = ctp_loglvl_ERROR, \
                                       .line_num = __LINE__,    \
                                       .func_name = __func__,   \
                                       .message = #cond});      \
            ctp_panic_cb();                                     \
        }                                                       \
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
static const void* utl_memcpy(void* dest, const void* src, const usize count)
{
    ASSERT(dest);
    ASSERT(src);
    if (((usize)src | (usize)dest | count) & sizeof(u32) - 1)
    {
        const u8* src_byte = (const u8*)src;
        u8* dest_byte = (u8*)dest;
        for (usize i = 0; i < count; ++i)
            *(dest_byte++) = *(src_byte++);
    }
    else
    {
        const u32* src_word = (const u32*)src;
        u32* dest_word = (u32*)dest;
        for (usize i = 0; i < count; i += sizeof(u32))
            *(dest_word++) = *(src_word++);
    }
    return dest;
}

static inline i32 utl_powi(i32 base, u32 exp)
{
    i32 tmp = 1;
    while (true)
    {
        if (exp & 1U)
            tmp *= base;
        exp >>= 1U;
        if (!exp)
            break;
        base *= base;
    }
    return tmp;
}

static inline u32 utl_powu(u32 base, u32 exp)
{
    u32 tmp = 1;
    while (true)
    {
        if (exp & 1U)
            tmp *= base;
        exp >>= 1U;
        if (!exp)
            break;
        base *= base;
    }
    return tmp;
}

/* Can handle negative powers. */
static inline f32 utl_powf(f32 base, i32 exp)
{
    if (exp == 0)
        return 1;
    f32 tmp = utl_powf(base, exp / 2);
    if ((exp % 2) == 0)
        return tmp * tmp;
    if (exp > 0)
        return base * tmp * tmp;
    return (tmp * tmp) / base;
}

static inline u32 utl_abs(i32 val)
{
    return (val < 0) ? -(u32)val : (u32)val;
}

static inline f32 utl_fabs(f32 val)
{
    return (val < 0) ? -val : val;
}

/* Requires IEEE 754 compliant floats. */
static inline bool utl_isnan(f32 val)
{
    return val != val;
}

/* Requires IEEE 754 compliant floats. */
static inline bool utl_isinf(f32 val)
{
    return !utl_isnan(val) && utl_isnan(val - val);
}

/* Null terminator not included.*/
static usize utl_strlen(const char* str)
{
    ASSERT(str);
    usize len = 0;
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
static char* utl_u32tostr(u32 val, char* buf)
{
    ASSERT(buf);
    const static u8 base = 10;

    if (val >= U32_MAX_CHAR_THRESHOLD)
        buf += U32_MAX_CHARS - 1;
    else
        for (u32 digits = base; digits <= val; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (i8)('0' + (val % base));
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
static char* utl_i32tostr(i32 val, char* buf)
{
    ASSERT(buf);
    const static u8 base = 10;

    u32 abs = (u32)val;
    if (val < 0)
    {
        abs = -(u32)val;
        *buf++ = '-';
    }
    if (abs >= U32_MAX_CHAR_THRESHOLD)
        buf += U32_MAX_CHARS - 1;
    else
        for (u32 digits = base; digits <= abs; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (i8)('0' + (abs % base));
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
__attribute__((no_sanitize("undefined"))) static char*
utl_f32tostr(f32 val, char* buf, u8 decimals)
{
    ASSERT(buf);
    const static f32 base = 10;

    if (utl_isnan(val))
    {
        static const char* nan = "NaN";
        utl_memcpy(buf, nan, utl_strlen(nan) + 1);
        return buf;
    }

    if (utl_isinf(val))
    {
        static const char* inf = "Inf";
        utl_memcpy(buf, inf, utl_strlen(inf) + 1);
        return buf;
    }

    i32 whole = (i32)val;
    f32 fraction = utl_fabs((val - (f32)whole));
    char* start = utl_i32tostr(whole, buf);

    while (*++buf != '\0')
        ;

    *buf++ = '.';

    if (decimals--)
    {
        fraction *= base;
        while ((i32)fraction == 0 && decimals--)
        {
            *buf++ = '0';
            fraction *= base;
        }
        while (decimals--)
            fraction *= base;
        utl_u32tostr((u32)fraction, buf);
    }
    else
        *buf = '\0';
    return start;
}

/*
 * Takes format specifiers of type %d, %u, %f, %s or %c.
 * e.g. utl_sprintf(buf, bufsz, "%f", (utl_fmts_t){.arr = {{.f = 123.456f}}})
 * 
 * @param buf - the destination string buffer
 * @param bufsz - size of destination string buffer
 * @param format - the string format
 * @param vals - struct wrapped array of format specifier values
 * @return - the destination string buffer
*/
static const char* utl_sprintf(char* buf, const usize bufsz, const char* format,
                               const utl_fmts_t vals)
{
    ASSERT(buf);
    ASSERT(format);

    const char* first = buf;
    const char* last = buf + bufsz - 1;
    usize i_vals = 0;

    while (*format != '\0')
    {
        if (buf == last)
        {
            LOG(ctp_loglvl_ERROR, "Destination buffer too small.");
            break;
        }

        if (*format != '%')
        {
            *buf++ = *format++;
        }
        else
        {
            if (i_vals == MAX_FMT_ARGS)
            {
                LOG(ctp_loglvl_ERROR, "Too many specifiers in format string.");
                break;
            }

            ++format;
            const char specifier = *format++;
            const utl_fmt_u val = vals.arr[i_vals++];

            if (((specifier == 'd' || specifier == 'u' || specifier == 'f') &&
                 (buf + F32_MAX_CHARS) > last) ||
                ((specifier == 's') && (buf + utl_strlen(val.s) > last)))
            {
                LOG(ctp_loglvl_ERROR, "Destination buffer too small.");
                break;
            }

            switch (specifier)
            {
                case 'c':
                    *buf++ = val.c;
                    continue;
                case 'd':
                    utl_i32tostr(val.d, buf);
                    break;
                case 'u':
                    utl_u32tostr(val.u, buf);
                    break;
                case 'f':
                    utl_f32tostr(val.f, buf, 3);
                    break;
                case 's': {
                    const char* str = val.s;
                    do
                    {
                        *buf = *str;
                    } while (*str++ != '\0' && ++buf);
                    break;
                }
                default:
                    LOG(ctp_loglvl_ERROR, "Invalid specifier used.");
                    *buf = '\0';
                    return first;
            }
            buf += utl_strlen(buf);
        }
    }
    *buf = '\0';
    return first;
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
    bool test = true;
    // if (i)
    // {
    // }
    // // bool* p = NULL;
    test = false;
    LOG_D("Hello world! %d", {.d = test});
    // // LOG_D("Hello world!", 0);
    // // LOG_E("Invalid map format", 0);
    // // char* s = "hello";
    // // PANIC("Invalid map format", .s = s);
    //
    // char arr[10] = "hello";
    // char arr2[10] = "";
    // // utl_u32tostr(115, arr2);
    //
    // utl_memcpy(arr2, arr, sizeof(arr));
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
