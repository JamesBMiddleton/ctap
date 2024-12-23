/*
    Author: James Middleton
    Repo: https://github.com/JamesBMiddleton/ctap
    License: MIT
    Docs: EOF
*/

// #define ctp_IMPL

#define bool _Bool
#define true 1
#define false 0
#define NULL ((void*)0)

typedef float f32;
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef signed char i8;
typedef signed short int i16;
typedef signed int i32;
typedef __SIZE_TYPE__ usize; //! GCC/Clang compiler dependent.

#define U8_MIN (0U)
#define U8_MAX (255U)
#define U16_MIN (0U)
#define U16_MAX (65535U)
#define U32_MIN (0U)
#define U32_MAX (4294967295U)
#define I8_MIN (-128)
#define I8_MAX (127)
#define I16_MIN (-32768)
#define I16_MAX (32767)
#define I32_MIN (-2147483648)
#define I32_MAX (2147483647)

#define F32_MIN (1.17549435e-38F) //! Assumes IEEE-754 compliance.
#define F32_MAX (3.40282347e+38F) //! Assumes IEEE-754 compliance.

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CTAP API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum { ctp_rc_NONE, ctp_rc_OK, ctp_rc_NULL_CALLBACK } ctp_rc_e;

typedef struct {
    // map..
    // input_cb..
    void (*log_update_callback)(void); // Optional.
    void (*panic_callback)(void); // Optional.
} ctp_init_args_t;

typedef enum {
    ctp_loglvl_ASSERT,
    ctp_loglvl_DEBUG,
    ctp_loglvl_WARN,
    ctp_loglvl_ERROR,
    ctp_loglvl_PANIC
} ctp_loglvl_e;

#define MAX_LOG_SZ 256

typedef struct {
    ctp_loglvl_e lvl;
    u32 line_num;
    const char* func_name;
    char message[MAX_LOG_SZ];
} ctp_log_t;

void ctp_init(ctp_init_args_t args, ctp_rc_e* rc);
ctp_log_t ctp_get_log(void);

// typedef struct {
//     u32 pixel_matrix;
// } ctp_frame_t;
//
//  ctp_frame_t ctp_get_frame(ctp_retcode_e* retcode);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CORE API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum { cor_rc_NONE, cor_rc_OK, cor_rc_MAP_INVALID } cor_rc_e;

typedef struct {
    u32 placeholder;
} cor_init_args_t;

static void cor_init(cor_init_args_t args, cor_rc_e* rc);

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
/////////////////////////////// UTIL API ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define MAX_FMT_ARGS 5

typedef union {
    const char* s;
    char c;
    i32 d;
    u32 u;
    f32 f;
} utl_fmt_u;

typedef struct {
    utl_fmt_u arr[MAX_FMT_ARGS];
} utl_fmts_t;

typedef enum { utl_rc_NONE, utl_rc_OK, utl_rc_NULL_CALLBACK, utl_rc_NULL_LOG } utl_rc_e;

typedef enum {
    utl_loglvl_ASSERT,
    utl_loglvl_DEBUG,
    utl_loglvl_WARN,
    utl_loglvl_ERROR,
    utl_loglvl_PANIC
} utl_loglvl_e;

typedef struct {
    utl_loglvl_e lvl;
    u32 line_num;
    const char* func_name;
    char message[MAX_LOG_SZ];
} utl_log_t;

typedef struct {
    void (*log_update_callback)(void);
    void (*panic_callback)(void);
} utl_init_args_t;

static void utl_init(utl_init_args_t args, utl_rc_e* rc);

static utl_log_t utl_get_log(void);

static const char* utl_sprintf(char* buf, usize bufsz, const char* format,
                               utl_fmts_t vals);

static const void* utl_memcpy(void* dest, const void* src, usize count);

static inline usize utl_strlen(const char* string);

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
 * LOG(loglvl, msg)
 * LOG_D(format, ...)
 * LOG_W(format, ...)
 * LOG_E(format, ...)
 * PANIC(msg)
 * ASSIGN_IF_ZERO(lvalue, rvalue)
 * ASSERT(cond)
 * STATIC_ASSERT(cond, msg)
 * FEQUAL(left, right)
 * MIN(left, right)
 * MAX(left, right)
 *
*/

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UTIL IMPL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    void (*panic_callback)(void);
    void (*log_update_callback)(void);

    utl_log_t log;
    // pool_t
} iutl_state_t;

//NOLINTBEGIN
static iutl_state_t iutl_state = {0}; // API func !l-value! usage only
//NOLINTEND

#define NULL_TERMINATOR_SZ 1
#define U32_MAX_CHARS (10) // '4294967295'
#define U32_MAX_CHAR_THRESHOLD 1000000000
#define F32_DECIMAL_CHARS (3)
#define NUMERIC_MAX_CHARS \
    (1 + U32_MAX_CHARS + 1 + F32_DECIMAL_CHARS) // '-2147483648.123'

/*
 * @return the last logged message.
*/
static utl_log_t utl_get_log(void)
{
    return iutl_state.log;
}

static utl_log_t iutl_new_log(const utl_loglvl_e lvl,
                              const char* const func_name, const u32 line_num,
                              const char* const format, const utl_fmts_t values)
{
    utl_log_t log = {.lvl = lvl, .line_num = line_num, .func_name = func_name};
    utl_sprintf(log.message, sizeof(log.message), format, values);
    return log;
}

#define LOG(loglvl, msg)                                    \
    do                                                      \
    {                                                       \
        iutl_state.log = (utl_log_t){.lvl = (loglvl),       \
                                     .line_num = __LINE__,  \
                                     .func_name = __func__, \
                                     .message = #msg};      \
        iutl_state.log_update_callback();                   \
    } while (0)

#ifdef DEBUG
#define LOG_D(format, ...)                                             \
    do                                                                 \
    {                                                                  \
        iutl_state.log =                                               \
            iutl_new_log(utl_loglvl_DEBUG, __func__, __LINE__, format, \
                         (utl_fmts_t){.arr = {__VA_ARGS__}});          \
        iutl_state.log_update_callback();                              \
    } while (0)
#else
#define LOG_D(...)
#endif

#define LOG_W(format, ...)                                            \
    do                                                                \
    {                                                                 \
        iutl_state.log =                                              \
            iutl_new_log(utl_loglvl_WARN, __func__, __LINE__, format, \
                         (utl_fmts_t){.arr = {__VA_ARGS__}});         \
        iutl_state.log_update_callback();                             \
    } while (0)

#define LOG_E(format, ...)                                             \
    do                                                                 \
    {                                                                  \
        iutl_state.log =                                               \
            iutl_new_log(utl_loglvl_ERROR, __func__, __LINE__, format, \
                         (utl_fmts_t){.arr = {__VA_ARGS__}});          \
        iutl_state.log_update_callback();                              \
    } while (0)

#define PANIC(msg)                                            \
    do                                                        \
    {                                                         \
        iutl_state.log = (utl_log_t){.lvl = utl_loglvl_PANIC, \
                                     .line_num = __LINE__,    \
                                     .func_name = __func__,   \
                                     .message = #msg};        \
        iutl_state.log_update_callback();                     \
        iutl_state.panic_callback();                          \
    } while (0)

#ifdef DEBUG
#define ASSERT(cond)                                              \
    do                                                            \
    {                                                             \
        if (!(cond))                                              \
        {                                                         \
            iutl_state.log = (utl_log_t){.lvl = utl_loglvl_ERROR, \
                                         .line_num = __LINE__,    \
                                         .func_name = __func__,   \
                                         .message = #cond};       \
            iutl_state.log_update_callback();                     \
            iutl_state.panic_callback();                          \
        }                                                         \
    } while (0)
#else
#define ASSERT(...)
#endif

#define ASSIGN_IF_ZERO(var, value)              \
    do                                          \
    {                                           \
        (var) = ((var) == 0) ? (value) : (var); \
    } while (0)

#define STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

#define FEQUAL_DELTA 1E-6f
#define FEQUAL(left, right)                    \
    ((((left) - (right)) > -(FEQUAL_DELTA)) && \
     (((left) - (right)) < (FEQUAL_DELTA)))

#define MIN(left, right) (((left) < (right)) ? (left) : (right))

#define MAX(left, right) (((left) > (right)) ? (left) : (right))

STATIC_ASSERT(sizeof(u8) == 1, u8_1_byte);
STATIC_ASSERT(sizeof(u16) == 2, u16_2_bytes);
STATIC_ASSERT(sizeof(u32) == 4, u32_4_bytes);
STATIC_ASSERT(sizeof(i8) == 1, i8_1_byte);
STATIC_ASSERT(sizeof(i16) == 2, i16_2_bytes);
STATIC_ASSERT(sizeof(i32) == 4, i32_4_bytes);
STATIC_ASSERT(sizeof(f32) == 4, f32_4_bytes);
STATIC_ASSERT(sizeof(void*) == sizeof(usize), ptr_usize_bytes);

/*
 * If aligned copy 32bit chunks from dest to src, else copy bytes. 
 * Assumes no overlap. 
 *
 * @param dest - copy destination 
 * @param src  - copy source
 * @param count - number of bytes to copy
 * @return copy destination
*/
static const void* utl_memcpy(void* dest, const void* src, const usize count)
{
    ASSERT(dest);
    ASSERT(src);
    if (((usize)src | (usize)dest | count) & (sizeof(u32) - 1))
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

/*
 * @param base - base.
 * @param exp - exponent. 
 * @return power.
*/
static inline i32 utl_powi(i32 base, u32 exp)
{
    i32 result = 1;
    while (true)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp == 0)
            break;
        base *= base;
    }
    return result;
}

/*
 * @param base - base.
 * @param exp - exponent. 
 * @return power.
*/
static inline u32 utl_powu(u32 base, u32 exp)
{
    u32 result = 1;
    if (exp == 0)
        return result;
    while (true)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp == 0)
            break;
        base *= base;
    }
    return result;
}

/*
 * Can handle negative powers.
 *
 * @param base - base.
 * @param exp - exponent. 
 * @return power.
*/
static inline f32 utl_powf(f32 base, i32 exp)
{
    f32 result = 1;
    if (exp == 0)
        return result;
    u32 abs_exp = utl_abs(exp);
    while (true)
    {
        if (abs_exp & 1U)
            result *= base;
        abs_exp >>= 1U;
        if (abs_exp == 0)
            break;
        base *= base;
    }
    return (exp > 0) ? result : 1.0F / result;
}

/* 
 * @param val - pos/neg value.
 * @return absolute value.
*/
static inline u32 utl_abs(const i32 val)
{
    return (val < 0) ? -(u32)val : (u32)val;
}

/* 
 * @param val - pos/neg value.
 * @return absolute value.
*/
static inline f32 utl_fabs(f32 val)
{
    return (val < 0) ? -val : val;
}

/* 
 * Requires IEEE 754 compliant floats. 
 *
 * @param val - potentially NaN float value.
 * @return val is NaN.
*/
static inline bool utl_isnan(const f32 val)
{
    return val != val;
}

/* 
 * Requires IEEE 754 compliant floats. 
 *
 * @param val - potentially infinite float value.
 * @return val is infinity.
*/
static inline bool utl_isinf(const f32 val)
{
    return !utl_isnan(val) && utl_isnan(val - val);
}

/* 
 * @param str - null terminated string.
 * @return length of str, null terminator not included.
 * */
static inline usize utl_strlen(const char* str)
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
    static const u8 base = 10;

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
    static const u8 base = 10;

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
utl_f32tostr(const f32 val, char* buf, u8 decimals)
{
    ASSERT(buf);
    static const f32 base = 10;

    if (utl_isnan(val))
    {
        static const char* nan = "NaN";
        utl_memcpy(buf, nan, utl_strlen(nan) + NULL_TERMINATOR_SZ);
        return buf;
    }

    if (utl_isinf(val))
    {
        static const char* inf = "Inf";
        utl_memcpy(buf, inf, utl_strlen(inf) + NULL_TERMINATOR_SZ);
        return buf;
    }

    i32 whole = (i32)val;
    f32 fraction = utl_fabs((val - (f32)whole));
    char* start = utl_i32tostr(whole, buf);

    buf += utl_strlen(buf);
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
            LOG(utl_loglvl_ERROR, "Destination buffer too small.");
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
                LOG(utl_loglvl_ERROR, "Too many specifiers in format string.");
                break;
            }

            ++format;
            const char specifier = *format++;
            const utl_fmt_u val = vals.arr[i_vals++];

            if (((specifier == 'd' || specifier == 'u' || specifier == 'f') &&
                 (buf + NUMERIC_MAX_CHARS) > last) ||
                ((specifier == 's') && (buf + utl_strlen(val.s) > last)))
            {
                LOG(utl_loglvl_ERROR, "Destination buffer too small.");
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
                    LOG(utl_loglvl_ERROR, "Invalid specifier used.");
                    *buf = '\0';
                    return first;
            }
            buf += utl_strlen(buf);
        }
    }
    *buf = '\0';
    return first;
}

static void iutl_do_nothing(void)
{
}

/* Intentionally trigger a 'divide by zero' trap */
static void iutl_divzero(void)
{
    u32 zero = 0;
    (void)(1 / zero);
}

/*
 * Initialise the utilities module. 
 *
 * @param args initialisation arguments.
 * @throw NULL_LOG, NULL_CALLBACK 
*/
static void utl_init(utl_init_args_t args, utl_rc_e* rc)
{
    iutl_state.log_update_callback = args.log_update_callback;
    iutl_state.panic_callback = args.panic_callback;
    ASSIGN_IF_ZERO(iutl_state.log_update_callback, iutl_do_nothing);
    ASSIGN_IF_ZERO(iutl_state.panic_callback, iutl_divzero);

    iutl_state.log = (utl_log_t){
        .func_name = __func__, .line_num = __LINE__, .message = ""};

    *rc = utl_rc_OK;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////// CTAP IMPL ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
} ictp_state_t;

//NOLINTBEGIN
static ictp_state_t ictp_state = {0}; // API func !l-value! usage only
//NOLINTEND

/* 
 * Retrieve the last log message created.
 * Set 'ctp_update_cb' to be notified when a new log message is available.
 *
 * @return the last log message created
*/
ctp_log_t ctp_get_log(void)
{
    const utl_log_t in_log = utl_get_log();
    ctp_loglvl_e lvl = ctp_loglvl_DEBUG;
    switch (in_log.lvl)
    {
        case utl_loglvl_DEBUG:
            lvl = ctp_loglvl_DEBUG;
            break;
        case utl_loglvl_WARN:
            lvl = ctp_loglvl_WARN;
            break;
        case utl_loglvl_ERROR:
            lvl = ctp_loglvl_ERROR;
            break;
        case utl_loglvl_PANIC:
            lvl = ctp_loglvl_PANIC;
            break;
        case utl_loglvl_ASSERT:
            lvl = ctp_loglvl_ASSERT;
            break;
    }
    ctp_log_t out_log = {
        .lvl = lvl, .line_num = in_log.line_num, .func_name = in_log.func_name};
    utl_memcpy(out_log.message, in_log.message, utl_strlen(in_log.message) + 1);
    return out_log;
}

/* 
 * Initialise the ctap runtime.
 *
 * @param args - runtime initialisation arguments
 * @throw NULL_CALLBACK, NULL_LOG 
*/
void ctp_init(ctp_init_args_t args, ctp_rc_e* rc)
{
    utl_rc_e utl_rc = {0};
    utl_init((utl_init_args_t){.log_update_callback = args.log_update_callback,
                               .panic_callback = args.panic_callback},
             &utl_rc);
    switch (utl_rc)
    {
        case utl_rc_OK:
            LOG_D("utl initialisation successful.", 0);
            break;
        case utl_rc_NULL_LOG:
            LOG_W("utl initialisation failed", 0);
            *rc = ctp_rc_NULL_CALLBACK;
            return;
        default:
            PANIC("utl initialisation failed with return code: ...");
    }

    const u32 placeholder = 42;
    cor_rc_e cor_rc = {0};
    cor_init((cor_init_args_t){.placeholder = placeholder}, &cor_rc);
    switch (cor_rc)
    {
        case cor_rc_OK:
            LOG_D("cor initialisation successful.", 0);
            break;
        default:
            PANIC("cor initialisation failed.");
    }

    *rc = ctp_rc_OK;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CORE IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef struct {
    u32 placeholder;
    // pool_t
} icor_state_t;

//NOLINTBEGIN
static icor_state_t icor_state = {0}; // API func usage only
//NOLINTEND

typedef struct {
    u32 startiness;
    u32 num_horses;
} icor_engine_starter_t;

/*
 * placeholder.
 *
 * @param starter - placeholder
 * @throw MAP_INVALID
*/
static void icor_start_the_engines(icor_engine_starter_t starter, cor_rc_e* rc)
{
    if (starter.startiness != 0)
        *rc = cor_rc_MAP_INVALID;
    LOG_D("Engines started with %u startiness and %u horses!",
          {.u = starter.startiness}, {.u = starter.num_horses});
}

static void icor_spaghettify_value(u32* value, cor_rc_e* rc)
{
    *value = 0;
    *rc = cor_rc_OK;
}

/*
 * Initialise the core module. 
 *
 * @param args - initialisation arguments.
 * @throw MAP_INVALID
*/
static void cor_init(cor_init_args_t args, cor_rc_e* rc)
{
    icor_state.placeholder = args.placeholder;

    icor_spaghettify_value(&icor_state.placeholder, rc);
    if (*rc != cor_rc_OK)
        return;

    icor_start_the_engines(
        (icor_engine_starter_t){.num_horses = icor_state.placeholder,
                                .startiness = icor_state.placeholder},
        rc);
    if (*rc != cor_rc_OK)
        return;

    LOG_D("Map loaded.", 0);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////// GRAPHICS IMPL ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// PHYSICS IMPL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// INPUT IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// AUDIO IMPL ///////////////////////////////////////
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

void tmp(void);
void tmp(void) // suppress 'unused' warnings temporarily
{
    FEQUAL(1, 1);
    MIN(1, 3);
    MAX(1, 3);
    LOG_D("Hello World", 0);
    LOG_W("Hello World", 0);
    LOG_E("Hello World", 0);
    PANIC("Hello World");
    u32 var = 0;
    u32 val = 1;
    ASSIGN_IF_ZERO(var, val);

    (void)U8_MIN;
    (void)U8_MAX;
    (void)U16_MIN;
    (void)U16_MAX;
    (void)U32_MIN;
    (void)U32_MAX;
    (void)I8_MIN;
    (void)I8_MAX;
    (void)I16_MIN;
    (void)I16_MAX;
    (void)I32_MIN;
    (void)I32_MAX;
    (void)F32_MIN;
    (void)F32_MAX;
    (void)NULL;

    icor_state.placeholder = 1;
}
