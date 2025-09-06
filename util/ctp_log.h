#ifndef CTP_LOG_H
#define CTP_LOG_H

int (*ctp_log_get_printf(void)) (const char* format, ...);
void ctp_log_set_printf(int (*fn)(const char* format, ...));

#define CTP_LOG_TOSTRING_(s) #s
#define CTP_LOG_TOSTRING(s) CTP_LOG_TOSTRING_(s)
#define CTP_LOG_LINENUM_AS_STRING CTP_LOG_TOSTRING(__LINE__)

#ifdef DEBUG
#define CTP_LOG(msg)                                      \
    do { ctp_log_get_printf()(__FILE__ ":" CTP_LOG_LINENUM_AS_STRING " | " #msg "\n" ); } while (0)
#define CTP_LOGF(msg, ...)                                      \
    do { ctp_log_get_printf()(__FILE__ ":" CTP_LOG_LINENUM_AS_STRING " | " #msg "\n", __VA_ARGS__); } while (0)
#else
#define CTP_LOG(...)
#define CTP_LOGF(...)
#endif

#ifdef CTP_LOG_DEFINE
static int (*log_printf) (const char* format, ...);
int (*ctp_log_get_printf(void)) (const char* format, ...) { return log_printf; }
void ctp_log_set_printf(int (*fn)(const char* format, ...)) { if (fn) log_printf = fn; }
#endif // CTP_LOG_DEFINE

#endif // CTP_LOG_H
