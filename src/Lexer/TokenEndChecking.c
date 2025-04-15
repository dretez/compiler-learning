#include <ctype.h>
#include <stddef.h>

#include "Lexer/TokenEndCheckers.h"

TokenEndChecker setTokenEndChecker(char start) {
    if (isdigit(start)) {
        return digitChecker;
    } else if (ispunct(start)) {
        return operatorChecker;
    }
    return NULL;
}

int digitChecker(unsigned char c) {
    return isspace(c) || isalpha(c) || ispunct(c);
}
int operatorChecker(unsigned char c) { return isspace(c) || isalnum(c); }
