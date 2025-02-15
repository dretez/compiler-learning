#ifndef INCLUDE_HEADER_TREE
#define INCLUDE_HEADER_TREE

#include <sys/types.h>

typedef struct tree {
    char *label;
    uint label_len;
    struct tree **child;
    uint child_count;
    struct tree *parent;
} Tree;

Tree *newTree(char *label, uint label_len);
void treeAddChild(Tree *root, Tree *child);
/* WARN: This function does not free the child node, nor does it free it's own
 * children, it only serves for the purpose of cleaning dangling pointers from a
 * tree
 */
void treeRemoveChild(Tree *root, Tree *child);
int getChildPos(Tree *root, Tree *child);
Tree *newDigitNode(char *c);
void printTree(Tree *root, uint depth);
/* This function frees an entire tree by using recursion.
 * If root has a parent, this will leave the parent with a dangling pointer to
 * the root and a wrong child count */
void freeTreeRec(Tree *root);
void freeTree(Tree *root);

#endif // !INCLUDE_HEADER_TREE
