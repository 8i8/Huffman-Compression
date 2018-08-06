#include <stdio.h>
#include <stdlib.h>
#include "lexer/LE_tokenizer.h"
#include "lexer/LE_lexer.h"
#include "lexer/LE_xml.h"
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
 * LE_get_token: Returns state on reading a token, will read recursivly untill
 * the end of element indicator is reached.
 */
int LE_get_token(F_Buf *buf, char c, int st_lex)
{
	/* Skip whitespace upto the next '<' */
	if ((c = LE_xml_goto_token(buf, c)) == 0) {
		fprintf(stderr, "%s: Expected a token.\n", __func__);
		return 0;
	}

	if ((c = LE_xml_read_token(buf, c, &st_lex)) == 0) {
		fprintf(stderr, "%s: Invalid token.\n", __func__);
		return 0;
	}

	if (c == '>' || c == EOF)
		return st_lex;
	else
		return LE_get_token(buf, c, st_lex);
}

