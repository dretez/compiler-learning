#include "../header/tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getChildPos(Tree *root, Tree *child) {
    int i = 0;
    for (i = root->child_count - 1; i >= 0; i--) {
        if (root->child[i] == child)
            break;
    }
    return i;
}

Tree *newTree(char *label, uint label_len) {
    Tree *new = malloc(sizeof(Tree));
    if (new == NULL) {
        printf("Error allocating memory for new node");
        exit(EXIT_FAILURE); // panic
    }
    new->label = malloc(sizeof(char) * label_len);
    memcpy(new->label, label, sizeof(char) * label_len);
    new->label_len = label_len;
    new->child = NULL;
    new->child_count = 0;
    new->parent = NULL;
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
    child->parent = root;
    root->child_count++;
}

void treeRemoveChild(Tree *root, Tree *child) {
    if (getChildPos(root, child) == -1)
        return;
    for (uint i = getChildPos(root, child); i < root->child_count - 1; i++) {
        root->child[i] = root->child[i + 1];
    }
    root->child_count--;
    Tree **aux = root->child;
    root->child = realloc(root->child, sizeof(Tree *) * root->child_count);
    if (root->child == NULL) { // panic, realloc failed, commit seppuku
        root->child_count++;
        root->child = aux;
        exit(EXIT_FAILURE);
    }
}

Tree *newDigitNode(char *c) {
    Tree *num = newTree(c, 1);
    Tree *digit = newTree("digit", 5);
    treeAddChild(digit, num);
    return digit;
}

void freeTreeRec(Tree *root) {
    for (uint i = 0; i < root->child_count; i++) {
        freeTreeRec(root->child[i]);
    }
    free(root);
}

void freeTree(Tree *root) {
    Tree *aux = root;
    while (aux != root->parent) {
        if (aux->child_count != 0) {
            aux = aux->child[aux->child_count - 1];
            continue;
        }
        Tree *parent = aux->parent;
        free(aux);
        if (parent == root->parent)
            break;
        aux = parent;
        aux->child_count--;
    }
    if (aux->parent != NULL)
        treeRemoveChild(aux->parent, aux);
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
