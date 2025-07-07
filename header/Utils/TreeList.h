#ifndef INCLUDE_UTILS_TREELIST_H
#define INCLUDE_UTILS_TREELIST_H

#include <stdio.h>
#include <sys/types.h>

typedef struct tree Tree;

typedef struct treelist {
    Tree **list;
    uint count;
    uint alloced; // size in bytes = alloced * sizeof(Tree)
} TreeList;

TreeList TreeList_init();
TreeList *TreeList_new();

void TreeList_clear(TreeList *);
void TreeList_free(TreeList **);
void TreeList_freeItems(TreeList *);

/**
 * Run a method on each {@code Tree} of a {@code TreeList}
 *
 * @param list Target list
 * @param Method to be run
 */
void TreeList_runMethod(TreeList *list, void (*method)(Tree *));

void TreeList_mvTreeToEnd(TreeList *tl, Tree *tree);
void TreeList_rmLast(TreeList *tl);

int TreeList_add(TreeList *tl, Tree *value);
void TreeList_remove(TreeList *, Tree *);

void TreeList_fprint(FILE *stream,
                     void (*valuePrinter)(void *),
                     TreeList list,
                     uint depth);

#endif // !INCLUDE_UTILS_TREELIST_H
