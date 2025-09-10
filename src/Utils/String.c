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

void String_clearUnusedMem(String *str) {
    char *aux = str->str;
    str->str = realloc(str->str, str->len);
    if (str->str == NULL) {
        str->str = aux;
        return;
    }
    str->allocSize = str->len;
}

/******************************** STRING DATA ********************************/

size_t String_len(String *str) {
    return str->len;
}

size_t String_allocSize(String *str) {
    return str->allocSize;
}

char String_getChar(String *str, size_t pos) {
    return pos >= str->len ? 0 : str->str[pos];
}

String String_getSlice(String *str, size_t start, size_t len) {
    start = start >= str->len ? str->len - 1 : start;
    len = start + len > str->len ? str->len - start : len;
    return (String){
        .str = &str->str[start],
        .len = len,
        .allocSize = 0,
    };
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

int String_cmpLiteral(String s1, char *s2) {
    size_t size = strlen(s2);
    String new = {
        .str = s2,
        .len = size,
        .allocSize = 0,
    };
    return String_cmp(s1, new);
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

void String_append(String *str, char *data, size_t dataLen) {
    if (str->allocSize - str->len < dataLen) {
        char *aux = str->str;
        str->str = realloc(str->str, str->len + dataLen);
        if (str->str == NULL) {
            str->str = aux;
            return;
        }
        str->allocSize = str->len + dataLen;
    }
    memcpy(&str->str[str->len], data, dataLen);
    str->len += dataLen;
}

String *String_cut(String *str, size_t start, size_t len) {
    start = start >= str->len ? str->len - 1 : start;
    len = start + len > str->len ? str->len - start : len;
    /* Create a String object to contain the cut section */
    String *new = String_new();
    if (new != NULL)
        String_append(new, &str->str[start], len);
    memcpy(&str->str[start], &str->str[start + len], str->len - (start + len));
    str->len -= len;
    return new;
}

void String_cutNoPreserve(String *str, size_t start, size_t len) {
    start = start >= str->len ? str->len - 1 : start;
    len = start + len > str->len ? str->len - start : len;
    memcpy(&str->str[start], &str->str[start + len], str->len - (start + len));
    str->len -= len;
}

void String_insertChar(String *str, char c, size_t pos) {
    pos = pos > str->len ? str->len : pos;
    if (str->len + 1 > str->allocSize) {
        char *aux = str->str;
        str->str = realloc(str->str, str->len + 1);
        if (str->str == NULL) {
            str->str = aux;
            return;
        }
        str->allocSize++;
    }
    memcpy(&str->str[pos + 1], &str->str[pos], str->len - pos);
    str->str[pos] = c;
    str->len++;
}

void String_insert(String *str, String *data, size_t pos) {
    pos = pos > str->len ? str->len : pos;
    if (str->len + 1 > str->allocSize) {
        char *aux = str->str;
        str->str = realloc(str->str, str->len + data->len);
        if (str->str == NULL) {
            str->str = aux;
            return;
        }
        str->allocSize = str->len + data->len;
    }
    memcpy(&str->str[pos + data->len], &str->str[pos], str->len - pos);
    memcpy(&str->str[pos], &data->str, data->len);
    str->len += data->len;
}
