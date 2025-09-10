#ifndef INCLUDE_UTILS_STRING_H_
#define INCLUDE_UTILS_STRING_H_

#include <stddef.h>
#include <sys/types.h>

/**
 * A String containing a pointer to an array of characters, as well as a size
 * tracker for safer memory handling.
 */
typedef struct string {
    char *str;
    size_t len;
    size_t allocSize;
} String;

/******************************** CONSTRUCTION ********************************/

/**
 * Provides default values for a String object.
 */
String String_init();
/**
 * Allocates a String to memory and initializes it.
 * Returns NULL if allocation fails.
 */
String *String_new();
/**
 * Initializes a String object using a C string.
 */
String String_fromCString(char *str, size_t len);

/********************************** CLEAN UP **********************************/

/**
 * Frees an allocated String and sets it to NULL, avoiding dangling pointers.
 */
void String_free(String **);
/**
 * Safely resets a string.
 */
void String_clear(String *);

/**
 * TODO:
 */
void String_clearUnusedMem(String *str);

/******************************** STRING DATA ********************************/

size_t String_len(String *str);

size_t String_allocSize(String *str);

/**
 * Returns a character from a String at the given position.
 *
 * @return The char at the requested position, or 0 if the position overflows
 * the String.
 */
char String_getChar(String *str, size_t pos);

/**
 * TODO:
 */
String String_getSlice(String *str, size_t start, size_t len);

/***************************** STRING OPERATIONS *****************************/

/**
 * Reallocates a String with the given size.
 * Useful when the final size is know beforehand, avoiding unnecessary
 * allocations.
 *
 * @return The new string size, or -1 in case of failure.
 */
int String_resize(String *str, size_t size);

/**
 * Copies the contents of a string into another.
 *
 * @return The size of the copied string, or -1 in case of failure.
 */
int String_cpy(String *to, String from);
/**
 * Compares 2 Strings.
 *
 * @return 0, if both Strings are equal;
 * >= 1, if the contents of s1 are greater than the contents of s2;
 * <= -1, if the contents of s1 are smaller than the contents of s2;
 */
int String_cmp(String s1, String s2);

/**
 * Compares a String object to a string literal.
 * @warn This function should only be used with string literals. In any other
 * situation, responsibility falls on the user to ensure the string is null
 * terminated ('\0').
 *
 * @return 0, if both Strings are equal;
 * >= 1, if the contents of s1 are greater than the contents of s2;
 * <= -1, if the contents of s1 are smaller than the contents of s2;
 */
int String_cmpLiteral(String s1, char *s2);

/**
 * Cuts a section of a String and returns it as a new String object. If the
 * creation of the new String object fails, this function has the same behaviour
 * as String_cutNoPreserve.
 *
 * @return A pointer to a String object containing the section cut from the
 * original String, or NULL, if the creation of the new String object fails.
 */
String *String_cut(String *str, size_t start, size_t len);

/**
 * Cuts a section of a String without preserving the data within that section.
 */
void String_cutNoPreserve(String *str, size_t start, size_t len);

/**
 * TODO:
 */
void String_insertChar(String *str, char c, size_t pos);

/**
 * TODO:
 */
void String_insert(String *str, String *data, size_t pos);

/**
 * Prints a String to stdout.
 */
void String_print(String);

#endif // INCLUDE_UTILS_STRING_H_
