#ifndef INCLUDE_UTILS_TREE_H
#define INCLUDE_UTILS_TREE_H

#include <sys/types.h>

#include "Utils/String.h"
#include "Utils/TreeList.h"

typedef struct tree Tree;

struct tree {
    String label;
    Tree *parent;
    TreeList children;
};

Tree newTree();

int treeAddNewChild(Tree *root, Tree child);

Tree *mallocTree(Tree orig);
void freeTree(Tree *root);
TreeList freeTreeNode(Tree *node);

#endif // ! INCLUDE_UTILS_TREE_H
