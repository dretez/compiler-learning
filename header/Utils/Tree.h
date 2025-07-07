#ifndef INCLUDE_UTILS_TREE_H
#define INCLUDE_UTILS_TREE_H

#include <stdio.h>
#include <sys/types.h>

#include "Utils/TreeList.h"

typedef struct tree Tree;

struct tree {
    void *data;
    Tree *parent;
    TreeList children;
};

Tree Tree_init();
Tree *Tree_new();
void Tree_clear(Tree *);
void Tree_free(Tree *root);
TreeList Tree_freeNode(Tree *node);

uint Tree_childCount(Tree);
Tree *Tree_getChild(Tree, uint idx);
TreeList *Tree_getChildren(Tree *);
Tree *Tree_getParent(Tree);
void *Tree_getData(Tree);

void Tree_addChild(Tree *node, Tree *child);
void Tree_removeChild(Tree *node, Tree *child);

void Tree_print(Tree tree, void (*valuePrinter)(void *));
void Tree_fprint(FILE *stream, void (*valuePrinter)(void *), Tree tree,
                 uint depth);
void Tree_printNode(Tree tree, void (*valuePrinter)(void *));
void Tree_fprintNode(FILE *stream, void (*valuePrinter)(void *), Tree tree);

#endif // ! INCLUDE_UTILS_TREE_H
