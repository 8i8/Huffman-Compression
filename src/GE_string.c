#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STR_BUF_INIT 4

#ifndef _string_
typedef struct {
	char *str;
	char *ptr;
	size_t buf;
} String;
#endif

/*
 * GD_string_init: Initiate a simple string struct that has a growth function.
 */
String *GE_string_init(String *Str)
{
	Str = malloc(sizeof(*Str));
	Str->buf = STR_BUF_INIT;
	Str->ptr = Str->str = calloc(Str->buf+1, 1);

	return Str;
}

/*
 * _string_len: Buffer for simple string struct.
 */
String *GE_string_len(String *Str, size_t len)
{
	size_t pos;

	if (len >= Str->buf) {
		pos = Str->ptr - Str->str;
		Str->buf <<= 1;
		Str->ptr = Str->str = realloc(Str->str, Str->buf+1);
		Str->ptr += pos;
	}

	return Str;
}

/*
 * GE_string_set: Set the buffer memory to and adequate size and then copy the
 * given string into the struct, set also the string pointer to the beginning of
 * the allocated memory address.
 */
String *GE_string_set(String *Str, char *str)
{
	Str = GE_string_len(Str, strlen(str));
	strcpy(Str->str, str);
	Str->ptr = Str->str;
	return Str;
}

/*
 * GE_string_free: Destroy string.
 */
void GE_string_free(String *Str)
{
	free(Str->str);
	free(Str);
}

