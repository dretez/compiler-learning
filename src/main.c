#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Grammar:
 *    list -> list + digit
 *    list -> list - digit
 *    list -> digit
 *    digit -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 */

typedef uint8_t Digit;

typedef struct list {
    struct list *l;
    char operator;
    Digit digit;
} List;

typedef struct {
    char *label;
    uint labelsize;
} TreeNode;

/* A tree is any structure with a root node and at least 1 child node.
 * By this definition, a list is a tree with 1 or 3 child nodes where the root
 * node's label is 'list'.*/
typedef struct tree {
    char *label;
    uint label_len;
    struct tree **child;
    uint child_count;
} Tree;

/*             R
 *            /|\
 *           / | \
 *           1 2 3
 */

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
    root->child = realloc(root, sizeof(Tree *) * (root->child_count + 1));
    if (root->child == NULL) {
        root->child = aux;
        return;
    }
    root->child[root->child_count] = child;
    root->child_count++;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Expected 1 argument: %s <file>", argv[0]);
        exit(EXIT_FAILURE);
    }
    int f = open(argv[1], O_RDONLY);
    char c = 0;

    Tree *tree = NULL;
    while (read(f, &c, sizeof(char))) {
        if (c >= '0' && c <= '9') {
            Tree *num = newTree(&c, 1);
            Tree *digit = newTree("digit", 5);
            treeAddChild(digit, num);
            Tree *list = newTree("list", 4);
            treeAddChild(list, digit);

        } else if (c == '+' || c == '-') {
            Tree *oper = newTree(&c, 1);
            Tree *list = newTree("list", 4);
        }
    }

    exit(EXIT_SUCCESS);
}

/* 9-5+2
 * reads '9'
 *      new tree with label 9, label len 1 and 0 children
 *      new tree with label digit, label len 5 and 1 child, the tree created
 *       before
 *      new tree with label list, label len 4 and 1 child, the tree created
 *       before
 *      sets the root to the list tree
 * reads '-'
 *      new tree with label -, label len 1 and 0 children
 *      new tree with label list, label len 4, 2 children, last list tree
 *      read next char
 *      reads 5
 *          new tree with label 5
 *          new tree with label digit
 *          add tree to list tree
 *      set root to new list tree
 */
