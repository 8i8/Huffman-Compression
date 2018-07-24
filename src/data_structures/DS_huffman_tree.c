#include <stdlib.h>
#include "general/GE_state.h"
#include "data_structures/DS_huffman_node.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_print.h"

/*
 * DS_huffman_tree_new_node: Internal function for creating new list nodes.
 */
HC_HuffmanNode *DS_huffman_tree_new_node(Data data)
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
 * DS_huffman_tree_add: Create the next node in the list and add data.
 */
HC_HuffmanNode **DS_huffman_tree_add(HC_HuffmanNode **list, Data data)
{
	if (*list == NULL)
		*list = DS_huffman_tree_new_node(data);
	else {
		while ((*list)->next != NULL)
			list = &(*list)->next;

		if (((*list)->next = DS_huffman_tree_new_node(data)) == NULL) {
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
 * DS_huffman_tree_insert: Insert a new node at current location.
 */
HC_HuffmanNode **DS_huffman_tree_insert(HC_HuffmanNode** list, Data data)
{
	HC_HuffmanNode *new;

	if (list == NULL || *list == NULL) {
		fprintf(stderr, "%s: NULL pointer.", __func__);
		return NULL;
	}

	if ((new = DS_huffman_tree_new_node(data)) == NULL) {
		fprintf(stderr, "%s: ", __func__);
		return NULL;
	}

	new->next = *list;
	*list = new;

	return list;
}

/*
 * DS_huffman_tree_insert_node: Insert a new node at current location.
 */
HC_HuffmanNode **DS_huffman_tree_insert_node(
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
 * DS_huffman_tree_insert_ordered: Insert a new node conditionaly.
 */
HC_HuffmanNode **DS_huffman_tree_insert_ordered(
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
 * DS_huffman_tree_pop: Pop the first node and return a pointer to the next node
 * of NULL if it does not exist.
 */
HC_HuffmanNode *DS_huffman_tree_pop(HC_HuffmanNode **list)
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
 *  Huffman tree
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * build_huffman_tree: Create a binary tree from the given linkedlist.
 */
HC_HuffmanNode **build_huffman_tree(HC_HuffmanNode **tree, const unsigned state)
{
	HC_HuffmanNode *new, *one, *two;
	Data data;
	DS_huffman_data_init(&data);

	if (is_set(state, VERBOSE))
		printf("Build huffman tree.\n");

	while ((*tree)->next)
	{
		one = DS_huffman_tree_pop(tree);
		two = DS_huffman_tree_pop(tree);

		one->next = NULL, two->next = NULL;
		data.next = NULL;
		data.frq = one->data.frq + two->data.frq;

		/* Add leaves to new node and give a binary value */
		if ((new = DS_huffman_tree_new_node(data)) == NULL)
			fprintf(stderr, "%s:", __func__);

		new->left = one, new->right = two;
		new->left->bit = '0', new->right->bit = '1';

		/* Insert new node into priority queue */
		if (*tree) {
			if (DS_huffman_tree_insert_ordered(tree, new, FN_data_frqcmp) == NULL)
				fprintf(stderr, "%s:", __func__);
		} else 
			*tree = new;
	}

	if (is_set(state, PRINT)) {
		printf("Print huffman tree.\n");
		print_huffman_tree(*tree);
	}

	return tree;
}

/*
 * HC_huffman_tree_free: Free huffman tree memory.
 */
int HC_huffman_tree_free(HC_HuffmanNode **tree)
{
	if (*tree == NULL) {
		fprintf(stderr, "%s(): tree node is NULL.\n", __func__);
		return 1;
	}
	else if ((*tree)->next != NULL)
		fprintf(stderr, "%s(): Priority queue nodes still exist.\n", __func__);

	if ((*tree)->left)
		HC_huffman_tree_free(&(*tree)->left);
	if ((*tree)->right)
		HC_huffman_tree_free(&(*tree)->right);

	free(*tree);

	return 0;
}

