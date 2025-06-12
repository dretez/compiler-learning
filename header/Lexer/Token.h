#ifndef INCLUDE_LEXER_TOKEN_H
#define INCLUDE_LEXER_TOKEN_H

#include <sys/types.h>

#include "Utils/String.h"

typedef struct _IO_FILE FILE;

typedef enum {
    INVALID,
    DIGIT,
    PLUS,
    MINUS,
    TIMES,
    DIVISION,
    EOF_TOKEN,
    NULL_TOKEN,
} TokenType;

typedef enum {
    NONE,
    MUL_DIV,
    ADD_SUB,
} TokenPrecendence;

typedef struct {
    String str;
    TokenType type;
    TokenPrecendence precedence;
} Token;

Token Token_init();
Token *Token_new();

void Token_clear(Token *);
void Token_free(Token **);

Token TokenReader_nextFromFile(int fd);

TokenPrecendence Token_getPrecedence(TokenType);

/* Visualization */

void Token_print(Token token);
void Token_fprint(FILE *stream, Token token);
void Token_println(Token token);
void Token_fprintln(FILE *stream, Token token);

#endif /* ifndef INCLUDE_LEXER_TOKEN_H */
