#ifndef INCLUDE_PARSER_AST_H
#define INCLUDE_PARSER_AST_H

#include "Lexer/Token.h"
#include "Utils/Tree.h"

typedef Tree AST;

Token *AST_data(AST *tree);

AST *AST_addToken(AST *root, Token *token);

void AST_print(AST *tree);
void AST_fprint(FILE *stream, AST *tree, uint depth);
void AST_println(AST *tree);
void AST_fprintln(FILE *stream, AST *tree, uint depth);

#endif // ! INCLUDE_PARSER_AST_H
