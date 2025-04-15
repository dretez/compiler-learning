#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "Utils/String.h"

String newEmptyString() { return (String){NULL, 0, 0}; }
String newString(char *str, size_t len) {
    String out = newEmptyString();
    stringCpy(&out, (String){str, len, len});
    return out;
}

int stringCpy(String *to, String from) {
    if (to->allocSize < from.len && resizeStringAlloc(to, from.len) == -1)
        return -1;
    memcpy(to->str, from.str, from.len);
    to->len = from.len;
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

void freeString(String *str) {
    free(str->str);
    str->str = NULL;
    str->len = 0;
    str->allocSize = 0;
}

int resizeStringAlloc(String *str, size_t size) {
    char *aux = str->str;
    str->str = realloc(str->str, sizeof(char) * size);
    if (str->str == NULL) {
        str->str = aux;
        return -1;
    }
    str->len = size < str->len ? size : str->len;
    str->allocSize = size;
    return size;
}
