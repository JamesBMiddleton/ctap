#ifndef TAP_ERRNO_H
#define TAP_ERRNO_H

typedef enum {
    TAP_ERRNO_OK,
    TAP_ERRNO_NULL,
    TAP_ERRNO_NOMEM,
    TAP_ERRNO_WARN_FRAME_SKIPPED
} TapErrno;

void tap_errno_set(TapErrno err);
TapErrno tap_errno_get(void);
void tap_errno_source_set(const char *source);
const char *tap_errno_source_get(void);

#ifdef TAP_ERRNO_DEFINE
static TapErrno tap_errno_value;
static const char *tap_errno_source;
void tap_errno_set(const TapErrno err) { tap_errno_value = err; }
TapErrno tap_errno_get(void) { return tap_errno_value; }
void tap_errno_source_set(const char *source) { tap_errno_source = source; }
const char *tap_errno_source_get(void) { return tap_errno_source; }
#endif /* TAP_ERRNO_DEFINE */

#endif /* TAP_ERRNO_H */
