#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "Lexer/Token.h"
#include "Utils/Logs.h"
#include "Utils/String.h"

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

    TokenList lex = newTokenList();
    for (Token token = nextToken(f); token.type != EOF_TOKEN;
         token = nextToken(f)) {
        tlistAddToken(&lex, token);
    }
    for (uint i = 0; i < lex.count; i++) {
        // For temporary visualization only
        printString(lex.list[i].str);
        printf("\n");
    }

    printf("============================\n");
    info("Press any key to exit");
    fgetc(stdin);
    exit(EXIT_SUCCESS);
}
