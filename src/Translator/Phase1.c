#include "Translator/Phase1.h"

#include "Utils/FileDescriptor.h"
#include "Utils/String.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void replaceTrigraphs(String *);

String *phase1(int fd) {
    String *str = String_new();
    if (str == NULL) {
        /* Could not create String structure */
        exit(EXIT_FAILURE);
    }
    String_resize(str, fdGetEnd(fd));

    str->len = read(fd, str->str, str->allocSize);

    if (0) {
        // Trigraph replacement is disabled by default. A flag will be
        // added in the future to manually enable trigraph replacement.
        replaceTrigraphs(str);
    }

    return str;
}

void replaceTrigraphs(String *str) {
    char c;
    for (size_t i = 0; i < str->len - 2; i++) {
        if (str->str[i] != '?' || str->str[i + 1] != '?')
            continue;
        switch (str->str[i + 2]) {
        default:
            continue;
        case '<':
            c = '{';
            break;
        case '>':
            c = '}';
            break;
        case '(':
            c = '[';
            break;
        case ')':
            c = ']';
            break;
        case '=':
            c = '#';
            break;
        case '/':
            c = '\\';
            break;
        case '\'':
            c = '^';
            break;
        case '!':
            c = '|';
            break;
        case '-':
            c = '~';
            break;
        }
        String_insertChar(str, c, i);
        String_cutNoPreserve(str, i + 1, 3);
    }
}
