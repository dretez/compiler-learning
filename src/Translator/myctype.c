#include "Translator/myctype.h"

#include <ctype.h>

int isNondigit(char c) {
    if (c == '_')
        return 1;
    if (isalpha(c))
        return 1;
    return 0;
}

int isSign(char c) {
    switch (c) {
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }
}
