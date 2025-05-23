#include <stdlib.h>

#include "Utils/Tree.h"
#include "Utils/TreeList.h"

TreeList newTreeList() {
    return (TreeList){
        NULL,
        0,
        0,
    };
}

int treeListAddTree(TreeList *tl, Tree value) {
    Tree *new = mallocTree(value);
    if (new == NULL)
        return -1;

    if (tl->count >= tl->alloced) {
        Tree **aux = tl->list;
        tl->list = realloc(tl->list, sizeof(Tree *) * (1 + tl->count));
        if (tl->list == NULL) {
            tl->list = aux;
            free(new);
            return -1;
        }
        tl->alloced = tl->count + 1;
    }
    tl->list[tl->count] = new;
    tl->count++;
    return tl->count;
}

void treeListRemoveTree(TreeList *tl, Tree *tree) {
    treeListMoveTreeToEnd(tl, tree);
    treeListRemoveLast(tl);
}
void treeListMoveTreeToEnd(TreeList *tl, Tree *tree) {
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
void treeListRemoveLast(TreeList *tl) {
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
