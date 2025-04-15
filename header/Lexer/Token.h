#ifndef INCLUDE_LEXER_TOKEN_H
#define INCLUDE_LEXER_TOKEN_H

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

Token nextToken(int fd);

#endif /* ifndef INCLUDE_LEXER_TOKEN_H */
