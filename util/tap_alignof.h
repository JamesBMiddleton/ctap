#ifndef TAP_ALIGNOF_H
#define TAP_ALIGNOF_H

#include <stddef.h>

#define tap_alignof(type) ((size_t)(offsetof(struct { char c; type d; }, d)))

#endif /* TAP_ALIGNOF_H */
