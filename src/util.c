#ifndef UTIL_C
#define UTIL_C
UTIL_C

// --------------------------- PRIMITIVES --------------------------------------

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

// #define U8_MIN (0U)
// #define U8_MAX (255U)
// #define U16_MIN (0U)
// #define U16_MAX (65535U)
// #define U32_MIN (0U)
// #define U32_MAX (4294967295U)
// #define I8_MIN (-128)
// #define I8_MAX (127)
// #define I16_MIN (-32768)
// #define I16_MAX (32767)
// #define I32_MIN (-2147483648)
// #define I32_MAX (2147483647)
// #define F32_MIN (1.17549435e-38F) //! Assumes IEEE-754 compliance.
// #define F32_MAX (3.40282347e+38F) //! Assumes IEEE-754 compliance.

typedef struct {
    const char* const chars;
    const usize len;
} str;

#define tostr(cstr) ((str) { .len = (sizeof("" cstr "")-1), .chars = (cstr) })

static inline str substr(str src, const usize start, const usize len)
{
    // assert(start < src.len); 
    // assert((start + len) <= src.len);
    return (str){.chars = src.chars + start, .len = len};
}
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API DECL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define util_MAX_FMT_ARGS 5
#define util_MAX_LOG_SZ 256

typedef union {
    const char* s;
    char c;
    i32 d;
    u32 u;
    f32 f;
} util_Fmt;

typedef struct {
    util_Fmt arr[util_MAX_FMT_ARGS];
} util_Fmts;

typedef enum {
    util_LogLvl_DEBUG,
    util_LogLvl_WARN,
    util_LogLvl_ERROR,
    util_LogLvl_PANIC,
    util_LogLvl_ASSERT
} util_LogLvl;

typedef struct {
    util_LogLvl lvl;
    u32 line_num;
    const char* func_name;
    const char* message;
} util_Log;

static const char* util_Sprintf(char* buf, usize bufsz, const char* format,
                                util_Fmts vals);

static const void* util_Memcpy(void* dest, const void* src, usize count);

static inline usize util_Strlen(const char* string);

static inline i32 util_Powi(i32 base, u32 exp);
static inline u32 util_Powu(u32 base, u32 exp);
static inline f32 util_Powf(f32 base, i32 exp);

static inline u32 util_Abs(i32 val);
static inline f32 util_Fabs(f32 val);

static inline bool util_Isnan(f32 val);
static inline bool util_Isinf(f32 val);

static char* util_U32tostr(u32 val, char* buf);
static char* util_I32tostr(i32 val, char* buf);
static char* util_F32tostr(f32 val, char* buf, u8 decimals);

static void util_RegisterEventHandlerLog(void (*callback)(const util_Log));
static void util_RegisterEventHandlerPanic(void (*callback)(void));

/* MACROS
 *
 * util_LOG(logLvl, msg)
 * util_LOG_DEBUG(msg)
 * util_LOG_WARN(msg)
 * util_LOG_ERROR(msg)
 * util_LOG_PANIC(msg)
 * util_LOGF_DEBUG(format, ...)
 * util_LOGF_WARN(format, ...)
 * util_LOGF_ERROR(format, ...)
 * util_LOGF_PANIC(format, ...)
 * util_util_ASSIGN_IF_ZERO(lvalue, rvalue)
 * util_ASSERT(cond)
 * util_STATIC_ASSERT(cond, msg)
 * util_FEQUAL(left, right)
 * util_MIN(left, right)
 * util_MAX(left, right)
 *
 */

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// EVENT IMPL //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* Intentionally trigger a 'divide by zero' trap */
static void util__EventTriggerPanicDefault(void)
{
    u32 zero = 0;
    u32 error = (1 / zero);
    zero = error;
}

static void util__EventTriggerLogDefault(const util_Log log)
{
    (void)log;
}

struct util__EventHandlers {
    void (*panic)(void);
    void (*log)(const util_Log);
} static util__event_handlers = {.panic = util__EventTriggerPanicDefault,
                                 .log = util__EventTriggerLogDefault};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
struct util__State {
    char log_data[util_MAX_LOG_SZ];
} static util__state = {0};

#define util__NULL_TERMINATOR_SZ 1
#define util__U32_MAX_CHARS (10) // '4294967295'
#define util__U32_MAX_CHAR_THRESHOLD 1000000000
#define util__F32_DECIMAL_CHARS (3)
#define util__NUMERIC_MAX_CHARS \
    (1 + util__U32_MAX_CHARS + 1 + util__F32_DECIMAL_CHARS) // '-2147483648.123'

#define util_LOG(log_lvl, msg)                                     \
    do                                                             \
    {                                                              \
        util__event_handlers.log((util_Log){.lvl = (log_lvl),      \
                                            .line_num = __LINE__,  \
                                            .func_name = __func__, \
                                            .message = #msg});     \
    } while (0)

#ifdef DEBUG
#define util_LOG_DEBUG(msg)               \
    do                                    \
    {                                     \
        util_LOG(util_LogLvl_DEBUG, msg); \
    } while (0)

#define util_LOGF_DEBUG(format, ...)                                      \
    do                                                                    \
    {                                                                     \
        util__event_handlers.log((util_Log){                              \
            .lvl = util_LogLvl_DEBUG,                                     \
            .line_num = __LINE__,                                         \
            .func_name = __func__,                                        \
            .message = util_Sprintf(util__state.log_data,                 \
                                    sizeof(util__state.log_data), format, \
                                    (util_Fmts){.arr = {__VA_ARGS__}})}); \
    } while (0)
#else
#define util_LOG_DEBUG(...)
#define util_LOGF_DEBUG(...)
#endif

#define util_LOG_WARN(msg)               \
    do                                   \
    {                                    \
        util_LOG(util_LogLvl_WARN, msg); \
    } while (0)

#define util_LOGF_WARN(format, ...)                                       \
    do                                                                    \
    {                                                                     \
        util__event_handlers.log((util_Log){                              \
            .lvl = util_LogLvl_WARN,                                      \
            .line_num = __LINE__,                                         \
            .func_name = __func__,                                        \
            .message = util_Sprintf(util__state.log_data,                 \
                                    sizeof(util__state.log_data), format, \
                                    (util_Fmts){.arr = {__VA_ARGS__}})}); \
    } while (0)

#define util_LOG_ERROR(msg)               \
    do                                    \
    {                                     \
        util_LOG(util_LogLvl_ERROR, msg); \
    } while (0)

#define util_LOGF_ERROR(format, ...)                                      \
    do                                                                    \
    {                                                                     \
        util__event_handlers.log((util_Log){                              \
            .lvl = util_LogLvl_ERROR,                                     \
            .line_num = __LINE__,                                         \
            .func_name = __func__,                                        \
            .message = util_Sprintf(util__state.log_data,                 \
                                    sizeof(util__state.log_data), format, \
                                    (util_Fmts){.arr = {__VA_ARGS__}})}); \
    } while (0)

#define util_PANIC()                            \
    do                                          \
    {                                           \
        util_LOG(util_LogLvl_PANIC, "Wuh Woh"); \
        util__event_handlers.panic();           \
    } while (0)

#ifdef DEBUG
#define util_ASSERT(cond)                                                 \
    do                                                                    \
    {                                                                     \
        if (!(cond))                                                      \
        {                                                                 \
            util__event_handlers.log((util_Log){.lvl = util_LogLvl_ERROR, \
                                                .line_num = __LINE__,     \
                                                .func_name = __func__,    \
                                                .message = #cond});       \
            util__event_handlers.panic();                                 \
        }                                                                 \
    } while (0)
#else
#define util_ASSERT(...)
#endif

// #define util_ASSIGN_IF_ZERO(var, value)         \
//     do                                          \
//     {                                           \
//         (var) = ((var) == 0) ? (value) : (var); \
//     } while (0)

#define util_STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

#define util__FEQUAL_DELTA 1E-6F
#define util_FEQUAL(left, right)                     \
    ((((left) - (right)) > -(util__FEQUAL_DELTA)) && \
     (((left) - (right)) < (util__FEQUAL_DELTA)))

#define util_MIN(left, right) (((left) < (right)) ? (left) : (right))

#define util_MAX(left, right) (((left) > (right)) ? (left) : (right))

util_STATIC_ASSERT(sizeof(u8) == 1, util__u8_1_byte);
util_STATIC_ASSERT(sizeof(u16) == 2, util__u16_2_bytes);
util_STATIC_ASSERT(sizeof(u32) == 4, util__u32_4_bytes);
util_STATIC_ASSERT(sizeof(i8) == 1, util__i8_1_byte);
util_STATIC_ASSERT(sizeof(i16) == 2, util__i16_2_bytes);
util_STATIC_ASSERT(sizeof(i32) == 4, util__i32_4_bytes);
util_STATIC_ASSERT(sizeof(f32) == 4, util__f32_4_bytes);
util_STATIC_ASSERT(sizeof(void*) == sizeof(usize), util__ptr_usize_bytes);

/*
 * If aligned copy 32bit chunks from dest to src, else copy bytes.
 * Assumes no overlap.
 *
 * @param dest - copy destination
 * @param src  - copy source
 * @param count - number of bytes to copy
 * @return copy destination
 */
static const void* util_Memcpy(void* dest, const void* src, const usize count)
{
    util_ASSERT(dest);
    util_ASSERT(src);
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
static inline i32 util_Powi(i32 base, u32 exp)
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
static inline u32 util_Powu(u32 base, u32 exp)
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
static inline f32 util_Powf(f32 base, i32 exp)
{
    f32 result = 1;
    if (exp == 0)
        return result;
    u32 abs_exp = util_Abs(exp);
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
static inline u32 util_Abs(const i32 val)
{
    return (val < 0) ? -(u32)val : (u32)val;
}

/*
 * @param val - pos/neg value.
 * @return absolute value.
 */
static inline f32 util_Fabs(f32 val)
{
    return (val < 0) ? -val : val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially NaN float value.
 * @return val is NaN.
 */
static inline bool util_Isnan(const f32 val)
{
    return val != val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially infinite float value.
 * @return val is infinity.
 */
static inline bool util_Isinf(const f32 val)
{
    return !util_Isnan(val) && util_Isnan(val - val);
}

/*
 * @param str - null terminated string.
 * @return length of str, null terminator not included.
 * */
static inline usize util_Strlen(const char* str)
{
    util_ASSERT(str);
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
static char* util_U32tostr(u32 val, char* buf)
{
    util_ASSERT(buf);
    static const u8 base = 10;

    if (val >= util__U32_MAX_CHAR_THRESHOLD)
        buf += util__U32_MAX_CHARS - 1;
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
static char* util_I32tostr(i32 val, char* buf)
{
    util_ASSERT(buf);
    static const u8 base = 10;

    u32 abs = (u32)val;
    if (val < 0)
    {
        abs = -(u32)val;
        *buf++ = '-';
    }
    if (abs >= util__U32_MAX_CHAR_THRESHOLD)
        buf += util__U32_MAX_CHARS - 1;
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
util_F32tostr(const f32 val, char* buf, u8 decimals)
{
    util_ASSERT(buf);
    static const f32 base = 10;

    if (util_Isnan(val))
    {
        static const char* nan = "NaN";
        util_Memcpy(buf, nan, util_Strlen(nan) + util__NULL_TERMINATOR_SZ);
        return buf;
    }

    if (util_Isinf(val))
    {
        static const char* inf = "Inf";
        util_Memcpy(buf, inf, util_Strlen(inf) + util__NULL_TERMINATOR_SZ);
        return buf;
    }

    i32 whole = (i32)val;
    f32 fraction = util_Fabs((val - (f32)whole));
    char* start = util_I32tostr(whole, buf);

    buf += util_Strlen(buf);
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
        util_U32tostr((u32)fraction, buf);
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
static const char* util_Sprintf(char* buf, const usize bufsz,
                                const char* format, const util_Fmts vals)
{
    util_ASSERT(buf);
    util_ASSERT(format);

    const char* first = buf;
    const char* last = buf + bufsz - 1;
    usize i_vals = 0;

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
                 (buf + util__NUMERIC_MAX_CHARS) > last) ||
                ((specifier == 's') && (buf + util_Strlen(val.s) > last)))
            {
                util_LOG_ERROR("Destination buffer too small.");
                break;
            }

            switch (specifier)
            {
                case 'c': *buf++ = val.c; continue;
                case 'd': util_I32tostr(val.d, buf); break;
                case 'u': util_U32tostr(val.u, buf); break;
                case 'f': util_F32tostr(val.f, buf, 3); break;
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

static void util_RegisterEventHandlerLog(void (*callback)(const util_Log))
{
    if (callback == NULL)
        util__event_handlers.panic();
    util__event_handlers.log = callback;
}

static void util_RegisterEventHandlerPanic(void (*callback)(void))
{
    if (callback == NULL)
        util__event_handlers.panic();
    util__event_handlers.panic = callback;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// UTEST IMPL /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#ifdef UTEST
#include <stdio.h>

__attribute__((used))
static void utest_util_EventHandlerLogPrintf(const util_Log log)
{
    const char* lvl = "UNKNOWN";
    switch (log.lvl)
    {
        case util_LogLvl_DEBUG: lvl = "DEBUG"; break;
        case util_LogLvl_WARN: lvl = "WARN"; break;
        case util_LogLvl_ERROR: lvl = "ERROR"; break;
        case util_LogLvl_PANIC: lvl = "PANIC"; break;
        case util_LogLvl_ASSERT: lvl = "ASSERT"; break;
    }
    printf("%s | %s::%d | %s\n", lvl, log.func_name, log.line_num, log.message);
    (void)fflush(stdout);
}

__attribute__((used))
static void utest_util_EventHandlerPanicDoNothing(void)
{
}

#endif // UTEST
#ifdef UTEST_UTIL

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <string.h>

static void utest_util_Powi(void)
{
    assert(util_Powi(0, 0) == 1);
    assert(util_Powi(0, 1) == 0);

    assert(util_Powi(1, 0) == 1);
    assert(util_Powi(1, 1) == 1);
    assert(util_Powi(1, 5) == 1);
    assert(util_Powi(2, 5) == 32);
    assert(util_Powi(5, 2) == 25);
    assert(util_Powi(14, 8) == 1475789056);

    assert(util_Powi(-1, 0) == 1);
    assert(util_Powi(-1, 1) == -1);
    assert(util_Powi(-1, 5) == -1);
    assert(util_Powi(-2, 5) == -32);
    assert(util_Powi(-5, 2) == 25);
    assert(util_Powi(-14, 7) == -105413504);
}

static void utest_util_Powu(void)
{
    assert(util_Powu(0, 1) == 0);
    assert(util_Powu(1, 0) == 1);
    assert(util_Powu(0, 0) == 1);
    assert(util_Powu(1, 1) == 1);
    assert(util_Powu(1, 1) == 1);
    assert(util_Powu(1, 5) == 1);
    assert(util_Powu(2, 5) == 32);
    assert(util_Powu(5, 2) == 25);
    assert(util_Powu(15, 8) == 2562890625);
}

static void utest_util_Powf(void)
{
    assert(util_FEQUAL(util_Powf(0, 0), 1));
    assert(util_FEQUAL(util_Powf(0, 1), 0));
    assert(util_FEQUAL(util_Powf(-1, 0), 1));
    assert(util_Isinf(util_Powf(0, -1)));

    assert(util_FEQUAL(util_Powf(42, 0), 1));
    assert(util_FEQUAL(util_Powf(42, 1), 42));
    assert(util_FEQUAL(util_Powf(42, 2), 1764));
    assert(util_FEQUAL(util_Powf(42, 3), 74088));

    assert(util_FEQUAL(util_Powf(-42, 0), 1));
    assert(util_FEQUAL(util_Powf(-42, 1), -42));
    assert(util_FEQUAL(util_Powf(-42, 2), 1764));
    assert(util_FEQUAL(util_Powf(-42, 3), -74088));

    assert(util_FEQUAL(util_Powf(0.1F, -1), 10));
    assert(util_FEQUAL(util_Powf(1.5F, 42), 24878997.7221F));
    assert(util_FEQUAL(util_Powf(1.5F, -42), 4.01945452614e-8F));

    assert(util_FEQUAL(util_Powf(-0.1F, -1), -10));
    assert(util_FEQUAL(util_Powf(-1.5F, 42), 24878997.7221F));
    assert(util_FEQUAL(util_Powf(-1.5F, -42), 4.01945452614e-8F));
}

static void utest_util_Abs(void)
{
    assert(util_Abs(1) == 1);
    assert(util_Abs(-1) == 1);
    assert(util_Abs(-0) == 0);
    assert(util_Abs(-123) == 123);
    assert(util_Abs(123) == 123);
    assert(util_Abs(INT_MIN + 1) == INT_MAX);
}

static void utest_util_Fabs(void)
{
    assert(util_FEQUAL(util_Fabs(-0.0F), 0.0F));
    assert(util_FEQUAL(util_Fabs(-1.0F), 1.0F));
    assert(util_FEQUAL(util_Fabs(1.0F), 1.0F));
    assert(util_FEQUAL(util_Fabs(-123.456F), 123.456F));
    assert(util_FEQUAL(util_Fabs(123.456F), 123.456F));
}

static void utest_util_Isnan(void)
{
    assert(util_Isnan(0.0F) == false);
    assert(util_Isnan(1.0F) == false);
    assert(util_Isnan(-1.0F) == false);
    assert(util_Isnan(-1.0F / 0.0F) == false);
    assert(util_Isnan(FLT_MAX) == false);
    assert(util_Isnan(FLT_MIN) == false);
    assert(util_Isnan(-0.0F / 0.0F) == true);
}
static void utest_util_Isinf(void)
{
    assert(util_Isinf(0.0F) == false);
    assert(util_Isinf(1.0F) == false);
    assert(util_Isinf(-1.0F) == false);
    assert(util_Isinf(-0.0F / 0.0F) == false);
    assert(util_Isinf(FLT_MAX) == false);
    assert(util_Isinf(FLT_MIN) == false);
    assert(util_Isinf(-1.0F / 0.0F) == true);
}

static void utest_util_Sprintf(void)
{
    char arr[util_MAX_LOG_SZ] = {0};

    const u32 u32_num = 123;
    const i32 i32_num = 123;

    const char* overflowcheck1 = "toomanychars";
    const char* overflowcheck2 = "toomanychar%c";
    const char* overflowcheck3 = "toomanychar%s";
    const char* overflowcheck4 = "waytoo%s";
    const char* overflowcheck5 = "floatdon'tfit%f";
    const char* overflowcheck6 = "intjustdon'tfit%d";

    // Check error cases.
    util_Sprintf(arr, util_Strlen(overflowcheck1), overflowcheck1,
                 (util_Fmts){0});
    assert(strcmp(arr, "toomanychar") == 0);

    util_Sprintf(arr, sizeof(arr), "invalid %t specifier", (util_Fmts){0});
    assert(strcmp(arr, "invalid ") == 0);

    util_Sprintf(
        arr, sizeof(arr), "%c%c%c%c%c%c too many specifiers",
        (util_Fmts){
            .arr = {
                {.c = 'h'}, {.c = 'e'}, {.c = 'l'}, {.c = 'l'}, {.c = 'o'}}});
    assert(strcmp(arr, "hello") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck2) - 1, overflowcheck2,
                 (util_Fmts){.arr = {{.c = 's'}}});
    assert(strcmp(arr, "toomanychar") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck3) - 1, overflowcheck3,
                 (util_Fmts){.arr = {{.s = "s"}}});
    assert(strcmp(arr, "toomanychar") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck4), overflowcheck4,
                 (util_Fmts){.arr = {{.s = "manychars"}}});
    assert(strcmp(arr, "waytoo") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck5) + 1, overflowcheck5,
                 (util_Fmts){.arr = {{.f = 1.0F}}});
    assert(strcmp(arr, "floatdon'tfit") == 0);

    util_Sprintf(arr, util_Strlen(overflowcheck6) + util__NUMERIC_MAX_CHARS - 2,
                 overflowcheck6, (util_Fmts){.arr = {{.d = 1}}});
    assert(strcmp(arr, "intjustdon'tfit") == 0);

    // Check u32s.
    util_Sprintf(arr, sizeof(arr), "%u", (util_Fmts){.arr = {{.u = u32_num}}});
    assert(strcmp(arr, "123") == 0);

    util_Sprintf(arr, sizeof(arr), "%u", (util_Fmts){.arr = {{.u = 0}}});
    assert(strcmp(arr, "0") == 0);

    util_Sprintf(arr, sizeof(arr), "%u", (util_Fmts){.arr = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "4294967295") == 0);

    util_Sprintf(arr, sizeof(arr), "a%ua",
                 (util_Fmts){.arr = {{.u = UINT_MAX}}});
    assert(strcmp(arr, "a4294967295a") == 0);

    // Check i32s.
    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = i32_num}}});
    assert(strcmp(arr, "123") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = -i32_num}}});
    assert(strcmp(arr, "-123") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = 0}}});
    assert(strcmp(arr, "0") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = INT_MIN}}});
    assert(strcmp(arr, "-2147483648") == 0);

    util_Sprintf(arr, sizeof(arr), "%d", (util_Fmts){.arr = {{.d = INT_MAX}}});
    assert(strcmp(arr, "2147483647") == 0);

    util_Sprintf(arr, sizeof(arr), "a%da",
                 (util_Fmts){.arr = {{.d = INT_MIN}}});
    assert(strcmp(arr, "a-2147483648a") == 0);

    // Check f32s.
    const f32 f32_num = 123.456F;
    const f32 f32_num_fract_part = .456F;
    const f32 f32_num_int_part = 123.F;

    util_Sprintf(arr, sizeof(arr), "%f", (util_Fmts){.arr = {{.f = f32_num}}});
    assert(strcmp(arr, "123.456") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = f32_num_fract_part}}});
    assert(strcmp(arr, "0.456") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = f32_num_int_part}}});
    assert(strcmp(arr, "123.000") == 0);

    util_Sprintf(arr, sizeof(arr), "%f", (util_Fmts){.arr = {{.f = 0}}});
    assert(strcmp(arr, "0.000") == 0);

    util_Sprintf(arr, sizeof(arr), "%f", (util_Fmts){.arr = {{.f = -0}}});
    assert(strcmp(arr, "0.000") == 0);

    util_Sprintf(arr, sizeof(arr), "%f", (util_Fmts){.arr = {{.f = -f32_num}}});
    assert(strcmp(arr, "-123.456") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = -0.0F / 0.0F}}});
    assert(strcmp(arr, "NaN") == 0);

    util_Sprintf(arr, sizeof(arr), "%f",
                 (util_Fmts){.arr = {{.f = -1.0F / 0.0F}}});
    assert(strcmp(arr, "Inf") == 0);

    // Check strings.
    util_Sprintf(arr, sizeof(arr), "%s", (util_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobar") == 0);

    util_Sprintf(arr, sizeof(arr), "a%s",
                 (util_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "afoobar") == 0);

    util_Sprintf(arr, sizeof(arr), "%sa",
                 (util_Fmts){.arr = {{.s = "foobar"}}});
    assert(strcmp(arr, "foobara") == 0);

    util_Sprintf(arr, sizeof(arr), "%s%s",
                 (util_Fmts){.arr = {{.s = "foo"}, {.s = "bar"}}});
    assert(strcmp(arr, "foobar") == 0);

    // Check chars.
    util_Sprintf(arr, sizeof(arr), "%c", (util_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "a") == 0);

    util_Sprintf(arr, sizeof(arr), "a%c", (util_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);

    util_Sprintf(arr, sizeof(arr), "%cc", (util_Fmts){.arr = {{.c = 'a'}}});
    assert(strcmp(arr, "ac") == 0);

    util_Sprintf(arr, sizeof(arr), "%c%c",
                 (util_Fmts){.arr = {{.c = 'a'}, {.c = 'a'}}});
    assert(strcmp(arr, "aa") == 0);
}

static void utest_util_MAX(void)
{
    const u32 zero = 0;
    assert(util_MAX(1, 2) == 2);
    assert(util_MAX(2, 1) == 2);
    assert(util_MAX(-1, 0) == 0);
    assert(util_MAX(zero, 0) == 0);
}

static void utest_util_MIN(void)
{
    const u32 zero = 0;
    assert(util_MIN(1, 2) == 1);
    assert(util_MIN(2, 1) == 1);
    assert(util_MIN(-1, 0) == -1);
    assert(util_MIN(zero, 0) == 0);
}

static void utest_util_Untested(void)
{
    util_LOG_WARN("Hello World");
    util_LOGF_WARN("Hello World", {0});
    util_LOGF_ERROR("Hello World", {0});
    util_LOGF_DEBUG("Hello World", {0});
    util_LOG_DEBUG("Hello World");
    util_PANIC();
}

#ifdef UTEST
static void utest_util_Main(void)
{
    util_RegisterEventHandlerLog(utest_util_EventHandlerLogPrintf);
    util_RegisterEventHandlerPanic(utest_util_EventHandlerPanicDoNothing);
    utest_util_Sprintf();
    utest_util_Powi();
    utest_util_Powu();
    utest_util_Powf();
    utest_util_Abs();
    utest_util_Fabs();
    utest_util_Isnan();
    utest_util_Isinf();
    utest_util_MAX();
    utest_util_MIN();
    utest_util_Untested();
}

i32 main(void)
{
    utest_util_Main();
    return 0;
}
#endif // UTEST
#endif // UTIL_UTEST
#endif // UTIL_C
