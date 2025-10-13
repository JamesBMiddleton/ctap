#ifndef TAP_LIST_H
#define TAP_LIST_H

#include "tap_def.h"

typedef struct TapList_ {
    struct TapList_ *prev, *next;
} TapList;

static void tap_list_init(TapList *node);
static void tap_list_insert(TapList *node, TapList *new);
static void tap_list_remove(TapList *node);
static void tap_list_clear(TapList *anchor);
#define tap_list_for_each(anchor, iter) for ((iter) = (anchor)->next; (iter) != (anchor); (iter) = (iter)->next)
#define tap_list_for_each_safe(anchor, iter, itertmp) \
    for ((iter) = (anchor)->next, (itertmp) = (iter)->next; (iter) != (anchor); (iter) = (itertmp), (itertmp) = (iter)->next)

void tap_list_init(TapList *node) 
{ 
    /* Ensure the node isn't already part of a list */
    tap_def_assert(node->prev == NULL || node->prev == node);
    tap_def_assert(node->next == NULL || node->next == node);
    node->prev = node; node->next = node; 
}

void tap_list_insert(TapList *node, TapList *new)
{	
    /* Ensure the new node isn't already part of a list */
    tap_def_assert(new->prev == NULL || new->prev == new);
    tap_def_assert(new->next == NULL || new->next == new);
	new->next = node->next;
	new->prev = node;
    node->next->prev = new;
	node->next = new;
}

void tap_list_remove(TapList *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = NULL;
    node->next = NULL;
}

void tap_list_clear(TapList *anchor)
{
    TapList *iter, *itertmp;
    tap_list_for_each_safe(anchor, iter, itertmp)
    {
        tap_list_remove(iter);
    }
}

#endif /* TAP_LIST_H */
