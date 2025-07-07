#ifndef INCLUDE_PARSER_AST_H
#define INCLUDE_PARSER_AST_H

#include "Lexer/Token.h"
#include "Utils/Tree.h"

// Abstract Syntax Tree
typedef struct {
    Token *token;
    Tree tree;
} AST;

AST AST_init();
AST *AST_new();
void AST_free(AST *root);
void AST_clear(AST *root);

AST *AST_addToken(AST *root, Token *token);

void AST_print(AST tree);
void AST_fprint(FILE *stream, AST tree, uint depth);
void AST_println(AST tree);
void AST_fprintln(FILE *stream, AST tree, uint depth);

#endif // ! INCLUDE_PARSER_AST_H
