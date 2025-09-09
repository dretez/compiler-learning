#ifndef INCLUDE_LEXER_TOKEN_H
#define INCLUDE_LEXER_TOKEN_H

#include "Utils/List.h"
#include "Utils/String.h"

typedef struct _IO_FILE FILE;

typedef enum {
    INVALID,
    INTEGER_LITERAL,

    /* Operators */
    /* Precedence 1, left-to-right */
    OP_SUFFIX_INC,
    OP_SUFFIX_DEC,
    OP_FUNC_CALL,
    OP_ARRAY_SUBSCRIPTING,
    OP_MEMBER_ACCESS,
    OP_PTR_MEMBER_ACCESS,
    OP_COMPOUND_LITERAL,
    /* Precedence 2, right-to-left */
    OP_PREFIX_INC,
    OP_PREFIX_DEC,
    OP_UNARY_PLUS,
    OP_UNARY_MINUS,
    OP_LOGICAL_NOT,
    OP_BITWISE_MOT,
    OP_TYPE_CAST,
    OP_DEREFERENCE,
    OP_ADDRESS_OF,
    OP_SIZEOF,
    OP_ALIGNOF,
    /* Precedence 3, left-to-right */
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_REMAINDER,
    /* Precedence 4, left-to-right */
    OP_ADDITION,
    OP_SUBTRACTION,
    /* Precedence 5, left-to-right */
    OP_BITWISE_LSHIFT,
    OP_BITWISE_RSHIFT,
    /* Precedence 6, left-to-right */
    OP_LOGICAL_LESS,
    OP_LOGICAL_LESS_EQUALS,
    OP_LOGICAL_GREATER,
    OP_LOGICAL_GREATER_EQUALS,
    /* Precedence 7, left-to-right */
    OP_LOGICAL_EQUALS,
    OP_LOGICAL_NOT_EQUALS,
    /* Precedence 8, left-to-right */
    OP_BITWISE_AND,
    /* Precedence 9, left-to-right */
    OP_BITWISE_XOR,
    /* Precedence 10, left-to-right */
    OP_BITWISE_OR,
    /* Precedence 11, left-to-right */
    OP_LOGICAL_AND,
    /* Precedence 12, left-to-right */
    OP_LOGICAL_OR,
    /* Precedence 13, right-to-left */
    OP_TERNARY_CONDITIONAL,
    /* Precedence 14, right-to-left */
    OP_ASSIGNMENT,
    OP_ASSIGNMENT_SUM,
    OP_ASSIGNMENT_DIFF,
    OP_ASSIGNMENT_PRODUCT,
    OP_ASSIGNMENT_QUOTIENT,
    OP_ASSIGNMENT_REMAINDER,
    OP_ASSIGNMENT_BIT_LS,
    OP_ASSIGNMENT_BIT_RS,
    OP_ASSIGNMENT_BIT_AND,
    OP_ASSIGNMENT_BIT_XOR,
    OP_ASSIGNMENT_BIT_OR,
    /* Precedence 15, left-to-right */
    OP_COMMA,

    /* Brackets */
    BRACKET_ROUND_L,
    BRACKET_ROUND_R,
    BRACKET_SQUARE_L,
    BRACKET_SQUARE_R,
    BRACKET_CURLY_L,
    BRACKET_CURLY_R,

    EOF_TOKEN,
    NULL_TOKEN,
} TokenType;

typedef enum {
    ONE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    ELEVEN,
    TWELVE,
    THIRTEEN,
    FOURTEEN,
    FIFTEEN,
    NONE,
} TokenPrecedence;

typedef struct {
    String str;
    TokenType type;
    TokenPrecedence precedence;
} Token;

Token Token_init();
Token *Token_new();

void Token_clear(Token *);
void Token_free(Token **);

Token TokenReader_nextFromFile(int fd);

TokenPrecedence Token_getPrecedence(TokenType);

int Token_isNum(const Token *);
int Token_isBinOp(const Token *);
int Token_isLR(const Token *);
int Token_isRL(const Token *);
int Token_getAssociativity(const Token *);

/* Visualization */

void Token_print(Token token);
void Token_fprint(FILE *stream, Token token);
void Token_println(Token token);
void Token_fprintln(FILE *stream, Token token);

void TokenList_print(List *list);
void TokenList_fprint(FILE *stream, List *list);

#endif /* ifndef INCLUDE_LEXER_TOKEN_H */
