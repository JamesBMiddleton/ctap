#ifndef TAP_ERRNO_H
#define TAP_ERRNO_H

typedef enum {
    TAP_ERRNO_OK,
    TAP_ERRNO_NULLPTR,
    TAP_ERRNO_NOMEM,
    TAP_ERRNO_MODULE_UNINITIALIZED,
    TAP_ERRNO_INVALID_ALLOCATOR,
    TAP_ERRNO_INVALID_LOGGER,
    TAP_ERRNO_INVALID_FILEPATH,
    TAP_ERRNO_INVALID_FILE_DESCRIPTOR,
    TAP_ERRNO_READ_FAILED,
    TAP_ERRNO_LSEEK_FAILED,
    TAP_ERRNO_FORMAT_ERROR,
    TAP_ERRNO_INVALID_VALUE,
} TapErrno;

void tap_errno_set(TapErrno err);
TapErrno tap_errno_get(void);
void tap_errno_source_set(const char *source);
const char *tap_errno_source_get(void);

#ifdef TAP_ERRNO_IMPLEMENTATION
static TapErrno errno_value;
static const char *errno_source;
void tap_errno_set(const TapErrno err)
{
    errno_value = err;
}
TapErrno tap_errno_get(void)
{
    return errno_value;
}
void tap_errno_source_set(const char *source)
{
    errno_source = source;
}
const char *tap_errno_source_get(void)
{
    return errno_source;
}
#endif /* TAP_ERRNO_IMPLEMENTATION */

#endif /* TAP_ERRNO_H */
