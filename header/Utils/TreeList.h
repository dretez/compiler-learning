#ifndef INCLUDE_UTILS_TREELIST_H
#define INCLUDE_UTILS_TREELIST_H

#include <sys/types.h>

typedef struct tree Tree;

typedef struct treelist {
    Tree **list;
    uint count;
    uint alloced; // size in bytes = alloced * sizeof(Tree)
} TreeList;

TreeList newTreeList();

int treeListAddTree(TreeList *tl, Tree value);

void treeListMoveTreeToEnd(TreeList *tl, Tree *tree);
void treeListRemoveLast(TreeList *tl);

#endif // !INCLUDE_UTILS_TREELIST_H
