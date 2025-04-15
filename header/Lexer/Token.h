#ifndef INCLUDE_LEXER_TOKEN_H
#define INCLUDE_LEXER_TOKEN_H

#include <sys/types.h>

#include "Utils/String.h"

typedef enum {
    INVALID,
    DIGIT,
    PLUS,
    MINUS,
    TIMES,
    DIVISION,
} TokenType;

typedef struct {
    String a;
    TokenType type;
} Token;

typedef struct {
    Token *list;
    uint count;
} TokenList;

Token nextToken(int fd);
TokenList newTokenList();
int tlistAddToken(TokenList *, Token);

#endif /* ifndef INCLUDE_LEXER_TOKEN_H */
