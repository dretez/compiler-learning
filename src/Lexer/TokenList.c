#include <stdio.h>
#include <stdlib.h>

#include "Lexer/TokenList.h"
#include "Lexer/Token.h"

TokenList TokenList_init() { 
    return (TokenList){
        .list = NULL,
        .count = 0,
    };
}

TokenList *TokenList_new() {
    TokenList *list = malloc(sizeof(TokenList));
    if (list == NULL)
        return NULL;
    *list = TokenList_init();
    return list;
}

void TokenList_clear(TokenList *list) {
    TokenList_forEach(list, Token_clear);
    free(list->list);
    *list = TokenList_init();
}

void TokenList_free(TokenList **list) {
    if (*list == NULL) 
        return;
    TokenList_clear(*list);
    free(*list);
    *list = NULL;
}

int TokenList_add(TokenList *tl, Token t) {
    if (tl == NULL)
        return -1;
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

Token *TokenList_get(TokenList *list, uint idx) {
    if (list == NULL || list->count < idx)
        return NULL;
    return &list->list[idx];
}

void TokenList_forEach(TokenList *list, void (*function)(Token *)) {
    if (list == NULL)
        return;
    for (uint i = 0; i < list->count; i++) {
        function(&list->list[i]);
    }
}

void TokenList_print(TokenList lex) {
    TokenList_fprint(stdout, lex);
}

void TokenList_fprint(FILE *stream, TokenList lex) {
    fprintf(stream, "Token list length: %d\n", lex.count);
    for (uint i = 0; i < lex.count; i++) {
        fprintf(stream, "Token %d:\n", i);
        Token_fprint(stream, lex.list[i]);
    }
}
