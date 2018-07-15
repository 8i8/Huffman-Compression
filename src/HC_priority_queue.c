#include "HC_mergesort.h"
#include "HC_struct.h"
#include "HC_huffman_tree.h"
#include <stdlib.h>
#include <string.h>
#include "HC_utf8.h"
#include "GE_state.h"
#include "HC_func_comp.h"
#include "HC_priority_queue.h"
#include "HC_print.h"

#define IN (1 << 0)

/*
 * HC_priority_queue_new_node: Internal function for creating new list nodes.
 */
HC_HuffmanNode *HC_priority_queue_new_node(Data data)
{
	HC_HuffmanNode *new_node = NULL;
	if ((new_node = malloc(sizeof(HC_HuffmanNode))) == NULL) {
		fprintf(stderr, "%s: memory allocation failed.", __func__);
		return NULL;
	}
	new_node->next = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->data = data;
	new_node->bit = '\0';

	return new_node;
}

/*
 * HC_priority_queue_add: Create the next node in the list and add data.
 */
HC_HuffmanNode **HC_priority_queue_add(HC_HuffmanNode **list, Data data)
{
	if (*list == NULL)
		*list = HC_priority_queue_new_node(data);
	else {
		while ((*list)->next != NULL)
			list = &(*list)->next;

		if (((*list)->next = HC_priority_queue_new_node(data)) == NULL) {
			fprintf(stderr, "%s: ", __func__);
			return NULL;
		}

		list = &(*list)->next;
		(*list)->data = data;
		(*list)->next = NULL;
	}

	return list;
}

/*
 * HC_priority_queue_insert: Insert a new node at current location.
 */
HC_HuffmanNode **HC_priority_queue_insert(HC_HuffmanNode** list, Data data)
{
	HC_HuffmanNode *new;

	if (list == NULL || *list == NULL) {
		fprintf(stderr, "%s: NULL pointer.", __func__);
		return NULL;
	}

	if ((new = HC_priority_queue_new_node(data)) == NULL) {
		fprintf(stderr, "%s: ", __func__);
		return NULL;
	}

	new->next = *list;
	*list = new;

	return list;
}

/*
 * HC_priority_queue_insert_node: Insert a new node at current location.
 */
HC_HuffmanNode **HC_priority_queue_insert_node(
						HC_HuffmanNode **list,
						HC_HuffmanNode *new_node)
{
	if (list == NULL || *list == NULL || new_node == NULL) {
		fprintf(stderr, "%s: NULL pointer.", __func__);
		return NULL;
	}

	new_node->next = *list;
	*list = new_node;

	return list;
}

/*
 * HC_priority_queue_insert_ordered: Insert a new node conditionaly.
 */
HC_HuffmanNode **HC_priority_queue_insert_ordered(
						HC_HuffmanNode **list,
						HC_HuffmanNode *new,
						int(*freq)(void*, void*))
{
	HC_HuffmanNode *start;
	start = *list;

	if (new == NULL) {
		fprintf(stderr, "%s: NULL pointer.", __func__);
		return NULL;
	}

	if (*list == NULL) {
		*list = new;
		return list;
	}

	if ((freq((void*)new, (void*)*list)) <= 0)
		start = new;
	else

		start = *list;

	while (*list && (*list)->next && (freq((void*)new, (void*)(*list)->next) > 0))
		list = &(*list)->next;

	new->next = (*list)->next;
	(*list)->next = new;
	list = &start;
	
	return list;
}

/*
 * HC_priority_queue_pop: Pop the first node and return a pointer to the next node
 * of NULL if it does not exist.
 */
HC_HuffmanNode *HC_priority_queue_pop(HC_HuffmanNode **list)
{
	HC_HuffmanNode *popped;

	if (list == NULL) {
		fprintf(stderr, "%s: Null pointer.", __func__);
		return NULL;
	}

	/* Remove the node */
	popped = *list;
	*list = (*list)->next;

	return popped;
}

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
		*list = HC_priority_queue_new_node(data);
		return list;
	}

	rtn = *list;

	while (*list != NULL)
	{
		if ((test = (*func)((void*)&data, (void*)&(*list)->data)) < 0) {
			if ((HC_priority_queue_insert(list, data)) == NULL) {
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
			HC_priority_queue_add(list, data);
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
static HC_HuffmanNode **compile_frequency_list(HC_HuffmanNode **list, FILE *fp)
{
	HC_HuffmanNode *start;
	start = *list;
	char c, *ptr;
	Data data;
	HC_data_init(&data);

	/* Scan document */
	while ((c = fgetc(fp)) != EOF)
	{
		ptr = data.utf8_char;

		/* Get multi-byte character */
		while ((*ptr++ = c) && utf8_countdown(c))
			c = fgetc(fp);

		*ptr = '\0';
		data.frq = 1;
		insert_or_count(list, data, FN_data_strcmp);
	}

	/* Add EOF char */
	memcpy(data.utf8_char, "EOF", 4), data.frq = 0;
	insert_or_count(list, data, FN_data_strcmp);

	list = &start;

	return list;
}

/*
 * compile_frequency_list_decomp: Compile a frequency list from the
 * table at the start of a compressed file.
 */
static HC_HuffmanNode **compile_frequency_list_decomp(
							HC_HuffmanNode **list,
							FILE *fp)
{
	HC_HuffmanNode *start; start = *list;
	char c, *ptr;
	Data data;

	/* Scan document */
	while ((c = fgetc(fp)) != EOF)
	{
		ptr = data.utf8_char;
		/* Get char for the length of the multi-byte character */
		while (utf8_countdown(c) && (*ptr++ = c))
			c = fgetc(fp);
		*ptr++ = c;
		*ptr = '\0';
		data.frq = 1;
		insert_or_count(list, data, FN_data_strcmp);
	}

	/* Add EOF char */
	memcpy(data.utf8_char, "EOF", 4), data.frq = 0;
	insert_or_count(list, data, FN_data_strcmp);

	list = &start;

	return list;
}

/*
 * create_priority_queue: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue.
 */
HC_HuffmanNode **create_priority_queue(
					HC_HuffmanNode **list,
					FILE *fp,
					const unsigned state)
{
	if (is_set(state, VERBOSE))
		printf("Create priority queue.\n");

	/* Count */
	if (compile_frequency_list(list, fp) == NULL)
		fprintf(stderr, "%s(): error compile_frequency_list failed.\n", __func__);

	/* Sort by frequency */
	if (HC_mergesort(list, FN_data_frqcmp) == NULL)
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
							FILE *fp)
{
	/* Get char count */
	list = compile_frequency_list_decomp(list, fp);

	/* Sort by frequency */
	list = HC_mergesort(list, FN_data_frqcmp);

	return list;
}

