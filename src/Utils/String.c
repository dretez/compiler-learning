#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils/String.h"

/******************************** CONSTRUCTION ********************************/

String String_init() { 
    return (String){
        .str = NULL,
        .len = 0,
        .allocSize = 0,
    }; 
}

String *String_new() {
    String *str = malloc(sizeof(String));
    if (str == NULL)
        return NULL;
    *str = String_init();
    return str;
}

String String_fromCString(char *str, size_t len) {
    String out = String_init();
    String_cpy(&out, (String){
        .str = str, 
        .len = len,
        .allocSize = len,
    });
    return out;
}

/********************************** CLEAN UP **********************************/

void String_free(String **str) {
    if (*str == NULL)
        return;
    String_clear(*str);
    free(*str);
    *str = NULL;
}

void String_clear(String *str) {
    free(str->str);
    *str = String_init();
}

/***************************** STRING OPERATIONS *****************************/

int String_cpy(String *to, String from) {
    if (to->allocSize < from.len && String_resize(to, from.len) == -1)
        return -1;
    memcpy(to->str, from.str, from.len);
    to->len = from.len;
    return from.len;
}

int String_cmp(String s1, String s2) {
    int out = memcmp(s1.str, s2.str,
                     sizeof(char) * (s1.len > s2.len ? s2.len : s1.len));
    if (out == 0 && s1.len != s2.len) {
        out = s1.len > s2.len ? 1 : -1;
    }
    return out;
}

int String_resize(String *str, size_t size) {
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

void String_print(String str) {
    for (size_t i = 0; i < str.len; i++) {
        putchar(str.str[i]);
    }
}
