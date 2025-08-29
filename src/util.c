#ifndef UTIL_C
#define UTIL_C
UTIL_C

#include "primitives.c"

#define vec_TYPE uchar
#include "vec.c"
#define vec_TYPE int
#include "vec.c"
#define vec_TYPE uint
#include "vec.c"
#define vec_TYPE char
#include "vec.c"
#define vec_TYPE float
#include "vec.c"

#define util_MAX_LOG_SZ 256
#define util_MAX_FMT_ARGS 5
#define NULL_TERMINATOR_SZ 1
#define UINT_MAX_CHARS (10) // '4294967295'
#define UINT_MAX_CHAR_THRESHOLD 1000000000
#define FLOAT_DECIMAL_CHARS (3)
#define NUMERIC_MAX_CHARS \
    (1 + UINT_MAX_CHARS + 1 + FLOAT_DECIMAL_CHARS) // '-2147483648.123'

typedef enum {
    util_LogLvl_DEBUG,
    util_LogLvl_WARN,
    util_LogLvl_ERROR,
    util_LogLvl_PANIC,
    util_LogLvl_ASSERT
} util_LogLvl;

typedef struct {
    util_LogLvl lvl;
    uint lineNum;
    const char* funcName;
    const char* message;
} util_Log;

typedef union {
    const char* s;
    char c;
    int d;
    uint u;
    float f;
} util_Fmt;

typedef struct {
    util_Fmt arr[util_MAX_FMT_ARGS];
} util_Fmts;

static const void* util_Memcpy(void* dest, const void* src, usize count);
static inline uint util_Abs(int val);
static inline int util_Powi(int base, uint exp);
static inline uint util_Powu(uint base, uint exp);
static inline float util_Powf(float base, int exp);
static inline float util_Fabs(float val);
static inline bool util_Isnan(float val);
static inline bool util_Isinf(float val);
static inline usize util_Strlen(const char* str);
static char* util_Uinttostr(uint val, char* buf);
static char* util_Inttostr(int val, char* buf);
static char* util_Floattostr(float val, char* buf, uchar decimals);
static const char* util_Sprintf(char* buf, usize bufsz,
                                const char* format, util_Fmts vals);
static void util_RegisterEventHandlerLog(void (*callback)(const util_Log));
static void util_RegisterEventHandlerPanic(void (*callback)(void));

static void EventHandlerPanicDefault(void);
static void EventHandlerLogDefault(util_Log log);

// util_LOG(log_lvl, msg)
// util_LOG_DEBUG(msg)
// util_LOGF_DEBUG(format, ...)
// util_LOG_WARN(msg)
// util_LOGF_WARN(format, ...)
// util_LOG_ERROR(msg)
// util_LOGF_ERROR(format, ...)
// util_PANIC()
// util_ASSERT(cond)
// util_ASSIGN_IF_ZERO(var, value)
// util_FLT_EQUAL(left, right)
// util_MIN(left, right)
// util_MAX(left, right)

/* Intentionally trigger a 'divide by zero' trap */
static void EventHandlerPanicDefault(void)
{
    uint zero = 0;
    uint error = (1 / zero);
    zero = error;
}

static void EventHandlerLogDefault(const util_Log log)
{
    (void)log;
}

struct EventHandlersUtil {
    void (*panic)(void);
    void (*log)(const util_Log);
} static gEventHandlersUtil = {.panic = EventHandlerPanicDefault,
                               .log = EventHandlerLogDefault};

struct StateUtil {
    char logData[util_MAX_LOG_SZ];
} static gStateUtil = {0};

#define util_LOG(log_lvl, msg)                                  \
    do                                                          \
    {                                                           \
        gEventHandlersUtil.log((util_Log){.lvl = (log_lvl),     \
                                          .lineNum = __LINE__,  \
                                          .funcName = __func__, \
                                          .message = #msg});    \
    } while (0)

#ifdef DEBUG
#define util_LOG_DEBUG(msg)               \
    do                                    \
    {                                     \
        util_LOG(util_LogLvl_DEBUG, msg); \
    } while (0)

#define util_LOGF_DEBUG(format, ...)                                         \
    do                                                                       \
    {                                                                        \
        gEventHandlersUtil.log((util_Log){                                   \
            .lvl = util_LogLvl_DEBUG,                                        \
            .lineNum = __LINE__,                                             \
            .funcName = __func__,                                            \
            .message =                                                       \
                util_Sprintf(gStateUtil.logData, sizeof(gStateUtil.logData), \
                             format, (util_Fmts){.arr = {__VA_ARGS__}})});   \
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

#define util_LOGF_WARN(format, ...)                                          \
    do                                                                       \
    {                                                                        \
        gEventHandlersUtil.log((util_Log){                                   \
            .lvl = util_LogLvl_WARN,                                         \
            .lineNum = __LINE__,                                             \
            .funcName = __func__,                                            \
            .message =                                                       \
                util_Sprintf(gStateUtil.logData, sizeof(gStateUtil.logData), \
                             format, (util_Fmts){.arr = {__VA_ARGS__}})});   \
    } while (0)

#define util_LOG_ERROR(msg)               \
    do                                    \
    {                                     \
        util_LOG(util_LogLvl_ERROR, msg); \
    } while (0)

#define util_LOGF_ERROR(format, ...)                                         \
    do                                                                       \
    {                                                                        \
        gEventHandlersUtil.log((util_Log){                                   \
            .lvl = util_LogLvl_ERROR,                                        \
            .lineNum = __LINE__,                                             \
            .funcName = __func__,                                            \
            .message =                                                       \
                util_Sprintf(gStateUtil.logData, sizeof(gStateUtil.logData), \
                             format, (util_Fmts){.arr = {__VA_ARGS__}})});   \
    } while (0)

#define util_PANIC()                            \
    do                                          \
    {                                           \
        util_LOG(util_LogLvl_PANIC, "Wuh Woh"); \
        gEventHandlersUtil.panic();             \
    } while (0)

#ifdef DEBUG
#define util_ASSERT(cond)                                               \
    do                                                                  \
    {                                                                   \
        if (!(cond))                                                    \
        {                                                               \
            gEventHandlersUtil.log((util_Log){.lvl = util_LogLvl_ERROR, \
                                              .lineNum = __LINE__,      \
                                              .funcName = __func__,     \
                                              .message = #cond});       \
            gEventHandlersUtil.panic();                                 \
        }                                                               \
    } while (0)
#else
#define util_ASSERT(...)
#endif

// #define util_ASSIGN_IF_ZERO(var, value)         \
//     do                                          \
//     {                                           \
//         (var) = ((var) == 0) ? (value) : (var); \
//     } while (0)

// #define util_STATIC_ASSERT(cond, msg) \
//     typedef char static_assert_##msg[(cond) ? 1 : -1]

#define FLT_EQUAL_DELTA 1E-6F
#define util_FLT_EQUAL(left, right)               \
    ((((left) - (right)) > -(FLT_EQUAL_DELTA)) && \
     (((left) - (right)) < (FLT_EQUAL_DELTA)))

#define util_MIN(left, right) (((left) < (right)) ? (left) : (right))

#define util_MAX(left, right) (((left) > (right)) ? (left) : (right))

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
    if (((usize)src | (usize)dest | count) & (sizeof(uint) - 1))
    {
        const uchar* src_byte = (const uchar*)src;
        uchar* dest_byte = (uchar*)dest;
        for (usize i = 0; i < count; ++i)
            *(dest_byte++) = *(src_byte++);
    }
    else
    {
        const uint* src_word = (const uint*)src;
        uint* dest_word = (uint*)dest;
        for (usize i = 0; i < count; i += sizeof(uint))
            *(dest_word++) = *(src_word++);
    }
    return dest;
}

/*
 * @param val - pos/neg value.
 * @return absolute value.
 */
static inline uint util_Abs(const int val)
{
    return (val < 0) ? -(uint)val : (uint)val;
}

/*
 * @param base - base.
 * @param exp - exponent.
 * @return power.
 */
static inline int util_Powi(int base, uint exp)
{
    int result = 1;
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
static inline uint util_Powu(uint base, uint exp)
{
    uint result = 1;
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
static inline float util_Powf(float base, int exp)
{
    float result = 1;
    if (exp == 0)
        return result;
    uint abs_exp = util_Abs(exp);
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
static inline float util_Fabs(float val)
{
    return (val < 0) ? -val : val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially NaN float value.
 * @return val is NaN.
 */
static inline bool util_Isnan(const float val)
{
    return val != val;
}

/*
 * Requires IEEE 754 compliant floats.
 *
 * @param val - potentially infinite float value.
 * @return val is infinity.
 */
static inline bool util_Isinf(const float val)
{
    return !util_Isnan(val) && util_Isnan(val - val);
}

/*
 * @param str - null terminated string.
 * @return length of str, null terminator not included.
 * */
static inline usize util_Strlen(const char* str)
{
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
static char* util_Uinttostr(uint val, char* buf)
{
    static const uchar base = 10;

    if (val >= UINT_MAX_CHAR_THRESHOLD)
        buf += UINT_MAX_CHARS - 1;
    else
        for (uint digits = base; digits <= val; digits *= base)
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
static char* util_Inttostr(int val, char* buf)
{
    static const uchar base = 10;

    uint abs = (uint)val;
    if (val < 0)
    {
        abs = -(uint)val;
        *buf++ = '-';
    }
    if (abs >= UINT_MAX_CHAR_THRESHOLD)
        buf += UINT_MAX_CHARS - 1;
    else
        for (uint digits = base; digits <= abs; digits *= base)
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
__attribute__((no_sanitize("undefined"))) static char*
util_Floattostr(const float val, char* buf, uchar decimals)
{
    static const float base = 10;

    if (util_Isnan(val))
    {
        static const char* nan = "NaN";
        util_Memcpy(buf, nan, util_Strlen(nan) + NULL_TERMINATOR_SZ);
        return buf;
    }

    if (util_Isinf(val))
    {
        static const char* inf = "Inf";
        util_Memcpy(buf, inf, util_Strlen(inf) + NULL_TERMINATOR_SZ);
        return buf;
    }

    int whole = (int)val;
    float fraction = util_Fabs((val - (float)whole));
    char* start = util_Inttostr(whole, buf);

    buf += util_Strlen(buf);
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
        util_Uinttostr((uint)fraction, buf);
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
__attribute__((used)) static const char* util_Sprintf(char* buf,
                                                      const usize bufsz,
                                                      const char* format,
                                                      const util_Fmts vals)
{
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

static void util_RegisterEventHandlerLog(void (*callback)(const util_Log))
{
    if (callback == NULL)
        gEventHandlersUtil.panic();
    gEventHandlersUtil.log = callback;
}

static void util_RegisterEventHandlerPanic(void (*callback)(void))
{
    if (callback == NULL)
        gEventHandlersUtil.panic();
    gEventHandlersUtil.panic = callback;
}

#endif // UTIL_C
