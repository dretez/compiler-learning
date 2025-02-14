#include "../header/tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tree *newTree(char *label, uint label_len) {
    Tree *new = malloc(sizeof(Tree));
    new->label = malloc(sizeof(char) * label_len);
    memcpy(new->label, label, sizeof(char) * label_len);
    new->label_len = label_len;
    new->child = NULL;
    new->child_count = 0;
    return new;
}

void treeAddChild(Tree *root, Tree *child) {
    Tree **aux = root->child;
    root->child =
        realloc(root->child, sizeof(Tree *) * (root->child_count + 1));
    if (root->child == NULL) {
        root->child = aux;
        return;
    }
    root->child[root->child_count] = child;
    root->child_count++;
}

Tree *newDigitNode(char *c) {
    Tree *num = newTree(c, 1);
    Tree *digit = newTree("digit", 5);
    treeAddChild(digit, num);
    return digit;
}

void printTree(Tree *root, uint depth) {
    for (uint i = 0; i < depth; i++) {
        printf("    ");
    }
    for (uint i = 0; i < root->label_len; i++) {
        putc(root->label[i], stdout);
    }
    putc('\n', stdout);
    if (root->child_count == 0) {
    }
    for (uint i = 0; i < root->child_count; i++) {
        printTree(root->child[i], depth + 1);
    }
}
