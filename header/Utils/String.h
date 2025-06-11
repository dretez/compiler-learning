#ifndef INCLUDE_UTILS_STRING_H_
#define INCLUDE_UTILS_STRING_H_

#include <stddef.h>
#include <sys/types.h>

/**
 * A String containing a pointer to an array of characters, as well as a size
 * tracker for safer memory handling.
 */
typedef struct {
    char *str;
    size_t len;
    size_t allocSize;
} String;

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

/**
 * Frees an allocated String and sets it to NULL, avoiding dangling pointers.
 */
void String_free(String **);
/**
 * Safely resets a string.
 */
void String_clear(String *);

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
 * Prints a String to stdout.
 */
void String_print(String);

#endif // INCLUDE_UTILS_STRING_H_
