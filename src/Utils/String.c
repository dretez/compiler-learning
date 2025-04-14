#include "Utils/String.h"
#include <stdlib.h>
#include <string.h>

String newEmptyString() { return (String){NULL, 0}; }
String newString(char *str, size_t len) {
    String out = newEmptyString();
    stringCpy(out, (String){str, len});
    return out;
}

int stringCpy(String to, String from) {
    if (from.len != to.len) {
        char *aux = to.str;
        to.str = realloc(to.str, sizeof(char) * from.len);
        if (to.str == NULL) {
            to.str = aux;
            return -1;
        }
        to.len = from.len;
    }
    memcpy(to.str, from.str, from.len);
    return from.len;
}

int stringCmp(String s1, String s2) {
    int out = memcmp(s1.str, s2.str,
                     sizeof(char) * (s1.len > s2.len ? s2.len : s1.len));
    if (out == 0 && s1.len != s2.len) {
        out = s1.len > s2.len ? 1 : -1;
    }
    return out;
}
