#include <ctype.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Lexer/Token.h"
#include "Lexer/TokenEndCheckers.h"
#include "Utils/FileDescriptor.h"
#include "Utils/String.h"

TokenType getTokenType(char start);

Token nextToken(int fd) {
    char c;
    read(fd, &c, sizeof(char));
    for (off_t fdEnd = fdGetEnd(fd);
         fdGetCur(fd) < fdEnd && isspace((unsigned char)c);
         read(fd, &c, sizeof(char))) {
    }

    if (fdGetCur(fd) >= fdGetEnd(fd))
        return (Token){newEmptyString(), EOF_TOKEN};

    off_t start = lseek(fd, 0, SEEK_CUR) - 1;
    TokenEndChecker endCheck = setTokenEndChecker(c);
    TokenType type = getTokenType(c);
    while (!endCheck(c))
        read(fd, &c, sizeof(char));
    off_t end = lseek(fd, 0, SEEK_CUR) - 1;

    lseek(fd, start, SEEK_SET);
    String value = newEmptyString();
    resizeStringAlloc(&value, end - start);
    read(fd, value.str, sizeof(char) * (end - start));
    value.len = end - start;

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
    tl->list[tl->count] = t;
    tl->count++;
    return tl->count;
}

void fprintTokenList(FILE *stream, TokenList lex) {
    fprintf(stream, "Token list length: %d\n", lex.count);
    for (uint i = 0; i < lex.count; i++) {
        fprintf(stream, "Token %d:\n\ttype: %d\n\tString:\n\t\t%p: ", i,
                lex.list[i].type, lex.list[i].str.str);
        printString(lex.list[i].str);
        fprintf(stream, "\n\t\tlen: %zu\n\t\talloced: %zu\n",
                lex.list[i].str.len, lex.list[i].str.allocSize);
    }
}
void printTokenList(TokenList lex) { fprintTokenList(stdout, lex); }
