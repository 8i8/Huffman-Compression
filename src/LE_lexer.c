#include <stdio.h>
#include <ctype.h>
#include "LE_tokenizer.h"
#include "HC_state.h"
#include "GE_string.h"
#include "LE_lexer.h"

extern int state;

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
//TODO NOW 5 returns the state; Needs token read from tokenizer.
int LE_get_token(FILE *fp, char c)
{
	while (isspace(c)) 
		c = fgetc(fp);

	/* If not a token then push back and return*/
	if (c != '<') {
		ungetc(c, fp);	
		return state;
	}

	while (c != '>')
	{
		while (isspace(c)) 
			c = fgetc(fp);

		if (c == '/')
			state &= LE_OUT;
		else
			state &= LE_IN;

		//TODO NOW c = _read_token(fp, c);
	}

	return state;
}

