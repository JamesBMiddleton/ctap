#ifndef TAP_ASSERT_H
#define TAP_ASSERT_H

#include "tap_log.h"

#ifdef TAP_DEBUG
    #define TAP_ASSERT(cond) do { if (!(cond)) { TAP_LOG1("Assertion failed: %s", #cond); *(volatile int *)0 = 0; }} while (0)
#else
    #define TAP_ASSERT(cond)
#endif

#define TAP_ASSERT_STATIC(cond, msg) typedef char tap_def_static_assert_##msg[(cond) ? 1 : -1]

#endif /* TAP_ASSERT_H */
