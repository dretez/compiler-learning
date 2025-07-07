#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "Utils/Tree.h"
#include "Utils/TreeList.h"

Tree Tree_init() {
    return (Tree){
        .data = NULL,
        .parent = NULL,
        .children = TreeList_init(),
    };
}

/***************************** MEMORY MANAGEMENT *****************************/

Tree *Tree_new() {
    Tree *tree = malloc(sizeof(Tree));
    if (tree == NULL)
        return NULL;
    *tree = Tree_init();
    return tree;
}

void Tree_clear(Tree *tree) {
    if (tree == NULL)
        return;
    TreeList_clear(&tree->children);
    Tree_removeChild(tree->parent, tree);
    *tree = Tree_init();
}

void Tree_free(Tree *root) {
    for (uint i = 0; i < root->children.count; i++) {
        root->children.list[i]->parent = NULL;
        Tree_free(root->children.list[i]);
    }
    if (root->parent != NULL) {
        TreeList_mvTreeToEnd(&root->parent->children, root);
    }
    Tree_freeNode(root);
}

TreeList Tree_freeNode(Tree *node) {
    Tree *gramps = node->parent;
    for (uint i = 0; i < node->children.count; i++) {
        node->children.list[i]->parent = gramps;
    }
    TreeList childrenCopy = node->children;
    free(node);
    return childrenCopy;
}

/********************************** GETTERS **********************************/

uint Tree_childCount(Tree tree) { return tree.children.count; }

Tree *Tree_getChild(Tree tree, uint idx) {
    if (tree.children.count < idx)
        return NULL;
    return tree.children.list[idx];
}

TreeList *Tree_getChildren(Tree *tree) { return &tree->children; }

Tree *Tree_getParent(Tree tree) { return tree.parent; }

void *Tree_getData(Tree tree) { return tree.data; }

/******************************************************************************/

void Tree_addChild(Tree *root, Tree *child) {
    child->parent = root;
    TreeList_add(&root->children, child);
}

void Tree_removeChild(Tree *parent, Tree *child) {
    if (parent == NULL || child == NULL)
        return;
    TreeList_remove(&parent->children, child);
}

/********************************** PRINTING **********************************/

void Tree_fprint(FILE *stream,
                 void (*valuePrinter)(void *),
                 Tree tree,
                 uint depth) {
    for (uint i = 0; i < depth; i++)
        fprintf(stream, "  ");
    valuePrinter(tree.data);
    fprintf(stream, "Parent: %p\n", Tree_getParent(tree));
    TreeList_fprint(stream, valuePrinter, tree.children, depth + 1);
}
void Tree_fprintNode(FILE *stream, void (*valuePrinter)(void *), Tree tree) {
    valuePrinter(tree.data);
    fprintf(stream, "Parent: %p;", Tree_getParent(tree));
    fprintf(stream, "Child count: %d\n", Tree_childCount(tree));
}
void Tree_print(Tree tree, void (*valuePrinter)(void *)) {
    Tree_fprint(stdout, valuePrinter, tree, 0);
}
void Tree_printNode(Tree tree, void (*valuePrinter)(void *)) {
    Tree_fprintNode(stdout, valuePrinter, tree);
}
