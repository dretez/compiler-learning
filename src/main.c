#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "Lexer/Token.h"
#include "Lexer/TokenList.h"
#include "Utils/Logs.h"
#include "Parser/Parsing.h"

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

    TokenList lex = TokenList_init();
    for (Token token = TokenReader_nextFromFile(f); token.type != EOF_TOKEN;
         token = TokenReader_nextFromFile(f)) {
        TokenList_add(&lex, token);
    }
    // TokenList_print(lex);

    /******************************** PARSING ********************************/

    parse(lex);

    printf("============================\n");
    info("Press any key to exit");
    fgetc(stdin);
    exit(EXIT_SUCCESS);
}
