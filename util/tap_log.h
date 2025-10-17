#ifndef TAP_LOG_H
#define TAP_LOG_H

int (*tap_log_get_printf(void)) (const char* format, ...);
void tap_log_set_printf(int (*fn)(const char* format, ...));

#define TAP_LOG_TOSTRING_(s) #s
#define TAP_LOG_TOSTRING(s) TAP_LOG_TOSTRING_(s)

#define TAP_LOG(msg) do { if (tap_log_get_printf()) { tap_log_get_printf()(__FILE__ ":" TAP_LOG_TOSTRING(__LINE__) " | " msg "\n" ); } } while (0)
#define TAP_LOG1(msg, one) do { if (tap_log_get_printf()) { tap_log_get_printf()(__FILE__ ":" TAP_LOG_TOSTRING(__LINE__) " | " msg "\n", one); } } while (0)

#ifdef TAP_LOG_DEFINE
static int (*log_printf) (const char* format, ...);
int (*tap_log_get_printf(void)) (const char* format, ...) { return log_printf; }
void tap_log_set_printf(int (*fn)(const char* format, ...)) { log_printf = fn; }
#endif /* TAP_LOG_DEFINE */

#endif /* TAP_LOG_H */
