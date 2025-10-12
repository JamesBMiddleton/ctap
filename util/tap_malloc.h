#ifndef TAP_MALLOC_H
#define TAP_MALLOC_H

#ifndef TAP_MALLOC
    #include <stdlib.h>
    #define TAP_MALLOC(bytes) malloc(bytes)
#endif

#endif /* TAP_ARENA_H */
