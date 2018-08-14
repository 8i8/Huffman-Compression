#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "general/GE_error.h"
#include "general/GE_state.h"
#include "general/GE_utf8.h"
#include "program/PG_print.h"
#include "lexer/LE_lexer.h"
#include "huffman/HC_mergesort.h"
#include "huffman/HC_huffman_tree.h"

/*
 * FN_data_strcmp: Compare Data one and two, the value should be a single char and
 * the result alphabetical order as per the ASCII char numbering system.
 */
int FN_data_strcmp(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	return strcmp(d1->utf8_char, d2->utf8_char);
}

/*
 * frequency_list_from_text: Sort alphabetically and keep count of each
 * occurrences of every character used.
 */
static HC_HuffmanNode **frequency_list_from_text(
						HC_HuffmanNode **list,
						F_Buf *buf)
{
	char c, *ptr;
	size_t utf8_count, len;
	Data data;
	data = HC_data_init();
	utf8_count = 0;

	GE_buffer_on(buf);
	while ((c = GE_buffer_fgetc(buf)) != EOF)
	{
		/* reset */
		ptr = data.utf8_char, len = 1;;

		/* Add char, check if multi-byte character */
		while ((*ptr++ = c)
				&& (utf8_count || (len = utf8_count = utf8_len(c)))
				&& utf8_count < 4)
			c = GE_buffer_fgetc(buf), utf8_count--;

		*ptr = '\0', data.frq = 1, data.len_char = len;
		DS_huffman_tree_insert_or_count(list, data, FN_data_strcmp);
	}

	/* Add EOF char */
	memcpy(data.utf8_char, "EOF", 4), data.frq = 1;
	DS_huffman_tree_insert_or_count(list, data, FN_data_strcmp);

	rewind(buf->fp);
	GE_buffer_off(buf);

	return list;
}

/*
 * frequency_list_from_metadata: Compile a frequency list from the
 * table at the start of a compressed file.
 */
static int frequency_list_from_metadata(
							HC_HuffmanNode **list,
							F_Buf *buf,
							int st_lex)
{
	char c = 0;
	size_t utf8_count;
	utf8_count = 0;
	Data data;
	data = HC_data_init();
	String *str = NULL;
	str = GE_string_init(str);

	/* Get and remove line end */
	GE_buffer_skip(buf, 1);

	while (is_set(st_lex, LEX_MAP))
	{
		/* remove tab */
		c = GE_buffer_fgetc(buf);

		/* get utf-8 char */
		while (GE_string_add_char(str, c)
				&& (utf8_count || (utf8_count = utf8_len(c)))
				&& utf8_count < 4)
			c = GE_buffer_fgetc(buf), utf8_count--;

		/* Deal with EOF character */
		if ((c = GE_buffer_fgetc(buf)) == 'O') {
			GE_string_add_char(str, c);
			c = GE_buffer_fgetc(buf);
			GE_string_add_char(str, c);
			c = GE_buffer_fgetc(buf);
		}

		/* Write char to data struct */
		memcpy(data.utf8_char, str->str, str->len+1);
		GE_string_reset(str);

		/* Check for utf-8 errors */
		if (utf8_count > 4)
			FAIL("utf8_countdown error");

		/* next is a space, skip it */
		c = GE_buffer_fgetc(buf);

		/* Get frequency */
		while (isalnum(c)) {
			GE_string_add_char(str, c);
			c = GE_buffer_fgetc(buf);
		}

		/* Write frq to data struct */
		data.frq = strtol(str->str, &(str->ptr), 10);
		GE_string_reset(str);

		/* remove line end */
		c = GE_buffer_fgetc(buf);

		/* Add to huffman linkedlist */
		DS_huffman_tree_add(list, data);

		/* Check for a to end the list */
		if (c == '<')
			st_lex = LE_get_token(buf, c, &st_lex);
	}

	GE_string_free(str);

	return st_lex;
}

/*
 * priority_queue: Compile a frequency list for all characters in the
 * document, sort that list to make a priority queue.
 */
int frequency_list_compression(
							HC_HuffmanNode **list,
							F_Buf *buf,
							const int st_prg)
{
	if (is_set(st_prg, VERBOSE))
		fprintf(stdout, "Create priority queue %s.\n", buf->name);

	/* Get char count */
	if (frequency_list_from_text(list, buf) == NULL)
		FAIL("frequency list failed");

	if (is_set(st_prg, PRINT)) {
		fprintf(stdout, "print frequency map %s.\n", buf->name);
		print_frequency_map(*list);
	}

	return st_prg;
}

/*
 * decompress_priority_queue: Retrieve the frequency map from the beginning
 * of a compressed file and make it into a list, sort the list into a priority
 * queue.
 */
int decompress_priority_queue(
							HC_HuffmanNode **list,
							F_Buf *buf,
							int st_lex)
{
	/* Get char count */
	if ((st_lex = frequency_list_from_metadata(list, buf, st_lex)) == 0)
		FAIL("metadata frequency list failed");

	/* Sort by frequency */
	if (HC_mergesort(list, FN_data_frqcmp) == NULL)
		FAIL("mergesort failed");

	return st_lex;
}

