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

TokenPrecedence Token_getPrecedence(TokenType type) {
    switch (type) {
    case OP_SUFFIX_INC:
    case OP_SUFFIX_DEC:
    case OP_FUNC_CALL:
    case OP_ARRAY_SUBSCRIPTING:
    case OP_MEMBER_ACCESS:
    case OP_PTR_MEMBER_ACCESS:
    case OP_COMPOUND_LITERAL:
        return ONE;
    case OP_PREFIX_INC:
    case OP_PREFIX_DEC:
    case OP_UNARY_PLUS:
    case OP_UNARY_MINUS:
    case OP_LOGICAL_NOT:
    case OP_BITWISE_MOT:
    case OP_TYPE_CAST:
    case OP_DEREFERENCE:
    case OP_ADDRESS_OF:
    case OP_SIZEOF:
    case OP_ALIGNOF:
        return TWO;
    case OP_MULTIPLICATION:
    case OP_DIVISION:
    case OP_REMAINDER:
        return THREE;
    case OP_ADDITION:
    case OP_SUBTRACTION:
        return FOUR;
    case OP_BITWISE_LSHIFT:
    case OP_BITWISE_RSHIFT:
        return FIVE;
    case OP_LOGICAL_LESS:
    case OP_LOGICAL_LESS_EQUALS:
    case OP_LOGICAL_GREATER:
    case OP_LOGICAL_GREATER_EQUALS:
        return SIX;
    case OP_LOGICAL_EQUALS:
    case OP_LOGICAL_NOT_EQUALS:
        return SEVEN;
    case OP_BITWISE_AND:
        return EIGHT;
    case OP_BITWISE_XOR:
        return NINE;
    case OP_BITWISE_OR:
        return TEN;
    case OP_LOGICAL_AND:
        return ELEVEN;
    case OP_LOGICAL_OR:
        return TWELVE;
    case OP_TERNARY_CONDITIONAL:
        return THIRTEEN;
    case OP_ASSIGNMENT:
    case OP_ASSIGNMENT_SUM:
    case OP_ASSIGNMENT_DIFF:
    case OP_ASSIGNMENT_PRODUCT:
    case OP_ASSIGNMENT_QUOTIENT:
    case OP_ASSIGNMENT_REMAINDER:
    case OP_ASSIGNMENT_BIT_LS:
    case OP_ASSIGNMENT_BIT_RS:
    case OP_ASSIGNMENT_BIT_AND:
    case OP_ASSIGNMENT_BIT_XOR:
    case OP_ASSIGNMENT_BIT_OR:
        return FOURTEEN;
    /* Precedence 15, left-to-right */
    case OP_COMMA:
        return FIFTEEN;
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
    case OP_SUFFIX_INC:
    case OP_SUFFIX_DEC:
    case OP_FUNC_CALL:
    case OP_ARRAY_SUBSCRIPTING:
    case OP_MEMBER_ACCESS:
    case OP_PTR_MEMBER_ACCESS:
    case OP_COMPOUND_LITERAL:
    case OP_PREFIX_INC:
    case OP_PREFIX_DEC:
    case OP_UNARY_PLUS:
    case OP_UNARY_MINUS:
    case OP_LOGICAL_NOT:
    case OP_BITWISE_MOT:
    case OP_TYPE_CAST:
    case OP_DEREFERENCE:
    case OP_ADDRESS_OF:
    case OP_SIZEOF:
    case OP_ALIGNOF:
    case OP_MULTIPLICATION:
    case OP_DIVISION:
    case OP_REMAINDER:
    case OP_ADDITION:
    case OP_SUBTRACTION:
    case OP_BITWISE_LSHIFT:
    case OP_BITWISE_RSHIFT:
    case OP_LOGICAL_LESS:
    case OP_LOGICAL_LESS_EQUALS:
    case OP_LOGICAL_GREATER:
    case OP_LOGICAL_GREATER_EQUALS:
    case OP_LOGICAL_EQUALS:
    case OP_LOGICAL_NOT_EQUALS:
    case OP_BITWISE_AND:
    case OP_BITWISE_XOR:
    case OP_BITWISE_OR:
    case OP_LOGICAL_AND:
    case OP_LOGICAL_OR:
    case OP_TERNARY_CONDITIONAL:
    case OP_ASSIGNMENT:
    case OP_ASSIGNMENT_SUM:
    case OP_ASSIGNMENT_DIFF:
    case OP_ASSIGNMENT_PRODUCT:
    case OP_ASSIGNMENT_QUOTIENT:
    case OP_ASSIGNMENT_REMAINDER:
    case OP_ASSIGNMENT_BIT_LS:
    case OP_ASSIGNMENT_BIT_RS:
    case OP_ASSIGNMENT_BIT_AND:
    case OP_ASSIGNMENT_BIT_XOR:
    case OP_ASSIGNMENT_BIT_OR:
    case OP_COMMA:
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
            return OP_ADDITION;
        case '-':
            return OP_SUBTRACTION;
        case '*':
            return OP_MULTIPLICATION;
        case '/':
            return OP_DIVISION;
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
