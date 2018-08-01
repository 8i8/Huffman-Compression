#include <stdio.h>

#ifndef _string_
#define _string_
typedef struct {
	char *str;
	char *ptr;
	size_t len;
	size_t buf;
} String;
#endif

#define STR_BUF_INIT 8

/*
 * GD_string_init: Innitiate a sinple string struct that has a growth function.
 */
String *GE_string_init(String *Str);

/*
 * _string_len: Buffer for simple string struct.
 */
String *GE_string_len(String *Str, size_t len);

/*
 * GE_string_set: Set the buffer memory to and adiquate size and then copy the
 * given string into the struct, set also the string pointer to the begining of
 * the allocated memory address.
 */
String *GE_string_set(String *Str, char *str);

/*
 * GE_string_add_char: Add a single char to the string, buffer that string if
 * required.
 */
String *GE_string_add_char(String *Str, char c);

/*
 * GE_string_rem_char: Remove a single char froṁ the string.
 */
String *GE_string_rem_char(String *Str);

/*
 * GE_string_concat: Add a string to the end of the current string.
 */
String *GE_string_concat(String *Str, char *string, int len);

/*
 * GE_string_reset: Set string to 0.
 */
String *GE_string_reset(String *Str);

/*
 * GE_string_free: Destroy string.
 */
void GE_string_free(String *Str);
