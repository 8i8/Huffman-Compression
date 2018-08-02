#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "general/GE_state.h"
#include "general/GE_utf8.h"
#include "lexer/LE_lexer.h"
#include "data_structures/DS_mergesort.h"
#include "data_structures/DS_huffman_tree.h"
#include "data_structures/DS_huffman_node.h"
#include "huffman/HC_print.h"

/*
 * insert_or_count: If the char does not yet exist in the char map,
 * insert it in alphabetical order. If it exists already, add one to the count
 * for that character.
 */
static HC_HuffmanNode **insert_or_count(
						HC_HuffmanNode **list,
						Data data,
						int(*func)(void*, void*))
{
	int test;
	HC_HuffmanNode *rtn;

	if (list == NULL) {
		fprintf(stderr, "%s: NULL pointer.", __func__);
		return NULL;
	} else if (*list == NULL) {
		*list = DS_huffman_tree_new_node(data);
		return list;
	}

	rtn = *list;

	while (*list != NULL)
	{
		if ((test = (*func)((void*)&data, (void*)&(*list)->data)) < 0) {
			if ((DS_huffman_tree_insert(list, data)) == NULL) {
				fprintf(stderr, "%s: ", __func__);
				return NULL;
			}
			break;
		} else if (test == 0) {
			(*list)->data.frq++;
			break;
		} else if (test > 0)
			list = &(*list)->next;
		if (*list == NULL) {
			DS_huffman_tree_add(list, data);
			break;
		}
	}

	list = &rtn;

	return list;
}

/*
 * frequency_list_compression: Sort alphabetically and keep count of each
 * occurrences of every character used.
 */
static HC_HuffmanNode **frequency_list_compression(
						HC_HuffmanNode **list,
						F_Buf **io,
						const unsigned state)
{
	char c, *ptr;
	size_t i, utf8_count;
	Data data;
	DS_huffman_data_init(&data);
	utf8_count = 0;

	for (i = is_set(state, COMPRESS); i < MAX_FILES && io[i]; i++) {
		GE_buffer_on(io[i]);
		while ((c = GE_buffer_fgetc(io[i])) != EOF)
		{
			ptr = data.utf8_char;

			/* Add char, check if multi-byte character */
			while ((*ptr++ = c)
					&& (utf8_count || (utf8_count = utf8_len(c)))
					&& utf8_count < 4)
				c = GE_buffer_fgetc(io[i]), utf8_count--;

			*ptr = '\0';
			data.frq = 1;
			insert_or_count(list, data, FN_data_strcmp);
		}

		/* Add EOF char */
		memcpy(data.utf8_char, "EOF", 4), data.frq = 1;
		insert_or_count(list, data, FN_data_strcmp);

		rewind(io[i]->fp);
		GE_buffer_off(io[i]);
	}


	return list;
}

/*
 * extract_frequency_list: Compile a frequency list from the
 * table at the start of a compressed file.
 * TODO NOW the count being given seems to be wrong
 */
static unsigned extract_frequency_list(
							HC_HuffmanNode **list,
							F_Buf *buf,
							unsigned state)
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

	//while (is_set(state, LEX_MAP))
	while (is_set(state, LEX_MAP))
	{
		/* remove tab */
		c = GE_buffer_fgetc(buf);

		/* get char */
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
			state = LE_get_token(buf, c, state);
	}

	GE_string_free(str);

	return state;
}

/*
 * priority_queue_compression: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue.
 */
unsigned priority_queue_compression(
					HC_HuffmanNode **list,
					F_Buf **io,
					const unsigned state)
{
	if (is_set(state, VERBOSE))
		fwrite("Create priority queue.\n", 1, 23, stdout);

	/* Get char count */
	if (frequency_list_compression(list, io, state) == NULL)
		fprintf(stderr, "%s(): error frequency_list_compression failed.\n", __func__);

	/* Sort by frequency */
	if (DS_mergesort(list, FN_data_frqcmp) == NULL)
		fprintf(stderr, "%s(): error mergesort failed.\n", __func__);

	if (is_set(state, PRINT)) {
		fwrite("print frequency map.\n", 1, 22, stdout);
		print_frequency_map(*list);
	}

	return state;
}

/*
 * priority_queue_decompression: Retrieve the frequency map from the beginning
 * of a compressed file and make it into a list, sort the list into a priority
 * queue.
 */
unsigned priority_queue_decompression(
							HC_HuffmanNode **list,
							F_Buf *buf,
							unsigned state)
{
	if (is_set(state, VERBOSE))
		fwrite("Create priority queue.\n", 1, 23, stdout);

	/* Get char count */
	if ((state = extract_frequency_list(list, buf, state)) == 0)
		fprintf(stderr, "%s(): error extract_frequency_list failed.\n", __func__);

	/* Sort by frequency */
	if (DS_mergesort(list, FN_data_frqcmp) == NULL)
		fprintf(stderr, "%s(): error mergesort failed.\n", __func__);

	if (is_set(state, PRINT)) {
		fwrite("print frequency map.\n", 1, 22, stdout);
		print_frequency_map(*list);
	}

	return state;
}

