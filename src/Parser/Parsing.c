#include <stdio.h>
#include <stdlib.h>

#include "Lexer/Token.h"
#include "Parser/AST.h"
#include "Utils/List.h"
#include "Utils/Logs.h"
#include "Utils/String.h"
#include "Utils/Tree.h"

void parse(List *lex) {
    AST *parse = Tree_new();
    Token *empty = Token_new();

    if (parse == NULL) {
        return;
    }

    Tree_setData(parse, List_get(lex, 0));

    for (uint i = 1; i < List_getSize(lex); i++) {
        Token *next;
        if (i == List_getSize(lex) - 1)
            next = empty;
        else
            next = List_get(lex, i + 1);
        Token *token = List_get(lex, i);

        // if (token->type == next->type) {
        if (0) {
            error("Unexpected tokens found:");
            printf("\tToken %d: ", i);
            String_print(token->str);
            printf("\n\tToken %d: ", i + 1);
            String_print(next->str);
            printf("\n");
            exit(EXIT_FAILURE);
        }

        parse = AST_addToken(parse, List_get(lex, i));
    }

    AST_println(parse);
}
