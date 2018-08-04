#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer/LE_tokenizer.h"
#include "lexer/LE_lexer.h"
#include "general/GE_string.h"
#include "general/GE_string.h"
#include "general/GE_state.h"

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
 * in_or_out: Returns the state of a search for a specified char having removed
 * whitespace.
 */
static inline char in_or_out(F_Buf *buf, char c, char in, int *off)
{
	while (isspace(c) && c != EOF) 
		c = GE_buffer_fgetc(buf);

	if (c == in)
		*off = 1, c = GE_buffer_fgetc(buf);

	while (isspace(c) && c != EOF) 
		c = GE_buffer_fgetc(buf);

	return c;
}

/*
 * LE_get_token: Returns a state on reading a token.
 */
int LE_get_token(F_Buf *buf, char c, int st_lex)
{
	String *str = NULL;
	int off, token;
        off = token = 0;

	/* Brutal, eat File until a token is found */
	while (c != '<' && c != EOF) 
		c = GE_buffer_fgetc(buf);

	/* If not a token then push back and return*/
	if (c != '<')
		return 0;

	/* String to temporarily store the newly retrieved token */
	str = GE_string_init(str);

	/* Read */
	while (c != '>' && (c = GE_buffer_fgetc(buf)) != '>' && c != EOF)
	{
		c = in_or_out(buf, c, '/', &off);

		/* valid ? */
		if (!isalnum(c)) {
			GE_buffer_ungetc(c, buf);
			fprintf(stderr, "%s: Invalid token non alpha numeric character.\n", __func__);
			return 0;
		}

		/* Read token, must be ASCII char */
		while (isalnum((c = GE_buffer_fgetc(buf))))
			GE_string_add_char(str, c);

		/* Does the token exist? */
		if ((token = LE_check_token(str->str)) == 0)
			fprintf(stderr, "%s: Token not found.\n", __func__);

		c = in_or_out(buf, c, '/', &off);

		/* Set/unset the token */
		if (off)
			state_unset(st_lex, token);
		else
			state_set(st_lex, token);

		GE_string_reset(str);
	}

	GE_string_free(str);

	if (c == '>' || c == EOF)
		return st_lex;
	else
		return LE_get_token(buf, c, st_lex);
}

