#ifndef UTILS_H
#define UTILS_H

// --------------------------- PRIMITIVES --------------------------------------
#ifndef PRIMITIVES
#define PRIMITIVES

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
//
#endif // PRIMITIVES

// -----------------------------------------------------------------------------

#define utl_MAX_FMT_ARGS 5
#define utl_MAX_LOG_SZ 256

typedef union {
    const char* s;
    char c;
    i32 d;
    u32 u;
    f32 f;
} UTL_Fmt;

typedef struct {
    UTL_Fmt arr[utl_MAX_FMT_ARGS];
} UTL_Fmts;

typedef enum {
    UTL_LogLvl_DEBUG,
    UTL_LogLvl_WARN,
    UTL_LogLvl_ERROR,
    UTL_LogLvl_PANIC,
    UTL_LogLvl_ASSERT
} UTL_LogLvl;

typedef struct {
    UTL_LogLvl lvl;
    u32 line_num;
    const char* func_name;
    char message[utl_MAX_LOG_SZ];
} UTL_Log;

typedef struct {
    void (*log_update_callback)(void);
    void (*panic_callback)(void);
} UTL_InitArg;
typedef enum {
    UTL_InitRet_OK,
    UTL_InitRet_NULL_CALLBACK,
    UTL_InitRet_NULL_LOG
} UTL_InitRet;
static UTL_InitRet UTL_Init(UTL_InitArg args);

static UTL_Log UTL_GetLog(void);

static const char* UTL_Sprintf(char* buf, usize bufsz, const char* format,
                               UTL_Fmts vals);

static const void* UTL_Memcpy(void* dest, const void* src, usize count);

static inline usize UTL_Strlen(const char* string);

static inline i32 UTL_Powi(i32 base, u32 exp);
static inline u32 UTL_Powu(u32 base, u32 exp);
static inline f32 UTL_Powf(f32 base, i32 exp);

static inline u32 UTL_Abs(i32 val);
static inline f32 UTL_Fabs(f32 val);

static inline bool UTL_Isnan(f32 val);
static inline bool UTL_Isinf(f32 val);

static char* UTL_U32tostr(u32 val, char* buf);
static char* UTL_I32tostr(i32 val, char* buf);
static char* UTL_F32tostr(f32 val, char* buf, u8 decimals);

/* MACROS
 *
 * UTL_LOG(logLvl, msg)
 * UTL_LOG_DEBUG(msg)
 * UTL_LOG_WARN(msg)
 * UTL_LOG_ERROR(msg)
 * UTL_LOG_PANIC(msg)
 * UTL_LOGF_DEBUG(format, ...)
 * UTL_LOGF_WARN(format, ...)
 * UTL_LOGF_ERROR(format, ...)
 * UTL_LOGF_PANIC(format, ...)
 * UTL_UTL_ASSIGN_IF_ZERO(lvalue, rvalue)
 * UTL_ASSERT(cond)
 * UTL_STATIC_ASSERT(cond, msg)
 * UTL_FEQUAL(left, right)
 * UTL_MIN(left, right)
 * UTL_MAX(left, right)
 *
 */

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INTERNAL IMPL ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static UTL_Log utl_NewLog(const UTL_LogLvl lvl, const char* const func_name,
                          const u32 line_num, const char* const format,
                          const UTL_Fmts values)
{
    UTL_Log log = {.lvl = lvl, .line_num = line_num, .func_name = func_name};
    UTL_Sprintf(log.message, sizeof(log.message), format, values);
    return log;
}

static void utl_DoNothing(void)
{
}

/* Intentionally trigger a 'divide by zero' trap */
static void utl_DivZero(void)
{
    u32 zero = 0;
    (void)(1 / zero);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// API IMPL ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-macros"
#pragma GCC diagnostic ignored "-Wunused-function"

struct utl_State {
    void (*panic_callback)(void);
    void (*log_update_callback)(void);

    UTL_Log log;
    // Pool
} static utl_state = {0}; // NOLINT

#define utl_NULL_TERMINATOR_SZ 1
#define utl_U32_MAX_CHARS (10) // '4294967295'
#define utl_U32_MAX_CHAR_THRESHOLD 1000000000
#define utl_F32_DECIMAL_CHARS (3)
#define utl_NUMERIC_MAX_CHARS \
    (1 + utl_U32_MAX_CHARS + 1 + utl_F32_DECIMAL_CHARS) // '-2147483648.123'

/*
 * @return the last logged message.
 */
static UTL_Log UTL_GetLog(void)
{
    return utl_state.log;
}

#define UTL_LOG(log_lvl, msg)                            \
    do                                                   \
    {                                                    \
        utl_state.log = (UTL_Log){.lvl = (log_lvl),      \
                                  .line_num = __LINE__,  \
                                  .func_name = __func__, \
                                  .message = #msg};      \
        utl_state.log_update_callback();                 \
    } while (0)

#ifdef DEBUG
#define UTL_LOG_DEBUG(msg)              \
    do                                  \
    {                                   \
        UTL_LOG(UTL_LogLvl_DEBUG, msg); \
    } while (0)

#define UTL_LOGF_DEBUG(format, ...)                                           \
    do                                                                        \
    {                                                                         \
        utl_state.log = utl_NewLog(UTL_LogLvl_DEBUG, __func__, __LINE__,      \
                                   format, (UTL_Fmts){.arr = {__VA_ARGS__}}); \
        utl_state.log_update_callback();                                      \
    } while (0)
#else
#define UTL_LOG_DEBUG(...)
#define UTL_LOGF_DEBUG(...)
#endif

#define UTL_LOG_WARN(msg)              \
    do                                 \
    {                                  \
        UTL_LOG(UTL_logLvl_WARN, msg); \
    } while (0)

#define UTL_LOGF_WARN(format, ...)                                            \
    do                                                                        \
    {                                                                         \
        utl_state.log = utl_NewLog(UTL_LogLvl_WARN, __func__, __LINE__,       \
                                   format, (UTL_Fmts){.arr = {__VA_ARGS__}}); \
        utl_state.log_update_callback();                                      \
    } while (0)

#define UTL_LOG_ERROR(msg)              \
    do                                  \
    {                                   \
        UTL_LOG(UTL_LogLvl_ERROR, msg); \
    } while (0)

#define UTL_LOGF_ERROR(format, ...)                                           \
    do                                                                        \
    {                                                                         \
        utl_state.log = utl_NewLog(UTL_LogLvl_ERROR, __func__, __LINE__,      \
                                   format, (UTL_Fmts){.arr = {__VA_ARGS__}}); \
        utl_state.log_update_callback();                                      \
    } while (0)

#define UTL_PANIC()                               \
    do                                            \
    {                                             \
        UTL_LOG(UTL_LogLvl_PANIC, "Wuh Woh"); \
        utl_state.panic_callback();               \
    } while (0)

#ifdef DEBUG
#define UTL_ASSERT(cond)                                       \
    do                                                         \
    {                                                          \
        if (!(cond))                                           \
        {                                                      \
            utl_state.log = (UTL_Log){.lvl = UTL_LogLvl_ERROR, \
                                      .line_num = __LINE__,    \
                                      .func_name = __func__,   \
                                      .message = #cond};       \
            utl_state.log_update_callback();                   \
            utl_state.panic_callback();                        \
        }                                                      \
    } while (0)
#else
#define UTL_ASSERT(...)
#endif

#define UTL_ASSIGN_IF_ZERO(var, value)          \
    do                                          \
    {                                           \
        (var) = ((var) == 0) ? (value) : (var); \
    } while (0)

#define UTL_STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

#define utl_FEQUAL_DELTA 1E-6f
#define UTL_FEQUAL(left, right)                    \
    ((((left) - (right)) > -(utl_FEQUAL_DELTA)) && \
     (((left) - (right)) < (utl_FEQUAL_DELTA)))

#define UTL_MIN(left, right) (((left) < (right)) ? (left) : (right))

#define UTL_MAX(left, right) (((left) > (right)) ? (left) : (right))

UTL_STATIC_ASSERT(sizeof(u8) == 1, utl_u8_1_byte);
UTL_STATIC_ASSERT(sizeof(u16) == 2, utl_u16_2_bytes);
UTL_STATIC_ASSERT(sizeof(u32) == 4, utl_u32_4_bytes);
UTL_STATIC_ASSERT(sizeof(i8) == 1, utl_i8_1_byte);
UTL_STATIC_ASSERT(sizeof(i16) == 2, utl_i16_2_bytes);
UTL_STATIC_ASSERT(sizeof(i32) == 4, utl_i32_4_bytes);
UTL_STATIC_ASSERT(sizeof(f32) == 4, utl_f32_4_bytes);
UTL_STATIC_ASSERT(sizeof(void*) == sizeof(usize), utl_ptr_usize_bytes);

/*
 * If aligned copy 32bit chunks from dest to src, else copy bytes.
 * Assumes no overlap.
 *
 * @param dest - copy destination
 * @param src  - copy source
 * @param count - number of bytes to copy
 * @return copy destination
 */
static const void* UTL_Memcpy(void* dest, const void* src, const usize count)
{
    UTL_ASSERT(dest);
    UTL_ASSERT(src);
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
static inline i32 UTL_Powi(i32 base, u32 exp)
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
static inline u32 UTL_Powu(u32 base, u32 exp)
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
static inline f32 UTL_Powf(f32 base, i32 exp)
{
    f32 result = 1;
    if (exp == 0)
        return result;
    u32 abs_exp = UTL_Abs(exp);
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
static inline u32 UTL_Abs(const i32 val)
{
    return (val < 0) ? -(u32)val : (u32)val;
}

/*
 * @param val - pos/neg value.
 * @return absolute value.
 */
static inline f32 UTL_Fabs(f32 val)
{
    return (val < 0) ? -val : val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially NaN float value.
 * @return val is NaN.
 */
static inline bool UTL_Isnan(const f32 val)
{
    return val != val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially infinite float value.
 * @return val is infinity.
 */
static inline bool UTL_Isinf(const f32 val)
{
    return !UTL_Isnan(val) && UTL_Isnan(val - val);
}

/*
 * @param str - null terminated string.
 * @return length of str, null terminator not included.
 * */
static inline usize UTL_Strlen(const char* str)
{
    UTL_ASSERT(str);
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
static char* UTL_U32tostr(u32 val, char* buf)
{
    UTL_ASSERT(buf);
    static const u8 base = 10;

    if (val >= utl_U32_MAX_CHAR_THRESHOLD)
        buf += utl_U32_MAX_CHARS - 1;
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
static char* UTL_I32tostr(i32 val, char* buf)
{
    UTL_ASSERT(buf);
    static const u8 base = 10;

    u32 abs = (u32)val;
    if (val < 0)
    {
        abs = -(u32)val;
        *buf++ = '-';
    }
    if (abs >= utl_U32_MAX_CHAR_THRESHOLD)
        buf += utl_U32_MAX_CHARS - 1;
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
UTL_F32tostr(const f32 val, char* buf, u8 decimals)
{
    UTL_ASSERT(buf);
    static const f32 base = 10;

    if (UTL_Isnan(val))
    {
        static const char* nan = "NaN";
        UTL_Memcpy(buf, nan, UTL_Strlen(nan) + utl_NULL_TERMINATOR_SZ);
        return buf;
    }

    if (UTL_Isinf(val))
    {
        static const char* inf = "Inf";
        UTL_Memcpy(buf, inf, UTL_Strlen(inf) + utl_NULL_TERMINATOR_SZ);
        return buf;
    }

    i32 whole = (i32)val;
    f32 fraction = UTL_Fabs((val - (f32)whole));
    char* start = UTL_I32tostr(whole, buf);

    buf += UTL_Strlen(buf);
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
        UTL_U32tostr((u32)fraction, buf);
    }
    else
        *buf = '\0';
    return start;
}

/*
 * Takes format specifiers of type %d, %u, %f, %s or %c.
 * e.g. UTL_sprintf(buf, bufsz, "%f", (UTL_Fmts){.arr = {{.f = 123.456f}}})
 *
 * @param buf - the destination string buffer
 * @param bufsz - size of destination string buffer
 * @param format - the string format
 * @param vals - struct wrapped array of format specifier values
 * @return - the destination string buffer
 */
static const char* UTL_Sprintf(char* buf, const usize bufsz, const char* format,
                               const UTL_Fmts vals)
{
    UTL_ASSERT(buf);
    UTL_ASSERT(format);

    const char* first = buf;
    const char* last = buf + bufsz - 1;
    usize i_vals = 0;

    while (*format != '\0')
    {
        if (buf == last)
        {
            UTL_LOG_ERROR("Destination buffer too small.");
            break;
        }

        if (*format != '%')
        {
            *buf++ = *format++;
        }
        else
        {
            if (i_vals == utl_MAX_FMT_ARGS)
            {
                UTL_LOG_ERROR("Too many specifiers in format string.");
                break;
            }

            ++format;
            const char specifier = *format++;
            const UTL_Fmt val = vals.arr[i_vals++];

            if (((specifier == 'd' || specifier == 'u' || specifier == 'f') &&
                 (buf + utl_NUMERIC_MAX_CHARS) > last) ||
                ((specifier == 's') && (buf + UTL_Strlen(val.s) > last)))
            {
                UTL_LOG_ERROR("Destination buffer too small.");
                break;
            }

            switch (specifier)
            {
                case 'c': *buf++ = val.c; continue;
                case 'd': UTL_I32tostr(val.d, buf); break;
                case 'u': UTL_U32tostr(val.u, buf); break;
                case 'f': UTL_F32tostr(val.f, buf, 3); break;
                case 's': {
                    const char* str = val.s;
                    do
                    {
                        *buf = *str;
                    } while (*str++ != '\0' && ++buf);
                    break;
                }
                default:
                    UTL_LOG_ERROR("Invalid specifier used.");
                    *buf = '\0';
                    return first;
            }
            buf += UTL_Strlen(buf);
        }
    }
    *buf = '\0';
    return first;
}

/*
 * Initialise the utilities module.
 *
 * @param args initialisation arguments.
 */
static UTL_InitRet UTL_Init(UTL_InitArg args)
{
    utl_state.log_update_callback = args.log_update_callback;
    utl_state.panic_callback = args.panic_callback;
    UTL_ASSIGN_IF_ZERO(utl_state.log_update_callback, utl_DoNothing);
    UTL_ASSIGN_IF_ZERO(utl_state.panic_callback, utl_DivZero);

    utl_state.log =
        (UTL_Log){.func_name = __func__, .line_num = __LINE__, .message = ""};

    UTL_LOGF_DEBUG("Initialisation complete.", 0);
    return UTL_InitRet_OK;
}

#pragma GCC diagnostic pop
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif // UTILS_H
