#ifndef TAP_LIST_H
#define TAP_LIST_H

typedef struct TapListHead_ {
    struct TapListHead_ *next, *prev;
} TapListHead;

// tap_list_init()
// tap_list_move()
// tap_list_swap()
// #define tap_list_for_each()
// ...
//
#endif // TAP_LIST_H
