#include <stdio.h>

#define TOKEN_MAX 256

/* LE_token_init: Initialise token trie for lexing */
void LE_lexer_init(void);

/* LE_token_free: Free trie memory */
void LE_lexer_free(void);

/* LE_get_token: Returns a state on reading a token */
int LE_get_token(FILE *fp, char c, int status);

