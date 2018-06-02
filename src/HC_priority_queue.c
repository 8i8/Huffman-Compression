#include "HC_error.h"
#include "HC_mergesort.h"
#include <stdlib.h>
#include <string.h>
#include "HC_utf8.h"
#include "HC_func_comp.h"

/*
 * HC_priority_queue_new_node: Internal function for creating new list nodes.
 */
HC_HuffmanNode *HC_priority_queue_new_node(Data data)
{
	HC_HuffmanNode *new_node = NULL;
	if ((new_node = malloc(sizeof(HC_HuffmanNode))) == NULL) {
		HC_error_set("%s: memory allocation failed.", __func__);
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
			HC_error_append("%s: ", __func__);
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
		HC_error_set("%s: NULL pointer.", __func__);
		return NULL;
	}
	if ((new = HC_priority_queue_new_node(data)) == NULL) {
		HC_error_append("%s: ", __func__);
		return NULL;
	}

	new->next = *list;
	*list = new;

	return list;
}

/*
 * HC_priority_queue_insert_node: Insert a new node at current location.
 */
HC_HuffmanNode **HC_priority_queue_insert_node(HC_HuffmanNode **list, HC_HuffmanNode *new_node)
{
	if (list == NULL || *list == NULL || new_node == NULL) {
		HC_error_set("%s: NULL pointer.", __func__);
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
		HC_error_set("%s: NULL pointer.", __func__);
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
		HC_error_set("%s: Null pointer.", __func__);
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
 *  Priority cue
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * _insert_or_count: If the char does not yet exist in the char map,
 * insert it in alphabetical order. If it exists already, add one to the count
 * for that character.
 */
HC_HuffmanNode **_insert_or_count(HC_HuffmanNode **list, Data data,
						int(*func)(void*, void*))
{
	int test;

	if (list == NULL) {
		HC_error_set("%s: NULL pointer.", __func__);
		return NULL;
	} else if (*list == NULL) {
		list = HC_priority_queue_add(list, data);
		return list;
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
			(*list)->data.num++;
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
 * _compile_frequeuency_list: Sort alphabetically and count occurrences of each character in
 * the document.
 */
HC_HuffmanNode **_compile_frequeuency_list(HC_HuffmanNode **list, FILE *fp)
{
	char c, *ptr;
	Data data;

	/* Scan document */
	while ((c = fgetc(fp)) != EOF)
	{
		ptr = data.str;
		/* Get char for the lenght of the multi-byte character */
		while (utf8_test_count(c) && (*ptr++ = c))
			c = fgetc(fp);
		*ptr++ = c;
		*ptr = '\0';
		data.num = 1;
		_insert_or_count(list, data, _comp_char);
	}

	/* Add EOF char */
	memcpy(data.str, "EOF", 4), data.num = 0;
	_insert_or_count(list, data, _comp_char);

	return list;
}

/*
 * create_priority_cue: Call the ListSort function and set the sort parameter.
 */
HC_HuffmanNode **create_priority_cue(HC_HuffmanNode **list, FILE *fp)
{
	/* Count */
	list = _compile_frequeuency_list(list, fp);

	/* Sort by frequeuency */
	list = HC_mergesort(list, _comp_freq);

	return list;
}

