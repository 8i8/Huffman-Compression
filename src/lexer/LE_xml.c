#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "lexer/LE_tokenizer.h"
#include "general/GE_error.h"
#include "general/GE_file_buffer.h"
#include "general/GE_utf8.h"
#include "general/GE_state.h"

#define TOKEN_MAX 255  /* Max length of a token */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  write xml objects
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int check_len(char *str)
{
	int len;
	len = strlen(str);
	assert(len < TOKEN_MAX);
	return len;
}

static char *make_open_tag(char *str, char *element)
{
	strcat(str, "<");
	strcat(str, element);
	strcat(str, ">");
	return str;
}

static char *make_close_tag(char *str, char *element)
{
	strcat(str, "</");
	strcat(str, element);
	strcat(str, ">");
	return str;
}

/*
 * LE_xml_element_open: Write an opening xml element.
 */
F_Buf *LE_xml_element_open(F_Buf *buf, char *token)
{
	int i;
	char str[TOKEN_MAX] = { '\0' };
	for (i = 0; i < buf->tab_depth; i++)
		strcat(str, "\t");
	make_open_tag(str, token);
	strcat(str, "\n");
	GE_buffer_fwrite(str, 1, check_len(str), buf);
	buf->tab_depth++;
	return buf;
}

/*
 * LE_xml_element_close: Write an closing xml element.
 */
F_Buf *LE_xml_element_close(F_Buf *buf, char *token)
{
	int i;
	char str[TOKEN_MAX] = { '\0' };
	buf->tab_depth--;
	for (i = 0; i < buf->tab_depth; i++)
		strcat(str, "\t");
	make_close_tag(str, token);
	strcat(str, "\n");
	GE_buffer_fwrite(str, 1, check_len(str), buf);
	return buf;
}

/*
 * LE_xml_add_word_pair: Add a word pair at the current tab depth..
 */
F_Buf *LE_xml_element_map(F_Buf *buf, char *st1, char *st2)
{
	int i;
	char str[TOKEN_MAX] = { '\0' };
	for (i = 0; i < buf->tab_depth; i++)
		strcat(str, "\t");
	strcat(str, "<ch>");
	strcat(str, st1);
	strcat(str, " ");
	strcat(str, st2);
	strcat(str, "</ch>\n");
	GE_buffer_fwrite(str, 1, check_len(str), buf);
	return buf;
}

/*
 * LE_xml_element_item: Add an xml item at the current tab depth.
 */
F_Buf *LE_xml_element_item(F_Buf *buf, char *item, char *tag)
{
	int i;
	char str[TOKEN_MAX] = { '\0' };
	for (i = 0; i < buf->tab_depth; i++)
		strcat(str, "\t");
	make_open_tag(str, tag);
	strcat(str, item);
	make_close_tag(str, tag);
	strcat(str, "\n");
	GE_buffer_fwrite(str, 1, check_len(str), buf);
	return buf;
}	

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Parse xml objects
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * in_or_out: Returns the state of a search for a specified char having removed
 * whitespace.
 */
static char in_or_out(F_Buf *buf, char c, char in, int *off)
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
 * LE_xml_read_token: Read and verify the token, set state acordingly.
 * TODO NEXT pushback written, is it working?
 */
char LE_xml_read_token(F_Buf *buf, char c, int *st_lex)
{
	char *ptr, str[TOKEN_MAX] = { '\0' };
	int utf8_count, off, token, pushback;
	utf8_count = off = token = pushback = 0;
	ptr = str;

	/* Read */
	while (c != '>' && (c = GE_buffer_fgetc(buf)) != '>' && c != EOF)
	{
		c = in_or_out(buf, c, '/', &off);

		/* valid ? */
		if (!isalnum(c) && !is_set(*st_lex, LEX_DECOMPRESS)) {
			GE_buffer_ungetc(c, buf);
			FAIL("Invalid token non alpha numeric character");
			return 0;
		}

		GE_buffer_pushback_mark(buf);

		/* Read token: pushback ok, alnum or utf8 ... ? */
		//TODO NEXT deal with the case of pushback going over the end
		//of the buffer.
		//TODO NOW this seems to be the cause of the problem at
		//runtime.
		while (pushback < TOKEN_MAX 
				&& (isalnum((c))
				|| ((utf8_count || (utf8_count = utf8_len(c)))
				&& utf8_count < 4))) {
			*ptr++ = c, pushback++;
			c = GE_buffer_fgetc(buf);
		}

		/* Does the token exist, if not return to pushback mark */
		if ((token = LE_check_token(str)) == 0) {
			WARNING("Token not found in hashtable");
			c = GE_buffer_pushback_goto(buf);
		}

		GE_buffer_pushback_unmark(buf);
		pushback = 0;

		c = in_or_out(buf, c, '/', &off);

		/* Set/unset the token */
		if (off)
			state_unset(*st_lex, token);
		else
			state_set(*st_lex, token);

		ptr = str;
		*ptr = '\0';
	}
	if (c == EOF)
		FAIL("EOF reached");
	return c;
}

