#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <assert.h>
#include "lexer/LE_tokenizer.h"
#include "lexer/LE_lexer.h"
#include "lexer/LE_xml.h"
#include "general/GE_error.h"
#include "general/GE_string.h"
#include "general/GE_state.h"
#include "general/GE_utf8.h"

/*
 * LE_token_init: Initialise token trie for lexing.
 */
void LE_lexer_init(void)
{
	LE_token_init();
}

/*
 * LE_token_free: Free trie memory.
 */
void LE_lexer_free(void)
{
	LE_token_free();
}

/*
 * goto_next_word: Move forwards until the next textual word is found.
 */
void goto_next_word(int c, F_Buf *buf)
{
	while (!isalnum((c)) && !utf8_len(c) && c != EOF)
		c = GE_buffer_fgetc(buf);

	/* If not a token then push back and return*/
	if (c == EOF)
		FAIL("end of file reached");
	else
		GE_buffer_ungetc(c, buf);
}

/*
 * LE_goto_glyph: Move forwards along the file stream to the next occurrence of
 * the given glyph.
 */
char LE_goto_glyph(F_Buf *buf, char c, char glyph)
{
	/* Brutal read until the glyph is found or EOF is reached, whichever
	 * happens first, assumes that a token will be the next relevant
	 * information on the stream */
	while (c != glyph && c != EOF)
		c = GE_buffer_fgetc(buf);

	/* If not a token then push back and return */
	if (c != glyph)
		FAIL("glyph not found in file stream");

	return c;
}

/*
 * LE_get_token: Returns state on reading a token, will read recursively until
 * the end of element indicator is reached.
 */
char LE_get_token(F_Buf *buf, char c, int *st_lex)
{
	c = LE_goto_glyph(buf, c, '<');

	if (c != '<') {
		FAIL("expected a '<' ...");
		return 0;
	}

	/* read and verify the validity, whilst setting the state of any valid
	 * tokens read */
	if ((c = LE_xml_read_token(buf, c, st_lex)) == 0) {
		state_set(*st_lex, LEX_ERROR);
		FAIL("parsing");
		return c;
	}

	if (c == '>')
		return c;
	else
		LE_get_token(buf, c, st_lex);
	return c;
}

/*
 * LE_read_ahead: Check ahead for a token.
 * TODO NEXT requires case for nearing the buffers end written.
 * TODO NEXT write this function
 */
int LE_look_ahead(F_Buf *buf, char glyph, ptrdiff_t dist)
{
	ptrdiff_t *end, *read;
	char *ptr;
        end = (ptrdiff_t*)buf->end;
	read = (ptrdiff_t*)buf->read;
        ptr = buf->read;

	assert(end - read > dist);

	while (ptr < buf->read + dist && ptr < buf->end)
		if (*++ptr == glyph)
			return 1;
	return 0;
}

/*
 * LE_get_utf8_char: Get the next utf-8 char from the file stream return it as
 * a string, requires a string of at least 5 bytes.
 */
char LE_get_utf8_char(F_Buf *buf, char *ptr)
{
	int utf8_count = 0;
	char c = GE_buffer_fgetc(buf);

	/* Get the next char regardless of what it is, if multibyte, transfer
	 * the entire string of char */
	while ((*ptr++ = c)
			&& (utf8_count || (utf8_count = utf8_len(c)))
			&& utf8_count < 3)
		c = GE_buffer_fgetc(buf), utf8_count--;

	if (utf8_count > 3)
		FAIL("utf-8 character read failed");

	*ptr = '\0';

	c = GE_buffer_fgetc(buf);
	while (isspace(c))
		c = GE_buffer_fgetc(buf);

	return c;
}

/*
 * LE_get_string: Get the next word up until the next whitespace or EOF
 * character, return that character.
 */
char LE_get_string(F_Buf *buf, char c, char *str)
{
	int utf8_count = 0;
	while ((isalnum(c)
			|| ((utf8_count || (utf8_count = utf8_len(c)))
				&& utf8_count < 3))
			&& c != EOF) {
		*str++ = c;
		c = GE_buffer_fgetc(buf);
	}
	*str = '\0';

	return c;
}

