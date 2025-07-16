#include "Utils/Tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "Utils/List.h"

#define NULL_PTR_GUARD(ptr)                                                    \
    if (ptr == NULL)                                                           \
    return

typedef struct tree {
    void *data;
    Tree *parent;
    List *children;

    void (*freeDataFunc)(void **);
} Tree;

void Tree_defaultFreeDataFunc(void **);
void Tree_AUX_freeChild(Tree **child);

void Tree_reset(Tree *tree) {
    NULL_PTR_GUARD(tree);
    tree->data = NULL;
    tree->parent = NULL;
}

/***************************** MEMORY MANAGEMENT *****************************/

Tree *Tree_new() {
    List *list = List_new();
    NULL_PTR_GUARD(list) NULL;
    List_setFreeItemFunc(list, (void (*)(void **))Tree_free);

    Tree *tree = malloc(sizeof(Tree));
    NULL_PTR_GUARD(tree) NULL;
    Tree_reset(tree);
    tree->children = list;
    tree->freeDataFunc = Tree_defaultFreeDataFunc;
    return tree;
}

void Tree_clear(Tree *tree) {
    NULL_PTR_GUARD(tree);
    List_clear(tree->children);
    Tree_removeChild(tree->parent, tree);
    Tree_reset(tree);
}

void Tree_free(Tree **root) {
    NULL_PTR_GUARD(root);
    NULL_PTR_GUARD(*root);
    List_runFunction((*root)->children, (void (*)(void **))Tree_AUX_freeChild);
    if ((*root)->parent != NULL)
        Tree_removeChild((*root)->parent, *root);
    Tree_freeNode(root);
}

List *Tree_freeNode(Tree **node) {
    NULL_PTR_GUARD(node) NULL;
    Tree *gramps = (*node)->parent;
    for (size_t i = 0; i < List_getSize((*node)->children); i++) {
        Tree *aux = List_get((*node)->children, i);
        Tree_setParent(aux, gramps);
    }
    List *childrenCopy = (*node)->children;
    (*node)->freeDataFunc(&(*node)->data);
    free(*node);
    *node = NULL;
    return childrenCopy;
}

/********************************** GETTERS **********************************/

uint Tree_childCount(Tree *tree) {
    NULL_PTR_GUARD(tree) 0;
    return List_getSize(tree->children);
}

Tree *Tree_getChild(Tree *tree, uint idx) {
    NULL_PTR_GUARD(tree) NULL;
    if (List_getSize(tree->children) <= idx)
        return NULL;
    return List_get(tree->children, idx);
}

List *Tree_getChildren(Tree *tree) {
    NULL_PTR_GUARD(tree) NULL;
    return tree->children;
}

Tree *Tree_getParent(Tree *tree) {
    NULL_PTR_GUARD(tree) NULL;
    return tree->parent;
}

void *Tree_getData(Tree *tree) {
    NULL_PTR_GUARD(tree) NULL;
    return tree->data;
}

/******************************************************************************/

void Tree_setFreeItemFunc(Tree *tree, void (*func)(void **)) {
    NULL_PTR_GUARD(tree);
    NULL_PTR_GUARD(func);
    tree->freeDataFunc = func;
}

void Tree_setParent(Tree *tree, Tree *parent) {
    NULL_PTR_GUARD(tree);
    Tree_removeChild(Tree_getParent(tree), tree);
    Tree_addChild(parent, tree);
}

void Tree_setData(Tree *tree, void *data) {
    NULL_PTR_GUARD(tree);
    tree->data = data;
}

void Tree_addChild(Tree *root, Tree *child) {
    NULL_PTR_GUARD(root);
    NULL_PTR_GUARD(child);
    child->parent = root;
    List_add(root->children, child);
}

void Tree_removeChild(Tree *parent, Tree *child) {
    NULL_PTR_GUARD(parent);
    NULL_PTR_GUARD(child);
    List_rmItem(parent->children, child);
}

/********************************** PRINTING **********************************/

void Tree_fprint(FILE *stream,
                 void (*valuePrinter)(void *),
                 Tree *tree,
                 uint depth) {
    NULL_PTR_GUARD(tree);
    for (uint i = 0; i < depth; i++)
        fprintf(stream, "  ");
    if (valuePrinter != NULL)
        valuePrinter(tree->data);
    fprintf(stream, "Parent: %p\n", Tree_getParent(tree));
    for (size_t i = 0; i < List_getSize(tree->children); i++) {
        Tree *child = List_get(tree->children, i);
        Tree_fprint(stream, valuePrinter, child, depth + 1);
    }
}
void Tree_fprintNode(FILE *stream, void (*valuePrinter)(void *), Tree *tree) {
    NULL_PTR_GUARD(tree);
    if (valuePrinter != NULL)
        valuePrinter(tree->data);
    fprintf(stream, "Parent: %p;", Tree_getParent(tree));
    fprintf(stream, "Child count: %d\n", Tree_childCount(tree));
}
void Tree_print(Tree *tree, void (*valuePrinter)(void *)) {
    Tree_fprint(stdout, valuePrinter, tree, 0);
}
void Tree_printNode(Tree *tree, void (*valuePrinter)(void *)) {
    Tree_fprintNode(stdout, valuePrinter, tree);
}

/******************************************************************************/

void Tree_defaultFreeDataFunc(void **data) {
    NULL_PTR_GUARD(data);
    NULL_PTR_GUARD(*data);
    free(*data);
    *data = NULL;
}

void Tree_AUX_freeChild(Tree **child) {
    NULL_PTR_GUARD(child);
    NULL_PTR_GUARD(*child);
    (*child)->parent = NULL;
    Tree_free(child);
}

#undef NULL_PTR_GUARD
