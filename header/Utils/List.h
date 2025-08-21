#ifndef INCLUDE_UTIL_LIST_H
#define INCLUDE_UTIL_LIST_H

#include <stddef.h>

#define LIST_FLAGS_IS_SORTED 0x01
#define LIST_FLAGS_KEEP_SORTED 0x02

typedef struct list List;

List List_init();
List *List_new();
void List_clear(List *list);
void List_free(List **list);

void List_setFreeItemFunc(List *list, void (*func)(void **));
void *List_get(List *list, size_t idx);
size_t List_getIdxOf(List *list, void *item);
size_t List_getSize(List *list);

void List_add(List *list, void *item);
void List_rmItem(List *list, void *item);
void List_rmIdx(List *list, size_t idx);
void *List_pop(List *list);

int List_contains(List *list, void *item);

void List_runFunction(List *list, void (*func)(void **));

#endif /* ifndef INCLUDE_UTIL_LIST_H */
