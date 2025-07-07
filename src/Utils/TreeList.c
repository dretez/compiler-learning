#include <stdio.h>
#include <stdlib.h>

#include "Utils/Tree.h"
#include "Utils/TreeList.h"

TreeList TreeList_init() {
    return (TreeList){
        .list = NULL,
        .count = 0,
        .alloced = 0,
    };
}

TreeList *TreeList_new() {
    TreeList *list = malloc(sizeof(TreeList));
    if (list == NULL)
        return NULL;
    *list = TreeList_init();
    return list;
}

void TreeList_clear(TreeList *list) {
    if (list == NULL || list->alloced == 0)
        return;
    for (uint i = 0; i < list->count; i++)
        list->list[i]->parent = NULL;
    free(list->list);
    *list = TreeList_init();
}

void TreeList_free(TreeList **list) {
    if (*list == NULL)
        return;
    TreeList_clear(*list);
    free(*list);
    *list = NULL;
}

void TreeList_freeItems(TreeList *list) {
    if (list == NULL)
        return;
    for (uint i = 0; i < list->count; i++)
        Tree_free(list->list[i]);
    TreeList_clear(list);
}

/******************************************************************************/

int TreeList_add(TreeList *tl, Tree *value) {
    if (value == NULL)
        return -1;

    if (tl->count >= tl->alloced) {
        Tree **aux = tl->list;
        tl->list = realloc(tl->list, sizeof(Tree *) * (1 + tl->count));
        if (tl->list == NULL) {
            tl->list = aux;
            return -1;
        }
        tl->alloced = tl->count + 1;
    }
    tl->list[tl->count] = value;
    tl->count++;
    return tl->count;
}

void TreeList_remove(TreeList *list, Tree *tree) {
    if (list == NULL)
        return;
    uint i = 0;
    while (list->list[i] != tree && i < list->count)
        i++;
    if (i == list->count) // tree doesn't exist in list.
        return;
    for (; i < list->count - 1; i++)
        list->list[i] = list->list[i + 1];
    list->count--;
    Tree **aux = list->list;
    list->list = realloc(list->list, sizeof(Tree *) * (list->count));
    if (list == NULL) {
        list->list = aux;
        return;
    }
    list->alloced = list->count;
}

void TreeList_mvTreeToEnd(TreeList *tl, Tree *tree) {
    uint i = 0;
    while (tl->list[i] != tree && i < tl->count)
        i++;
    if (i == tl->count)
        return;
    Tree *aux = tl->list[i];
    for (; i < tl->count - 1; i++)
        tl->list[i] = tl->list[i + 1];
    tl->list[i] = aux;
}

void TreeList_rmLast(TreeList *tl) {
    if (tl->count == 0)
        return;
    tl->count--;
    Tree **aux = tl->list;
    tl->list = realloc(tl->list, sizeof(Tree *) * (tl->count));
    if (tl->list == NULL) {
        tl->list = aux;
        return;
    }
    tl->alloced = tl->count;
}

void TreeList_runMethod(TreeList *list, void (*method)(Tree *)) {
    for (uint i = 0; i < list->count; i++)
        method(list->list[i]);
}

/******************************************************************************/

void TreeList_fprint(FILE *stream,
                     void (*valuePrinter)(void *),
                     TreeList list,
                     uint depth) {
    for (uint i = 0; i < list.count; i++) {
        Tree_fprint(stream, valuePrinter, *list.list[i], depth);
    }
}
