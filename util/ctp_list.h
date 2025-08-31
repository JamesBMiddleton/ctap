#ifndef CTP_LIST_H
#define CTP_LIST_H

#if defined(__clang__) || defined(__GNUC__)
#define container_of_const(ptr, type, member)                                  \
    ((const type*)((const char*)(__typeof__(((const type*)0)->member)*){ptr} - \
                   ((size_t) & (((type*)0)->member))))
#else
#define container_of_const(ptr, type, member) \
    ((const type*)((const char*)(ptr) - ((size_t) & (((type*)0)->member))))
#endif

struct list_head {
	struct list_head *next, *prev;
};

// ctp_list_init()
// ctp_list_move()
// ctp_list_swap()
// ctp_list_for_each()
// ...
//
#endif // CTP_LIST_H
