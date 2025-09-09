#include "Translator/Phase2.h"

#include "Utils/String.h"

#include <stddef.h>

void phase2(String *file) {
    for (size_t i = 1; i < file->len; i++) {
        if (file->str[i] != '\n' || file->str[i - 1] != '\\')
            continue;
        String_cutNoPreserve(file, i - 1, 2);
    }
    if (file->str[file->len - 1] != '\n') {
        /* WARN: Undefined behaviour, should warn user */
    }
}
