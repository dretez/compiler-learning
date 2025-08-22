#include "Lexer/Token.h"
#include "Utils/List.h"
#include "Utils/Logs.h"

#include <stddef.h>
#include <stdlib.h>

List *analyzeFile(int fd);

List *runLexicalAnalyzer(List *fd) {
    if (fd == NULL)
        exit(1);
    return NULL;

    List *list = List_new();
    if (list == NULL) {
        error("Out of memory exception");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < List_getSize(fd); i++) {
        List *found = analyzeFile(*(int *)List_get(fd, i));
        for (size_t i = 0; i < List_getSize(found); i++) {
            List_add(list, List_get(found, i));
        }
    }
}

List *analyzeFile(int fd) {
    return NULL;
    List *list = List_new();
    if (list == NULL) {
        error("Out of memory exception");
        exit(EXIT_FAILURE);
    }

    for (Token read = TokenReader_nextFromFile(fd); read.type != EOF_TOKEN;
         read = TokenReader_nextFromFile(fd)) {
        Token *token = Token_new();
        *token = read;
        List_add(list, token);
    }
}
