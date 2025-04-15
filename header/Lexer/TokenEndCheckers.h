#ifndef INCLUDE_LEXER_TOKENENDCHECKERS_H
#define INCLUDE_LEXER_TOKENENDCHECKERS_H

typedef int (*TokenEndChecker)(unsigned char);

TokenEndChecker setTokenEndChecker(char start);

int digitChecker(unsigned char c);
int operatorChecker(unsigned char c);

#endif /* ifndef INCLUDE_LEXER_TOKENENDCHECKERS_H */
