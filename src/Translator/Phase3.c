#include "Translator/Phase3.h"

#include "Translator/myctype.h"
#include "Utils/Logs.h"
#include "Utils/String.h"
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int singleLineCommentStartCheck(String *str, size_t idx) {
    return !String_cmpLiteral(String_getSlice(str, idx, 2), "//");
}

int multiLineCommentStartCheck(String *str, size_t idx) {
    return !String_cmpLiteral(String_getSlice(str, idx, 2), "/*");
}

int wsStartCheck(String *str, size_t idx) {
    return isspace(str->str[idx]);
}

enum ReadState {
    UNKNOWN,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    WHITESPACE,
    TOKEN,
};

/**
 * Preprocessing token type
 */
enum PPTokenType {
    HEADER_NAME,
    IDENTIFIER,
    NUMBER,
    CHAR_CONST,
    STRING_LITERAL,
    PUNCTUATOR,
    UNKNOWN_TOKEN,
};

typedef struct {
    String *str;
    size_t idx;
    enum ReadState state, prevState;
} ReaderData;

void setCurReadState(ReaderData *data);

void handleCurReadState(ReaderData *data);

void readSingleLineComment(ReaderData *data);
void readMultiLineComment(ReaderData *data);
void readWhiteSpace(ReaderData *data);
void readToken(ReaderData *data);

void phase3(String *file) {
    ReaderData data = (ReaderData){
        .str = file,
        .idx = 0,
        .state = UNKNOWN,
        .prevState = UNKNOWN,
    };
    while (data.idx < file->len) {
        handleCurReadState(&data);
    }
}

void handleCurReadState(ReaderData *data) {
    setCurReadState(data);
    switch (data->state) {
    case SINGLE_LINE_COMMENT:
        readSingleLineComment(data);
        break;
    case MULTI_LINE_COMMENT:
        readMultiLineComment(data);
        break;
    case WHITESPACE:
        readWhiteSpace(data);
        break;
    case TOKEN:
        readToken(data);
        break;
    default:
        data->idx++;
        data->prevState = UNKNOWN;
        break;
    }
}

void setCurReadState(ReaderData *data) {
    if (singleLineCommentStartCheck(data->str, data->idx)) {
        data->state = SINGLE_LINE_COMMENT;
        return;
    }
    if (multiLineCommentStartCheck(data->str, data->idx)) {
        data->state = MULTI_LINE_COMMENT;
        return;
    }
    if (wsStartCheck(data->str, data->idx)) {
        data->state = WHITESPACE;
        return;
    }
    data->state = TOKEN;
}

void readSingleLineComment(ReaderData *data) {
    String *str = data->str;
    size_t idx = data->idx + 2;
    while (idx + 1 < str->len && str->str[idx] != '\n')
        idx++;
    String_cutNoPreserve(str, data->idx, idx - data->idx);
    if (data->prevState != WHITESPACE && idx - data->idx > 0)
        String_insertChar(str, ' ', data->idx++);
    data->idx++;
    data->prevState = UNKNOWN;
}

void readMultiLineComment(ReaderData *data) {
    String *str = data->str;
    size_t idx = data->idx + 2;
    while (idx + 1 < str->len &&
           String_cmpLiteral(String_getSlice(str, idx, 2), "*/"))
        idx++;
    if (idx + 1 == str->len) {
        error("Unterminated multi-line comment file found");
        exit(EXIT_FAILURE);
    }
    String_cutNoPreserve(str, data->idx, idx - data->idx + 2);
    if (data->prevState != WHITESPACE && idx - data->idx > 0)
        String_insertChar(str, ' ', data->idx++);
    data->prevState = WHITESPACE;
}

void readWhiteSpace(ReaderData *data) {
    String *str = data->str;
    size_t idx = data->idx;
    while (idx < str->len && isspace(str->str[idx])) {
        if (str->str[idx] != '\n') {
            idx++;
            continue;
        }
        String_cutNoPreserve(str, data->idx, idx - data->idx);
        if (data->prevState != WHITESPACE && idx - data->idx > 0)
            String_insertChar(str, ' ', data->idx++);
        idx = ++data->idx;
        data->prevState = UNKNOWN;
    }
    String_cutNoPreserve(str, data->idx, idx - data->idx);
    if (data->prevState != WHITESPACE && idx - data->idx > 0)
        String_insertChar(str, ' ', data->idx++);
    data->prevState = WHITESPACE;
}

int isIdentifierNondigit(String *str, size_t pos) {
    if (isNondigit(String_getChar(str, pos)))
        return 1;
    if (String_getChar(str, pos) == '\\') {
        switch (String_getChar(str, pos + 1)) {
        case 'U':
        case 'u':
            // TODO: universal-character-name
        default:
            break;
        }
    }
    return 0;
}

void readNumber(ReaderData *data) {
    String *str = data->str;
    size_t idx;
    for (idx = data->idx + 1; idx < str->len; idx++) {
        switch (String_getChar(str, idx)) {
        case 'e':
        case 'E':
        case 'p':
        case 'P':
            if (isSign(String_getChar(str, idx + 1))) {
                idx++;
            }
        case '.':
            continue;
        default:
            if (isdigit(String_getChar(str, idx)))
                continue;
            if (isNondigit(String_getChar(str, idx)))
                continue;
        }
        break;
    }
    String newStr = String_init();
    newStr.str = &str->str[data->idx];
    newStr.len = idx - data->idx;
    printf("Number - ");
    String_print(newStr);
    printf("\n");

    data->idx = idx;
}

void readToken(ReaderData *data) {
    String *str = data->str;
    size_t idx = data->idx;
    enum PPTokenType type;

    switch (str->str[idx]) {
    case '\'':
        type = CHAR_CONST;
        break;
    case '\"':
        type = STRING_LITERAL;
        break;
    case 'L':
        if (String_getChar(str, idx + 1) == '\'') {
            type = CHAR_CONST;
            break;
        }
        if (String_getChar(str, idx + 1) == '\"') {
            type = STRING_LITERAL;
            break;
        }
        // else fallthrough
    case '_':
        type = IDENTIFIER;
        break;
    case '.':
        if (isdigit(String_getChar(str, idx + 1))) {
            type = NUMBER;
            break;
        }
        // else fallthrough
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case '+':
    case '-':
    case '*':
    case '/':
    case '!':
    case '&':
    case '|':
    case '^':
    case '~':
    case '%':
    case '<':
    case '>':
    case '=':
    case '?':
    case ':':
    case ';':
    case ',':
    case '#':
        type = PUNCTUATOR;
        break;
    default:
        if (isdigit(str->str[idx])) {
            type = NUMBER;
            break;
        }
        if (isalpha(str->str[idx])) {
            type = IDENTIFIER;
            break;
        }
        type = UNKNOWN_TOKEN;
    }

    switch (type) {
    case NUMBER:
        readNumber(data);
        break;
    case HEADER_NAME:
    case IDENTIFIER:
    case CHAR_CONST:
    case STRING_LITERAL:
    case PUNCTUATOR:
    case UNKNOWN_TOKEN:
        while (idx < str->len && !singleLineCommentStartCheck(str, idx) &&
               !multiLineCommentStartCheck(str, idx) &&
               !wsStartCheck(str, idx)) {
            idx++;
        }
        String newStr = String_init();
        newStr.str = &str->str[data->idx];
        newStr.len = idx - data->idx;
        printf("Token - ");
        String_print(newStr);
        printf("\n");
        data->idx = idx;
    }

    data->prevState = UNKNOWN;
}
