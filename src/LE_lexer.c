#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "LE_tokenizer.h"
#include "GE_string.h"
#include "LE_lexer.h"
#include "GE_string.h"
#include "GE_state.h"

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
 * TODO NEXT
 */
unsigned LE_get_token(FILE *fp, char c, unsigned state)
{
	int off, token;
        off = token = 0;

	while (isspace(c)) 
		c = fgetc(fp);

	/* If not a token then push back and return*/
	if (c != '<') {
		ungetc(c, fp);	
		return state;
	}

	while (c != '>' && c != EOF)
	{
		while (isspace(c)) 
			c = fgetc(fp);

		if (c == '/')
			off = 1;

		String *str = malloc(sizeof(String));

		while (isalnum((c = fgetc(fp))))
			GE_string_add_char(str, c);

		state_set(token, LE_check_token(str->str));

		free(str);

		if (off)
			state_unset(state, token);
		else
			state_set(state, token);
	}

	if (c == '>' || c == EOF)
		return state;
	else
		return 	LE_get_token(fp, c, state);
}

