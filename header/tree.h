#ifndef INCLUDE_HEADER_TREE
#define INCLUDE_HEADER_TREE

#include <sys/types.h>

typedef struct tree {
    char *label;
    uint label_len;
    struct tree **child;
    uint child_count;
} Tree;

Tree *newTree(char *label, uint label_len);
void treeAddChild(Tree *root, Tree *child);
Tree *newDigitNode(char *c);
void printTree(Tree *root, uint depth);

#endif // !INCLUDE_HEADER_TREE
