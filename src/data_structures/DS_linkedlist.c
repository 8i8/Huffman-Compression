#include <stdlib.h>
#include "general/GE_state.h"
#include "huffman/HC_huffman_node.h"
#include "huffman/HC_priority_queue.h"

/*
 * DS_linkedlist_new_node: Internal function for creating new list nodes.
 */
HC_HuffmanNode *DS_linkedlist_new_node(Data data)
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
 * DS_linkedlist_add: Create the next node in the list and add data.
 */
HC_HuffmanNode **DS_linkedlist_add(HC_HuffmanNode **list, Data data)
{
	if (*list == NULL)
		*list = DS_linkedlist_new_node(data);
	else {
		while ((*list)->next != NULL)
			list = &(*list)->next;

		if (((*list)->next = DS_linkedlist_new_node(data)) == NULL) {
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
 * DS_linkedlist_insert: Insert a new node at current location.
 */
HC_HuffmanNode **DS_linkedlist_insert(HC_HuffmanNode** list, Data data)
{
	HC_HuffmanNode *new;

	if (list == NULL || *list == NULL) {
		fprintf(stderr, "%s: NULL pointer.", __func__);
		return NULL;
	}

	if ((new = DS_linkedlist_new_node(data)) == NULL) {
		fprintf(stderr, "%s: ", __func__);
		return NULL;
	}

	new->next = *list;
	*list = new;

	return list;
}

/*
 * DS_linkedlist_insert_node: Insert a new node at current location.
 */
HC_HuffmanNode **DS_linkedlist_insert_node(
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
 * DS_linkedlist_insert_ordered: Insert a new node conditionaly.
 */
HC_HuffmanNode **DS_linkedlist_insert_ordered(
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
 * DS_linkedlist_pop: Pop the first node and return a pointer to the next node
 * of NULL if it does not exist.
 */
HC_HuffmanNode *DS_linkedlist_pop(HC_HuffmanNode **list)
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

