#include "HC_error.h"
#include "HC_mergesort.h"
#include <stdlib.h>
#include <string.h>
#include "HC_utf8.h"
#include "HC_func_comp.h"

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
	new_node->prev = NULL;
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

		(*list)->next->prev = (*list);
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
						HC_HuffmanNode *newList,
						int(*freq)(void*, void*))
{
	if (newList == NULL) {
		fprintf(stderr, "%s: NULL pointer.", __func__);
		return NULL;
	}

	if (*list == NULL) {
		*list = newList;
		return list;
	}
	
	
	while (*list && (*list)->next && (freq((void*)*list, (void*)newList) < 0))
		list = &(*list)->next;

	newList->next = *list;
	newList->prev = (*list)->prev;
	(*list)->prev = newList;
	*list = newList;

	return list;
}

/*
 * HC_priority_queue_pop: Pop the first node and return a pointer to the next node
 * of NULL if it does not exist.
 */
HC_HuffmanNode *HC_priority_queue_pop(HC_HuffmanNode *list)
{
	HC_HuffmanNode *popped;

	if (list == NULL) {
		fprintf(stderr, "%s: Null pointer.", __func__);
		return NULL;
	}

	/* Remove the node */
	popped = list;

	if (list->prev)
		list->prev->next = list->next;
	if (list->next)
		list->next->prev = list->prev;

	list = list->next;

	popped->next = NULL;
	popped->prev = NULL;

	return list;
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

	if (list == NULL) {
		HC_error_set("%s: NULL pointer.", __func__);
		return NULL;
	} else if (*list == NULL) {
		HC_error_set("%s: NULL pointer.", __func__);
		return NULL;
	}

	while (*list != NULL)
	{
		if ((test = (*func)((void*)&data, (void*)&(*list)->data)) < 0) {
			if ((HC_priority_queue_insert(list, data)) == NULL) {
				HC_error_append("%s: ", __func__);
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

	return list;
}

/*
 * compile_frequency_list: Sort alphabetically and keep count of the
 * occurrences of each character.
 */
static HC_HuffmanNode *compile_frequency_list(HC_HuffmanNode **list, FILE *fp)
{
	char c, *ptr;
	Data data;

	/* Scan document */
	while ((c = fgetc(fp)) != EOF)
	{
		ptr = data.utf8_char;

		/* Get multi-byte character */
		while ((*ptr++ = c) && utf8_count(c))
			c = fgetc(fp);

		*ptr = '\0';
		data.frq = 1;
		insert_or_count(list, data, FN_data_strcmp);
	}

	/* Add EOF char */
	memcpy(data.utf8_char, "EOF", 4), data.frq = 0;
	insert_or_count(list, data, FN_data_strcmp);

	return *list;
}

/*
 * compile_frequency_list_decomp: Compile a frequency list from the
 * table at the start of a compressed file.
 */
//TODO NOW 2a compile_frequency_list_decomp: needs a lexer.
static HC_HuffmanNode **compile_frequency_list_decomp(
							HC_HuffmanNode **list,
							FILE *fp)
{
	char c, *ptr;
	Data data;

	/* Scan document */
	while ((c = fgetc(fp)) != EOF)
	{
		ptr = data.utf8_char;
		/* Get char for the length of the multi-byte character */
		while (utf8_count(c) && (*ptr++ = c))
			c = fgetc(fp);
		*ptr++ = c;
		*ptr = '\0';
		data.frq = 1;
		insert_or_count(list, data, FN_data_strcmp);
	}

	/* Add EOF char */
	memcpy(data.utf8_char, "EOF", 4), data.frq = 0;
	insert_or_count(list, data, FN_data_strcmp);

	return list;
}

/*
 * create_priority_queue: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue.
 */
HC_HuffmanNode **create_priority_queue(HC_HuffmanNode **list, FILE *fp)
{
	/* Count */
	if ((*list = compile_frequency_list(list, fp)) == NULL)
		printf("%s(): error compile_frequency_list failed.\n", __func__);

	/* Sort by frequency */
	if ((list = HC_mergesort(list, FN_data_frqcmp)) == NULL)
		printf("%s(): error mergesort failed.\n", __func__);

	return list;
}

/*
 * build_priority_queue_from_file: Retrieve the frequency mapping from the
 * beginning of a compressed file and make it into a list, sort the list into a
 * priority queue.
 */
// TODO NOW 2 build_priority_queue_from_file. calls frequency list, and mergesort.
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

/*
 * print_frequency: Output the frequency map.
 */
void print_frequency(HC_HuffmanNode **list)
{
	while (*list != NULL) {
		printf("%s ", (*list)->data.utf8_char);
		printf("%lu\n", (*list)->data.frq);
		list = &(*list)->next;
	}
}

