#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "general/GE_state.h"
#include "general/GE_utf8.h"
#include "general/GE_print.h"
#include "lexer/LE_lexer.h"
#include "data_structures/DS_mergesort.h"
#include "data_structures/DS_huffman_tree.h"
#include "data_structures/DS_huffman_tree.h"

/*
 * compression_frequency_list: Sort alphabetically and keep count of each
 * occurrences of every character used.
 */
static HC_HuffmanNode **compression_frequency_list(
						HC_HuffmanNode **list,
						F_Buf **io,
						const int st_prg)
{
	char c, *ptr;
	size_t i, utf8_count;
	Data data;
	DS_huffman_data_init(&data);
	utf8_count = 0;

	for (i = is_set(st_prg, COMPRESS); i < MAX_FILES && io[i]; i++) {
		GE_buffer_on(io[i]);
		while ((c = GE_buffer_fgetc(io[i])) != EOF)
		{
			ptr = data.utf8_char;

			//TODO NOW is this required still?
			//if (isspace(c))
			//	*ptr++ = '\\';

			/* Add char, check if multi-byte character */
			while ((*ptr++ = c)
					&& (utf8_count || (utf8_count = utf8_len(c)))
					&& utf8_count < 4)
				c = GE_buffer_fgetc(io[i]), utf8_count--;

			*ptr = '\0';
			data.frq = 1;
			DS_huffman_tree_insert_or_count(list, data, FN_data_strcmp);
		}

		/* Add EOF char */
		memcpy(data.utf8_char, "EOF", 4), data.frq = 1;
		DS_huffman_tree_insert_or_count(list, data, FN_data_strcmp);

		rewind(io[i]->fp);
		GE_buffer_off(io[i]);
	}

	return list;
}

/*
 * decompression_frequency_list: Compile a frequency list from the
 * table at the start of a compressed file.
 */
static int decompression_frequency_list(
							HC_HuffmanNode **list,
							F_Buf *buf,
							int st_lex)
{
	char c = 0;
	size_t utf8_count;
	utf8_count = 0;

	Data d, *data;
	data = &d;
	DS_huffman_data_init(data);
	String *str = NULL;
	str = GE_string_init(str);

	/* Get and remove line end */
	GE_buffer_skip(buf, 1);

	while (is_set(st_lex, LEX_MAP))
	{
		/* remove tab */
		c = GE_buffer_fgetc(buf);

		/* escape char */
 		// TODO NOW: char written into tree node here.
		//if (isspace(c))
		//	GE_string_add_char(str, '\\');

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
		memcpy(data->utf8_char, str->str, str->len+1);
		GE_string_reset(str);

		/* Check for utf-8 errors */
		if (utf8_count > 4)
			fprintf(stderr, "%s: utf8_countdown error.\n", __func__);

		/* next is a space, skip it */
		c = GE_buffer_fgetc(buf);

		/* Get frequency */
		while (isalnum(c)) {
			GE_string_add_char(str, c);
			c = GE_buffer_fgetc(buf);
		}

		/* Write frq to data struct */
		data->frq = strtol(str->str, &(str->ptr), 10);
		GE_string_reset(str);

		/* remove line end */
		c = GE_buffer_fgetc(buf);

		/* Add to huffman linkedlist */
		DS_huffman_tree_add(list, *data);

		/* Check for a to end the list */
		if (c == '<')
			st_lex = LE_get_token(buf, c, st_lex);
	}

	GE_string_free(str);

	return st_lex;
}

/*
 * priority_queue: Compile a frequency list for all characters in the
 * document, sort that list to make a priority queue.
 */
int priority_queue(
					HC_HuffmanNode **list,

					F_Buf **io,
					const int st_prg)
{
	if (is_set(st_prg, VERBOSE))
		fwrite("Create priority queue.\n", 1, 23, stdout);

	/* Get char count */
	if (compression_frequency_list(list, io, st_prg) == NULL)
		fprintf(stderr, "%s(): error compression_frequency_list failed.\n", __func__);

	/* Sort by frequency */
	if (DS_mergesort(list, FN_data_frqcmp) == NULL)
		fprintf(stderr, "%s(): error mergesort failed.\n", __func__);

	if (is_set(st_prg, PRINT)) {
		fwrite("print frequency map.\n", 1, 22, stdout);
		print_frequency_map(*list);
	}

	return st_prg;
}

/*
 * decompression_priority_queue: Retrieve the frequency map from the beginning
 * of a compressed file and make it into a list, sort the list into a priority
 * queue.
 */
int decompression_priority_queue(
							HC_HuffmanNode **list,
							F_Buf *buf,
							int st_lex)
{
	/* Get char count */
	if ((st_lex = decompression_frequency_list(list, buf, st_lex)) == 0)
		fprintf(stderr, "%s(): error decompression_frequency_list failed.\n", __func__);

	/* Sort by frequency */
	if (DS_mergesort(list, FN_data_frqcmp) == NULL)
		fprintf(stderr, "%s(): error mergesort failed.\n", __func__);

	return st_lex;
}

