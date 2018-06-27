#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GE_string.h"

/*
 * GD_string_init: Initiate a simple string struct that has a growth function.
 */
String *GE_string_init(String *Str)
{
	Str = malloc(sizeof(*Str));
	Str->buf = STR_BUF_INIT;
	Str->len = 0;
	Str->ptr = Str->str = calloc(Str->buf+1, 1);

	return Str;
}

/*
 * _string_len: Buffer for simple string struct.
 */
String *GE_string_len(String *Str, size_t len)
{
	if (len >= Str->buf) {
		Str->len = Str->ptr - Str->str;
		Str->buf <<= 1;
		Str->ptr = Str->str = realloc(Str->str, Str->buf+1);
		Str->ptr += Str->len;
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
	Str->len = strlen(str);
	Str = GE_string_len(Str, Str->len);
	strcpy(Str->str, str);
	Str->ptr = Str->str;
	return Str;
}

/*
 * GE_string_add: Add a single char to the string, buffer that string if
 * required.
 */
String *GE_string_add(String *Str, char c)
{
	Str->len++;
	if (Str->len >= Str->buf) {
		Str->buf <<= 1;
		Str->ptr = Str->str = realloc(Str->str, Str->buf+1);
		Str->ptr += Str->len;
	}
	*Str->ptr++ = c;
	Str->ptr = '\0';
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

