#include <stdio.h>

#ifndef string_
#define string_
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
 * GE_string_free: Destroy string.
 */
void GE_string_free(String *Str);

/*
 * GE_string_stack_init: Initalise a string whos struct is allocated on the
 * stack.
 */
String GE_string_stack_init(String str);

/*
 * GE_string_stack_free: Free the heap alocated string inside of a stack
 * alocated String struct.
 */
void GE_string_stack_free(String str);

/*
 * GE_string_len: Buffer for simple string struct.
 */
String *GE_string_len(String *Str, size_t len);

/*
 * GE_string_getchar: Get char from stdin.
 */
String *GE_string_getchar(String *Str);

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

