#include <ctype.h>
#include "LE_lexer.h"
#include "HC_state.h"

typedef struct _token_chain {
	char *id;
	int flag;
	_token_chain *next;
} Token;

static Tokens **token_table;

void token_add(char *token, int flag)
{
	int bucket = hash(token);
	Token *new_token;
       
	if ((new_token = malloc(sizeof(Token)) == NULL))
		goto on_error;

	new_token->id = strdup(token);
	new_token->flag = flag;
	new_token->next = NULL;

	if (token_table[bucket]) {
		Token current = token[bucket];
		while (current->next)
			current = current->next;
		current->next = new_token;
	} else
		token_table[bucket] = new_token;

	return 0;

on_error:
	return 1;
}

void token_init()
{
	int state = 0;
	if ((token_table = calloc(256 * sizeof(Token))) == NULL)
		goto on_error;

	token_add("map", state & LE_MAP);
	token_add("comp", state & LE_DECOMP);

	return 0;

on_error:
	return 1;
}

char LE_read_token(FILE *fp, char c)
{
	size_t len = 0;
	char token[TOKEN_MAX], *pt;
	pt = &token[0];
	*pt = c;

	while (isalpha(c) && ++len <= TOKEN_MAX)
		*++pt = c = fgetc(fp);

	//TODO NOW DS_Trie_step(c);

	return c;
}

