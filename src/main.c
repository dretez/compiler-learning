#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Utils/Logs.h"
#include "tree.h"

/* Grammar:
 *    list -> list + digit
 *    list -> list - digit
 *    list -> digit
 *    digit -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        error("Expected 1 argument: ");
        logAppend(argv[0]);
        logAppend(" <file>");
        exit(EXIT_FAILURE);
    }
    int f = open(argv[1], O_RDONLY);
    if (f == -1) {
        error("Couldn't open \"");
        logAppend(argv[1]);
        logAppend("\"");
        exit(EXIT_FAILURE);
    }

    char c = '\0';
    Tree *tree = NULL;
    while (read(f, &c, sizeof(char))) {
        if (c >= '0' && c <= '9') {
            Tree *list = newTree("list", 4);
            treeAddChild(list, newDigitNode(&c));
            tree = list;
        } else if (c == '+' || c == '-') {
            Tree *oper = newTree(&c, 1);
            Tree *list = newTree("list", 4);
            treeAddChild(list, tree);
            tree = list;
            if (!read(f, &c, sizeof(char)))
                exit(EXIT_FAILURE);
            treeAddChild(list, newDigitNode(&c));
            treeAddChild(list, oper);
        }
    }
    printTree(tree, 0);

    printf("\n");
    info("Press any key to exit");
    fgetc(stdin);
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
