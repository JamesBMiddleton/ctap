
#define util_MAX_LOG_SZ 256

typedef enum {
    util_LogLvl_DEBUG,
    util_LogLvl_WARN,
} util_LogLvl;

typedef struct {
    util_LogLvl lvl;
    uint lineNum;
    const char* funcName;
    const char* message;
} util_Log;


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
