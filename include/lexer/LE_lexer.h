
#include <stdio.h>
#include "general/GE_file_buffer.h"

#define TOKEN_MAX 256

/* LE_token_init: Initialise token trie for lexing */
void LE_lexer_init(void);

/* LE_token_free: Free trie memory */
void LE_lexer_free(void);

/* LE_goto_glyph: Move forwards along the file stream to the next occurrence of
 * the given glyph */
char LE_goto_glyph(F_Buf *buf, char c, char glyph);

/* LE_get_token: Returns a state on reading a token */
char LE_get_token(F_Buf *buf, char c, int *st_lex);

/* LE_get_utf8_char: Get the next utf-8 char from the file setream return it as
 * a string */
char LE_get_utf8_char(F_Buf *buf, char *str);

/* LE_get_string: Get the next word upto the next whitespace or EOF */
char LE_get_string(F_Buf *buf, char c, char *str);

