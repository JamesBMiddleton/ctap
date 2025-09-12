#ifndef TAP_LOG_H
#define TAP_LOG_H

int (*tap_log_get_printf(void)) (const char* format, ...);
void tap_log_set_printf(int (*fn)(const char* format, ...));

#define TAP_LOG_TOSTRING_(s) #s
#define TAP_LOG_TOSTRING(s) TAP_LOG_TOSTRING_(s)
#define TAP_LOG_LINENUM_AS_STRING TAP_LOG_TOSTRING(__LINE__)

#ifdef DEBUG
#define TAP_LOG(msg)                                      \
    do { tap_log_get_printf()(__FILE__ ":" TAP_LOG_LINENUM_AS_STRING " | " #msg "\n" ); } while (0)
#define TAP_LOGF(msg, ...)                                      \
    do { tap_log_get_printf()(__FILE__ ":" TAP_LOG_LINENUM_AS_STRING " | " #msg "\n", __VA_ARGS__); } while (0)
#else
#define TAP_LOG(...)
#define TAP_LOGF(...)
#endif

#ifdef TAP_LOG_DEFINE
static int (*log_printf) (const char* format, ...);
int (*tap_log_get_printf(void)) (const char* format, ...) { return log_printf; }
void tap_log_set_printf(int (*fn)(const char* format, ...)) { if (fn) log_printf = fn; }
#endif // TAP_LOG_DEFINE

#endif // TAP_LOG_H
