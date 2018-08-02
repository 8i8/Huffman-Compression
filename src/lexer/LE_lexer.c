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
 * LE_get_token: Returns a state on reading a token.
 */
unsigned LE_get_token(F_Buf *buf, char c, unsigned state)
{
	int off, token;
        off = token = 0;

	/* If not a token then push back and return*/
	if (c != '<')
		return 0;

	String *str = NULL;
	str = GE_string_init(str);

	/* Read a token */
	while (c != '>' && (c = GE_buffer_fgetc(buf)) != '>' && c != EOF)
	{
		while (isspace(c)) 
			c = GE_buffer_fgetc(buf);

		if (c == '/')
			off = 1, c = GE_buffer_fgetc(buf);

		while (isspace(c)) 
			c = GE_buffer_fgetc(buf);

		if (isalnum(c)) {

			GE_string_add_char(str, c);

			while (isalnum((c = GE_buffer_fgetc(buf))))
				GE_string_add_char(str, c);

			if ((token = LE_check_token(str->str)) == 0)
				fprintf(stderr, "%s: Token not found.\n", __func__);

			while (isspace(c)) 
				c = GE_buffer_fgetc(buf);

			if (c == '/')
				off = 1, c = GE_buffer_fgetc(buf);

			while (isspace(c)) 
				c = GE_buffer_fgetc(buf);
	
			if (off)
				state_unset(state, token);
			else
				state_set(state, token);

			GE_string_reset(str);

		} else
			GE_buffer_ungetc(c, buf);

	}

	GE_string_free(str);


	if (c == '>' || c == EOF)
		return state;
	else
		return 	LE_get_token(buf, c, state);
}

