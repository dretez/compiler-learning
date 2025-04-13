#ifndef INCLUDE_UTILS_STRING_H_
#define INCLUDE_UTILS_STRING_H_

#include <stddef.h>
#include <sys/types.h>

typedef struct {
    char *str;
    size_t len;
} String;

String newEmptyString();
String newString(char *str, size_t len);

int stringCpy(String to, String from);
int stringCmp(String s1, String s2);

#endif // INCLUDE_UTILS_STRING_H_
