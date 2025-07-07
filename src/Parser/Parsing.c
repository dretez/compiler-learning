#include <stdio.h>
#include <stdlib.h>

#include "Lexer/Token.h"
#include "Lexer/TokenList.h"
#include "Parser/AST.h"
#include "Utils/Logs.h"
#include "Utils/String.h"

void parse(TokenList lex) {
    AST *parse = AST_new();
    Token empty = Token_init();

    if (parse == NULL) {
        return;
    }

    parse->token = TokenList_get(&lex, 0);

    for (uint i = 1; i < lex.count; i++) {
        // Could consider switching to a doubly linked list
        Token *next = i == lex.count - 1 ? &empty : &lex.list[i + 1];
        //Token *prev = i == 0 ? &empty : &lex.list[i - 1];
        Token *token = &lex.list[i];

        //if (token->type == next->type) {
        if (0) {
            error("Unexpected tokens found:");
            printf("\tToken %d: ", i);
            String_print(token->str);
            printf("\n\tToken %d: ", i + 1);
            String_print(next->str);
            printf("\n");
            exit(EXIT_FAILURE);
        }

        parse = AST_addToken(parse, TokenList_get(&lex, i));
    }

    AST_println(*parse);
}
