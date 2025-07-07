#include <stdio.h>
#include <stdlib.h>

#include "Parser/AST.h"
#include "Lexer/Token.h"
#include "Utils/Tree.h"

void AST_setParent(AST *node, AST *parent);
void AST_updateData(AST *ast);
uint AST_getMaxChildren(AST *ast);

AST AST_init() {
    AST new = {
        .token = NULL,
        .tree = Tree_init(),
    };
    return new;
}

AST *AST_new() {
    AST *ast = malloc(sizeof(AST));
    if (ast == NULL)
        return NULL;
    *ast = AST_init();
    AST_updateData(ast);
    return ast;
}

void AST_free(AST *root) {
    AST_clear(root);
    free(root);
}

void AST_clear(AST *root) {
    Tree_clear(&root->tree);
    root->token = NULL;
}

AST *AST_addToken(AST *root, Token *token) {
    AST *tree = AST_new();
    if (tree == NULL)
        return root;
    tree->token = token;
    // if (token->precedence == NONE) {
    //     AST_setParent(tree, root);
    //     return root;
    // }

    if (token->precedence != NONE && token->precedence >= root->token->precedence) {
        AST_setParent(root, tree);
        return tree;
    }
    if (AST_getMaxChildren(root) < root->tree.children.count) {
        AST_setParent(tree, root);
        return root;
    }
    AST *rootsLastChild = (AST *)(Tree_getChild(root->tree, root->tree.children.count - 1)->data);
    if (token->precedence <= rootsLastChild->token->precedence) {
        AST_setParent(tree, root);
        return root;
    }
    
    AST_setParent(rootsLastChild, tree);
    AST_setParent(tree, root);
    return root;
}

void AST_print(AST tree) {
    AST_fprint(stdout, tree, 0);
}

void AST_fprint(FILE *stream, AST tree, uint depth) {
    for(uint i = 0; i < depth; i++)
        fprintf(stream, "  ");
    Token_fprintln(stream, *tree.token);
    fprintf(stream, "\n");

    for (uint i = 0; i < Tree_childCount(tree.tree); i++) {
        AST_fprint(stream, *((AST *)(Tree_getChild(tree.tree, i)->data)), depth + 1);
    }
}

void AST_println(AST tree) {
    AST_fprintln(stdout, tree, 0);
}
void AST_fprintln(FILE *stream, AST tree, uint depth) {
    AST_fprint(stream, tree, depth);
    fprintf(stream, "\n");
}

void AST_setParent(AST *node, AST *parent) {
    Tree_removeChild(node->tree.parent, &node->tree);
    Tree_addChild(&parent->tree, &node->tree);
}

void AST_updateData(AST *ast) {
    ast->tree.data = ast;
}

uint AST_getMaxChildren(AST *ast) {
    switch (ast->token->type) {
    case PLUS:
    case MINUS:
    case TIMES:
    case DIVISION:
        return 2;
    default:
        return 0;
    }
}
