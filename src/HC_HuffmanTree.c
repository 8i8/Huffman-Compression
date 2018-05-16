#include "HC_HuffmanTree.h"
#include "HC_Error.h"
#include <stdlib.h>
#include <string.h>

/*
 * HC_HuffmanTree_new_node: Internal function for creating new list nodes.
 */
HC_HuffmanTree *HC_HuffmanTree_new_node(Data data)
{
	HC_HuffmanTree *new_node = NULL;
	if ((new_node = malloc(sizeof(HC_HuffmanTree))) == NULL) {
		HC_Error_set("%s: memory allocation failed.", __func__);
		return NULL;
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->data = data;

	return new_node;
}

/*
 * HC_HuffmanTree_add: Create the next node in the list and add data.
 */
HC_HuffmanTree **HC_HuffmanTree_add(HC_HuffmanTree **list, Data data)
{
	if (*list == NULL)
		*list = HC_HuffmanTree_new_node(data);
	else {
		while ((*list)->next != NULL)
			list = &(*list)->next;

		if (((*list)->next = HC_HuffmanTree_new_node(data)) == NULL) {
			HC_Error_append("%s: ", __func__);
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
 * HC_HuffmanTree_insert: Insert a new node at current location.
 */
HC_HuffmanTree **HC_HuffmanTree_insert(HC_HuffmanTree **list, Data data)
{
	HC_HuffmanTree *new;

	if (list == NULL || *list == NULL) {
		HC_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	}
	if ((new = HC_HuffmanTree_new_node(data)) == NULL) {
		HC_Error_append("%s: ", __func__);
		return NULL;
	}

	new->next = *list;
	*list = new;

	return list;
}

/*
 * HC_HuffmanTree_insert_ordered: Insert a new node conditionaly.
 */
HC_HuffmanTree **HC_HuffmanTree_insert_ordered(HC_HuffmanTree **list, HC_HuffmanTree *newList,
						int(*func)(void*, void*))
{
	HC_HuffmanTree **ptr;
	ptr = list;

	if (newList == NULL) {
		HC_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	}

	if (list == NULL) {
		*list = newList;
		return list;
	}
	
	
	while (*list && (func((void*)*list, (void*)newList) < 0) && ((*list)->next != NULL))
		list = &(*list)->next;

	newList->next = *list;
	newList->prev = (*list)->prev;
	*list = newList;

	return ptr;
}

/*
 * HC_HuffmanTree_pop: Pop the first node and return a pointer to the new list
 * address.
 */
HC_HuffmanTree *HC_HuffmanTree_pop(HC_HuffmanTree *list)
{
	HC_HuffmanTree *popped;

	if (list == NULL) {
		HC_Error_set("%s: Null pointer.", __func__);
		return NULL;
	}

	/* Remove the node */
	popped = list;
	list = list->next;
	popped->next = NULL;
	popped->prev = NULL;

	return list;
}

int _huffman_tree_walk(HC_HuffmanTree **tree, int(*func)(void*, void*))
{
}

/*
 * HC_HuffmanTree_walk: Walk tree and perform (*func) on every node.
 */
int HC_HuffmanTree_walk(HC_HuffmanTree **tree, int(*func)(void*, void*))
{
}

