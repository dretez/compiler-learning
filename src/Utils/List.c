#include "Utils/List.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define NULL_PTR_GUARD(ptr)                                                    \
    if (ptr == NULL)                                                           \
    return
#define LIST_IDX_OVERFLOW_GUARD(list, idx)                                     \
    if (idx > list.count)                                                      \
    return
#define LIST_CHECK_FLAG(list, flag) (list.flags ^ flag)
#define LIST_SET_FLAG(list, flag) (list->flags |= flag)

typedef struct list {
    void **data;
    size_t count;
    size_t alloced;
    uint8_t flags;

    void (*freeItemFcn)(void **);
    void (*sortingFunc)(List *);
} List;

/*****************************************************************************/

void List_defaultFreeItemFunc(void **);

/*****************************************************************************/

List List_init() {
    return (List){
        .data = NULL,
        .count = 0,
        .alloced = 0,
        .flags = 0x00,

        .freeItemFcn = List_defaultFreeItemFunc,
    };
}

List *List_new() {
    List *list = malloc(sizeof(List));
    if (list == NULL)
        return NULL;
    *list = List_init();
    return list;
}

void List_clear(List *list) {
    NULL_PTR_GUARD(list);
    NULL_PTR_GUARD(list->data);
    List_runFunction(list, list->freeItemFcn);
    free(list->data);
    *list = List_init();
}

void List_free(List **list) {
    NULL_PTR_GUARD(list);
    List_clear(*list);
    free(*list);
    *list = NULL;
}

/*****************************************************************************/

void List_setFreeItemFunc(List *list, void (*func)(void **)) {
    NULL_PTR_GUARD(list);
    list->freeItemFcn = func;
}

void *List_get(List *list, size_t idx) {
    LIST_IDX_OVERFLOW_GUARD((*list), idx) NULL;
    return idx >= list->count ? NULL : list->data[idx];
}

size_t List_getIdxOf(List *list, void *item) {
    size_t i = 0;
    while (i < list->count && list->data[i] != item)
        i++;
    return i;
}

size_t List_getSize(List *list) {
    return list->count;
}

/*****************************************************************************/

void List_add(List *list, void *item) {
    NULL_PTR_GUARD(list);
    NULL_PTR_GUARD(item);
    if (List_contains(list, item))
        return;

    if (list->count >= list->alloced) {
        void **aux = list->data;
        list->data = realloc(list->data, sizeof(void **) * (list->count + 1));
        if (list->data == NULL) {
            list->data = aux;
            return;
        }
        list->alloced = list->count + 1;
    }
    list->data[list->count] = item;
    list->count++;
}

void List_rmItem(List *list, void *item) {
    NULL_PTR_GUARD(list);
    NULL_PTR_GUARD(item);
    size_t idx = List_getIdxOf(list, item);
    LIST_IDX_OVERFLOW_GUARD((*list), idx);

    List_rmIdx(list, idx);
}

void List_rmIdx(List *list, size_t idx) {
    NULL_PTR_GUARD(list);
    LIST_IDX_OVERFLOW_GUARD((*list), idx);

    if (LIST_CHECK_FLAG((*list), LIST_FLAGS_KEEP_SORTED))
        for (size_t i = idx; i < list->count - 1; i++)
            list->data[i] = list->data[i + 1];
    else
        list->data[idx] = list->data[list->count - 1];
    list->count--;

    void *aux = list->data;
    list->data = realloc(list->data, sizeof(void *) * list->count);
    if (list->data == NULL) {
        list->data = aux;
        return;
    }
    list->alloced = list->count;
}

void *List_pop(List *list) {
    NULL_PTR_GUARD(list) NULL;

    size_t idx = List_getSize(list) - 1;
    void *item = List_get(list, idx);
    List_rmIdx(list, idx);
    return item;
}

/*****************************************************************************/

int List_contains(List *list, void *item) {
    return List_getIdxOf(list, item) != list->count;
}

void List_runFunction(List *list, void (*func)(void **)) {
    for (size_t i = 0; i < list->count; i++)
        func(&(list->data[i]));
}

/*****************************************************************************/

void List_defaultFreeItemFunc(void **itemPtr) {
    NULL_PTR_GUARD(itemPtr);
    NULL_PTR_GUARD(*itemPtr);
    free(*itemPtr);
    *itemPtr = NULL;
}

#undef NULL_PTR_GUARD
#undef LIST_IDX_OVERFLOW_GUARD
#undef LIST_CHECK_FLAG
#undef LIST_SET_FLAG
