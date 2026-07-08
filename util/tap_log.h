#ifndef TAP_LOG_H
#define TAP_LOG_H

#include <stdio.h>

#define TAP_LOG(msg, ...)                                                            \
    do                                                                               \
    {                                                                                \
        printf(__FILE__ ":" TAP_DEF_TOSTRING(__LINE__) " | " msg "\n", __VA_ARGS__); \
    } while (0)

#endif /* TAP_LOG_H */
