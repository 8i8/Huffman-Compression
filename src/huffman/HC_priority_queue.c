#include <stdlib.h>
#include <string.h>
#include "general/GE_state.h"
#include "data_structures/DS_mergesort.h"
#include "huffman/HC_huffman_node.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_utf8.h"
#include "huffman/HC_func_comp.h"
#include "huffman/HC_linkedlist.h"
#include "huffman/HC_print.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Priority queue
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

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
		*list = DS_linkedlist_new_node(data);
		return list;
	}

	rtn = *list;

	while (*list != NULL)
	{
		if ((test = (*func)((void*)&data, (void*)&(*list)->data)) < 0) {
			if ((DS_linkedlist_insert(list, data)) == NULL) {
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
			DS_linkedlist_add(list, data);
			break;
		}
	}

	list = &rtn;

	return list;
}

/*
 * compile_frequency_list: Sort alphabetically and keep count of the
 * occurrences of each character.
 */
static HC_HuffmanNode **compile_frequency_list(
						HC_HuffmanNode **list,
						F_Buf **io,
						const unsigned state)
{
	char c, *ptr;
	size_t i;
	Data data;
	HC_data_init(&data);

	for (i = is_set(state, COMPRESS); i < MAX_FILES && io[i]; i++) {
		GE_buffer_on(io[i]);
		while ((c = GE_buffer_fgetc(io[i])) != EOF)
		{
			ptr = data.utf8_char;

			/* Add char and check for multi-byte character */
			while ((*ptr++ = c) && utf8_countdown(c))
				c = GE_buffer_fgetc(io[i]);

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
 * compile_frequency_list_decomp: Compile a frequency list from the
 * table at the start of a compressed file.
 * TODO this function has not been tested.
 */
static HC_HuffmanNode **compile_frequency_list_decomp(
							HC_HuffmanNode **list,
							F_Buf **io)
{
	Data data;
	char c, *ptr;
	size_t i;

	/* Scan document */
	for (i = 0; i < MAX_FILES && io[i]; i++) {
		GE_buffer_on(io[i]);
		while ((c = GE_buffer_fgetc(io[i])) != EOF)
		{
			ptr = data.utf8_char;
			/* Get char for the length of the multi-byte character */
			while ((*ptr++ = c) && utf8_countdown(c))
				c = GE_buffer_fgetc(io[i]);
			*ptr++ = c;
			*ptr = '\0';
			data.frq = 1;
			insert_or_count(list, data, FN_data_strcmp);
		}

		/* Add EOF char */
		memcpy(data.utf8_char, "EOF", 4), data.frq = 0;
		insert_or_count(list, data, FN_data_strcmp);

		GE_buffer_rewind(io[i]);
		GE_buffer_off(io[i]);
	}

	return list;
}

/*
 * create_priority_queue: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue.
 */
HC_HuffmanNode **create_priority_queue(
					HC_HuffmanNode **list,
					F_Buf **buf,
					const unsigned state)
{
	if (is_set(state, VERBOSE))
		printf("Create priority queue.\n");

	/* Count */
	if (compile_frequency_list(list, buf, state) == NULL)
		fprintf(stderr, "%s(): error compile_frequency_list failed.\n", __func__);

	/* Sort by frequency */
	if (DS_mergesort(list, FN_data_frqcmp) == NULL)
		fprintf(stderr, "%s(): error mergesort failed.\n", __func__);

	if (is_set(state, PRINT)) {
		printf("print frequeancy map.\n");
		print_frequency_map(*list);
	}

	return list;
}

/*
 * build_priority_queue_from_file: Retrieve the frequency mapping from the
 * beginning of a compressed file and make it into a list, sort the list into a
 * priority queue.
 */
HC_HuffmanNode **build_priority_queue_from_file(
							HC_HuffmanNode **list,
							F_Buf **io)
{
	/* Get char count */
	list = compile_frequency_list_decomp(list, io);

	/* Sort by frequency */
	list = DS_mergesort(list, FN_data_frqcmp);

	return list;
}

