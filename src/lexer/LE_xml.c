#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "lexer/LE_tokenizer.h"
#include "general/GE_error.h"
#include "general/GE_file_buffer.h"
#include "general/GE_utf8.h"
#include "general/GE_state.h"

#define STR_LEN 255

static int check_len(char *str)
{
	int len;
	len = strlen(str);
	assert(len < STR_LEN);
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
	char str[STR_LEN] = { '\0' };
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
	char str[STR_LEN] = { '\0' };
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
	char str[STR_LEN] = { '\0' };
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
	char str[STR_LEN] = { '\0' };
	for (i = 0; i < buf->tab_depth; i++)
		strcat(str, "\t");
	make_open_tag(str, tag);
	strcat(str, item);
	make_close_tag(str, tag);
	strcat(str, "\n");
	GE_buffer_fwrite(str, 1, check_len(str), buf);
	return buf;
}	

/*
 * LE_xml_goto_token: Move forwards allong the file stream to the next token.
 */
char LE_xml_goto_token(F_Buf *buf, char c)
{
	while (isspace(c)) 
		c = GE_buffer_fgetc(buf);

	/* If not a token then push back and return*/
	if (c != '<')
		return 0;

	return c;
}

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
 */
char LE_xml_read_token(F_Buf *buf, char c, int *st_lex)
{
	char *ptr, str[STR_LEN] = { '\0' };
	int utf8_count, off, token;
	utf8_count = off = token = 0;
	ptr = str;

	/* Read */
	while (c != '>' && (c = GE_buffer_fgetc(buf)) != '>' && c != EOF)
	{
		c = in_or_out(buf, c, '/', &off);

		/* valid ? */
		if (!isalnum(c)) {
			GE_buffer_ungetc(c, buf);
			FAIL("Invalid token non alpha numeric character");
			return 0;
		}

		/* Read token */
		while (isalnum((c))
				|| ((utf8_count || (utf8_count = utf8_len(c)))
				&& utf8_count < 4)) {
			*ptr++ = c;
			c = GE_buffer_fgetc(buf);
		}

		/* Does the token exist? */
		if ((token = LE_check_token(str)) == 0)
			FAIL("Token not found");

		c = in_or_out(buf, c, '/', &off);

		/* Set/unset the token */
		if (off)
			state_unset(*st_lex, token);
		else
			state_set(*st_lex, token);

		ptr = str;
		*ptr = '\0';
	}

	return c;
}

