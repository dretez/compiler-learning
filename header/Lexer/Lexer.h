#ifndef INCLUDE_LEXER_LEXER_H
#define INCLUDE_LEXER_LEXER_H

typedef struct list List;

List *analyzeFile(int fd);
List *runLexicalAnalyzer(List *fd);

#endif /* ifndef INCLUDE_LEXER_LEXER_H */
