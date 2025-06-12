#ifndef INCLUDE_LEXER_TOKENLIST_H
#define INCLUDE_LEXER_TOKENLIST_H

#include <sys/types.h>

#include "Lexer/Token.h"

typedef struct {
    Token *list;
    uint count;
} TokenList;

TokenList TokenList_init();
TokenList *TokenList_new();

void TokenList_clear(TokenList *);
void TokenList_free(TokenList **);

int TokenList_add(TokenList *, Token);

Token *TokenList_get(TokenList *list, uint idx);

void TokenList_forEach(TokenList *list, void (*function)(Token *));

void TokenList_print(TokenList);
void TokenList_fprint(FILE *stream, TokenList);

#endif /* ifndef INCLUDE_LEXER_TOKENLIST_H */
