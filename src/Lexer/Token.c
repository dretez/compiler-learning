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

Token Token_init() {
    return (Token){
        .precedence = Token_getPrecedence(NULL_TOKEN),
        .type = NULL_TOKEN,
        .str = String_init(),
    };
}

Token *Token_new() {
    Token *token = malloc(sizeof(Token));
    if (token == NULL)
        return NULL;
    *token = Token_init();
    return token;
}

void Token_clear(Token *token) {
    if (token == NULL)
        return;
    String_clear(&token->str);
    *token = Token_init();
}

void Token_free(Token **token) {
    if (*token == NULL)
        return;
    String_clear(&(*token)->str);
    free(*token);
    *token = NULL;
}

Token TokenReader_nextFromFile(int fd) {
    char c;
    read(fd, &c, sizeof(char));
    for (off_t fdEnd = fdGetEnd(fd);
         fdGetCur(fd) < fdEnd && isspace((unsigned char)c);
         read(fd, &c, sizeof(char))) {
    }

    if (fdGetCur(fd) >= fdGetEnd(fd))
        return (Token){
            .precedence = Token_getPrecedence(EOF_TOKEN),
            .type = EOF_TOKEN,
            .str = String_init(),
        };

    off_t start = lseek(fd, 0, SEEK_CUR) - 1;
    TokenEndChecker endCheck = setTokenEndChecker(c);
    TokenType type = getTokenType(c);
    while (!endCheck(c))
        read(fd, &c, sizeof(char));
    off_t end = lseek(fd, 0, SEEK_CUR) - 1;

    lseek(fd, start, SEEK_SET);
    String value = String_init();
    String_resize(&value, end - start);
    read(fd, value.str, sizeof(char) * (end - start));
    value.len = end - start;

    return (Token){
        .precedence = Token_getPrecedence(type),
        .type = type,
        .str = value,
    };
}

TokenPrecendence Token_getPrecedence(TokenType type) {
    switch (type) {
    case OPERATOR_PLUS:
    case OPERATOR_MINUS:
        return ADD_SUB;
    case OPERATOR_TIMES:
    case OPERATOR_DIVISION:
        return MUL_DIV;
    case INVALID:
    case INTEGER_LITERAL:
    case EOF_TOKEN:
    case NULL_TOKEN:
        return NONE;
    default:
        return -1;
    }
}

int Token_isNum(const Token *token) {
    if (token == NULL)
        return 0;
    switch (token->type) {
    case INTEGER_LITERAL:
        return 1;
    default:
        return 0;
    }
}

int Token_isBinOp(const Token *token) {
    if (token == NULL)
        return 0;
    switch (token->type) {
    case OPERATOR_PLUS:
    case OPERATOR_MINUS:
    case OPERATOR_TIMES:
    case OPERATOR_DIVISION:
        return 1;
    default:
        return 0;
    }
}

void Token_fprint(FILE *stream, Token token) {
    fprintf(stream, "Type: %d, Precedence: %d, String: \"", token.type,
            token.precedence);
    String_print(token.str);
    fprintf(stream, "\"");
}

void Token_fprintln(FILE *stream, Token token) {
    Token_fprint(stream, token);
    fprintf(stream, "\n");
}

void Token_print(Token token) {
    Token_fprint(stdout, token);
}

void Token_println(Token token) {
    Token_fprintln(stdout, token);
}

TokenType getTokenType(char start) {
    if (isdigit(start)) {
        return INTEGER_LITERAL;
    } else if (ispunct(start)) {
        switch (start) {
        case '+':
            return OPERATOR_PLUS;
        case '-':
            return OPERATOR_MINUS;
        case '*':
            return OPERATOR_TIMES;
        case '/':
            return OPERATOR_DIVISION;
        default:
            return INVALID;
        }
    } else
        return INVALID;
}

void TokenList_print(List *list) {
    TokenList_fprint(stdout, list);
}

void TokenList_fprint(FILE *stream, List *list) {
    fprintf(stream, "Token list length: %zu\n", List_getSize(list));
    for (uint i = 0; i < List_getSize(list); i++) {
        fprintf(stream, "Token %d:\n", i);
        Token_fprint(stream, *(Token *)List_get(list, i));
    }
}
