#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "Lexer/Token.h"
#include "Parser/Parsing.h"
#include "Utils/List.h"
#include "Utils/Logs.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        error("Expected 1 argument: ");
        logAppend(argv[0]);
        logAppend(" <file>");
        exit(EXIT_FAILURE);
    }
    int f = open(argv[1], O_RDONLY);
    if (f == -1) {
        error("Couldn't open \"");
        logAppend(argv[1]);
        logAppend("\"");
        exit(EXIT_FAILURE);
    }

    /**************************** LEXICAL ANALYSIS ****************************/

    List *list = List_new();
    if (list == NULL) {
        error("Out of memory exception");
        exit(EXIT_FAILURE);
    }

    for (Token read = TokenReader_nextFromFile(f); read.type != EOF_TOKEN;
         read = TokenReader_nextFromFile(f)) {
        Token *token = Token_new();
        *token = read;
        List_add(list, token);
    }
    // TokenList_print(lex);

    /******************************** PARSING ********************************/

    parse(list);

    printf("============================\n");
    info("Press any key to exit");
    fgetc(stdin);
    exit(EXIT_SUCCESS);
}
