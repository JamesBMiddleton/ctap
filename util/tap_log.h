#ifndef TAP_LOG_H
#define TAP_LOG_H

#include "tap_def.h"

typedef struct {
    int (*printf) (const char* format, ...);
} TapLogLogger;
void tap_log_set_logger(TapLogLogger log);

int (*tap_log_get_printf(void)) (const char* format, ...);
#define TAP_LOG(msg) do { if (tap_log_get_printf()) { tap_log_get_printf()(__FILE__ ":" TAP_DEF_TOSTRING(__LINE__) " | " msg "\n" ); } } while (0)
#define TAP_LOG1(msg, one) do { if (tap_log_get_printf()) { tap_log_get_printf()(__FILE__ ":" TAP_DEF_TOSTRING(__LINE__) " | " msg "\n", one); } } while (0)

#ifdef TAP_LOG_IMPLEMENTATION
static TapLogLogger log_logger;
void tap_log_set_logger(TapLogLogger logger) { log_logger = logger; }
int (*tap_log_get_printf(void)) (const char* format, ...) { return log_logger.printf; }
#endif /* TAP_LOG_IMPLEMENTATION */

#endif /* TAP_LOG_H */
