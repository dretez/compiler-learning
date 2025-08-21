#include <stdio.h>
#include <stdlib.h>

#include "Lexer/Token.h"
#include "Utils/List.h"
#include "Utils/Logs.h"
#include "Utils/String.h"
#include "Utils/Tree.h"

typedef struct parser {
    List *tokens;
    size_t index;
} ParserData;

void AST_fprint(FILE *stream, Tree *tree, uint depth);
void AST_println(Tree *tree);
void AST_fprintln(FILE *stream, Tree *tree, uint depth);

Tree *handleNumber(ParserData *parser);
Tree *handleBinOperator(ParserData *parser, Tree *left);

void parse(List *tokens) {
    ParserData data = {
        .tokens = tokens,
        .index = 0,
    };
    Tree *parse = NULL;

    while (data.index < List_getSize(tokens)) {
        Token *token = List_get(tokens, data.index);
        switch (token->type) {
        case INTEGER_LITERAL:
            parse = handleNumber(&data);
            break;
        case OPERATOR_PLUS:
        case OPERATOR_MINUS:
        case OPERATOR_TIMES:
        case OPERATOR_DIVISION:
            parse = handleBinOperator(&data, parse);
            break;
        default:
            exit(1);
        }
    }
    AST_println(parse);
}

Tree *handleNumber(ParserData *parser) {
    Token *cur = List_get(parser->tokens, parser->index);
    if (!Token_isNum(cur)) // Unexpected token received
        exit(1);

    Tree *tree = Tree_new();
    if (tree == NULL)
        return NULL;
    Tree_setData(tree, cur);
    parser->index++;
    return tree;
}

Tree *handleBinOperator(ParserData *parser, Tree *left) {
    if (left == NULL)
        exit(1);
    Token *cur = List_get(parser->tokens, parser->index);
    Token *prev = List_get(parser->tokens, parser->index - 1);
    if (!Token_isBinOp(cur) || !Token_isNum(prev)) // Unexpected tokens received
        exit(1);
    parser->index++;
    Token *next = List_get(parser->tokens, parser->index);
    if (!Token_isNum(next)) // unexpected/missing token type for operand
        exit(1);

    Tree *tree = Tree_new();
    Tree_setData(tree, cur);
    Tree_addChild(tree, left);

    next = List_get(parser->tokens, parser->index + 1);
    if (Token_isBinOp(next))
        if (Token_getPrecedence(next->type) < Token_getPrecedence(cur->type))
            Tree_addChild(tree, handleBinOperator(parser, handleNumber(parser)));
        else {
            Tree_addChild(tree, handleNumber(parser));
            return handleBinOperator(parser, tree);
        }
    else
        Tree_addChild(tree, handleNumber(parser));
    return tree;
}

void AST_fprint(FILE *stream, Tree *tree, uint depth) {
    for (uint i = 0; i < depth; i++)
        fprintf(stream, "|  ");
    Token_fprintln(stream, *(Token *)Tree_getData(tree));

    for (uint i = 0; i < Tree_childCount(tree); i++) {
        Tree *ctree = Tree_getChild(tree, i);
        AST_fprint(stream, ctree, depth + 1);
    }
}

void AST_println(Tree *tree) {
    AST_fprintln(stdout, tree, 0);
}

void AST_fprintln(FILE *stream, Tree *tree, uint depth) {
    AST_fprint(stream, tree, depth);
    fprintf(stream, "\n");
}