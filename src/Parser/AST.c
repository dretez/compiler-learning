#include <stdio.h>

#include "Lexer/Token.h"
#include "Parser/AST.h"
#include "Utils/Tree.h"

uint AST_getMaxChildren(AST *ast);

Token *AST_data(AST *tree) {
    return Tree_getData(tree);
}

AST *AST_addToken(AST *root, Token *token) {
    AST *tree = Tree_new();
    if (tree == NULL)
        return root;
    Tree_setData(tree, token);
    // if (token->precedence == NONE) {
    //     Tree_setParent(tree, root);
    //     return root;
    // }

    if (token->precedence != NONE &&
        token->precedence >= AST_data(root)->precedence) {
        Tree_setParent(root, tree);
        return tree;
    }
    if (AST_getMaxChildren(root) < Tree_childCount(root)) {
        Tree_setParent(tree, root);
        return root;
    }
    AST *last = Tree_getChild(root, Tree_childCount(root) - 1);
    if (token->precedence <= AST_data(last)->precedence) {
        Tree_setParent(tree, root);
        return root;
    }

    Tree_setParent(last, tree);
    Tree_setParent(tree, root);
    return root;
}

void AST_print(AST *tree) {
    AST_fprint(stdout, tree, 0);
}

void AST_fprint(FILE *stream, AST *tree, uint depth) {
    for (uint i = 0; i < depth; i++)
        fprintf(stream, "|  ");
    Token_fprintln(stream, *AST_data(tree));

    for (uint i = 0; i < Tree_childCount(tree); i++) {
        Tree *ctree = Tree_getChild(tree, i);
        AST_fprint(stream, ctree, depth + 1);
    }
}

void AST_println(AST *tree) {
    AST_fprintln(stdout, tree, 0);
}
void AST_fprintln(FILE *stream, AST *tree, uint depth) {
    AST_fprint(stream, tree, depth);
    fprintf(stream, "\n");
}

uint AST_getMaxChildren(AST *ast) {
    switch (AST_data(ast)->type) {
    case OPERATOR_PLUS:
    case OPERATOR_MINUS:
    case OPERATOR_TIMES:
    case OPERATOR_DIVISION:
        return 2;
    default:
        return 0;
    }
}
