#include <stdlib.h>

#include "Utils/String.h"
#include "Utils/Tree.h"
#include "Utils/TreeList.h"

Tree newTree() {
    return (Tree){
        newEmptyString(),
        NULL,
        newTreeList(),
    };
}

int treeAddNewChild(Tree *root, Tree child) {
    child.parent = root;
    return treeListAddTree(&root->children, child);
}

Tree *mallocTree(Tree orig) {
    Tree *tree = malloc(sizeof(Tree));
    if (tree == NULL)
        return NULL;
    *tree = orig;
    return tree;
}

void freeTree(Tree *root) {
    for (uint i = 0; i < root->children.count; i++) {
        root->children.list[i]->parent = NULL;
        freeTree(root->children.list[i]);
    }
    if (root->parent != NULL) {
        treeListMoveTreeToEnd(&root->parent->children, root);
    }
    freeTreeNode(root);
}
TreeList freeTreeNode(Tree *node) {
    for (uint i = 0; i < node->children.count; i++) {
        node->children.list[i]->parent = NULL;
    }
    TreeList childrenCopy = node->children;
    freeString(&node->label);
    free(node);
    return childrenCopy;
}
