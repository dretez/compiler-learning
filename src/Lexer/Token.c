#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Lexer/Token.h"
#include "Lexer/TokenEndCheckers.h"
#include "Utils/String.h"

TokenType getTokenType(char start);

Token nextToken(int fd) {
    char c;
    for (read(fd, &c, sizeof(char)); isspace((unsigned char)c);
         read(fd, &c, sizeof(char))) {
    }
    off_t start = lseek(fd, 0, SEEK_CUR);
    TokenEndChecker endCheck = setTokenEndChecker(c);
    TokenType type = getTokenType(c);
    while (!endCheck(c))
        read(fd, &c, sizeof(char));
    off_t end = lseek(fd, 0, SEEK_CUR);
    String value = newEmptyString();
    resizeStringAlloc(&value, end - start);
    lseek(fd, start, SEEK_SET);
    read(fd, value.str, sizeof(char) * (end - start));
    return (Token){value, type};
}

TokenType getTokenType(char start) {
    if (isdigit(start)) {
        return DIGIT;
    } else if (ispunct(start)) {
        switch (start) {
        case '+':
            return PLUS;
        case '-':
            return MINUS;
        case '*':
            return TIMES;
        case '/':
            return DIVISION;
        default:
            return INVALID;
        }
    } else
        return INVALID;
}

TokenList newTokenList() { return (TokenList){NULL, 0}; }

int tlistAddToken(TokenList *tl, Token t) {
    Token *aux = tl->list;
    tl->list = realloc(tl->list, sizeof(Token) * (tl->count + 1));
    if (tl->list == NULL) {
        tl->list = aux;
        return -1;
    }
}
