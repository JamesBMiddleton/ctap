#ifndef CTP_LOG_H
#define CTP_LOG_H

extern int (*ctp_log_printf) (const char* format, ...); 

#define CTP_LOG_TOSTRING_(s) #s
#define CTP_LOG_TOSTRING(s) CTP_LOG_TOSTRING_(s)
#define CTP_LOG_LINENUM_AS_STRING CTP_LOG_TOSTRING(__LINE__)

#ifdef DEBUG
#define CTP_LOG_DEBUG(msg)                                      \
    do { ctp_log_printf("DEBUG | " __FILE__ ":" CTP_LOG_LINENUM_AS_STRING " | " #msg "\n" ); } while (0)
#define CTP_LOGF_DEBUG(msg, ...)                                      \
    do { ctp_log_printf("DEBUG | " __FILE__ ":" CTP_LOG_LINENUM_AS_STRING " | " #msg "\n", __VA_ARGS__); } while (0)
#else
#define CTP_LOG_DEBUG(...)
#define CTP_LOGF_DEBUG(...)
#endif

#endif // CTP_LOG_H
