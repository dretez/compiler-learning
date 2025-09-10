#include "Translator/Phase3.h"

#include "Utils/Logs.h"
#include "Utils/String.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum ReadState {
    UNKNOWN,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    WHITESPACE,
    TOKEN,
};

enum TokenTypes {
    HEADER_NAME,
    IDENTIFIER,
    NUMBER,
    CHAR_CONST,
    STRING_LITERAL,
    OPERATOR,
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
    if (!String_cmpLiteral(String_getSlice(data->str, data->idx, 2), "//")) {
        data->state = SINGLE_LINE_COMMENT;
        return;
    }
    if (!String_cmpLiteral(String_getSlice(data->str, data->idx, 2), "/*")) {
        data->state = MULTI_LINE_COMMENT;
        return;
    }
    if (isspace(data->str->str[data->idx])) {
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

void readToken(ReaderData *data) {
    String *str = data->str;
    size_t idx = data->idx;
    data->idx++;
    data->prevState = UNKNOWN;
}
