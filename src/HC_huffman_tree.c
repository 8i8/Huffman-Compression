#include "HC_priority_queue.h"
#include "HC_mergesort.h"
#include "HC_func_comp.h"
#include "HC_print.h"
#include <stdlib.h>


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman tree
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * populate_map: Set all values in the Data struct to '\0'.
 */
static Data *data_init(Data *map)
{
	map->utf8_char[0] = '\0';
	map->string[0] = '\0';
	map->len = 0;
	map->frq = 0;
	map->next = NULL;
	return map;
}

/*
 * build_huffman_tree: Create a binary tree from the given linked list.
 *TODO NOW this function is not working correctly.
 */
HC_HuffmanNode **build_huffman_tree(HC_HuffmanNode **list)
{
	HC_HuffmanNode *new, *one, *two;
	Data data;
	data_init(&data);

	while ((*list)->next)
	{
		one = *list;
		*list = HC_priority_queue_pop(*list);
		two = *list;
		*list = HC_priority_queue_pop(*list);

		data.frq = one->data.frq + two->data.frq;

		/* Add leaves to new node and give a binary value */
		if ((new = HC_priority_queue_new_node(data)) == NULL)
			fprintf(stderr, "%s:", __func__);

		new->left = one, new->right = two;
		new->left->bit = '0', new->right->bit = '1';

		/* Insert new node into priority queue */
		//TODO NOW there is an issue in the return of this function,
		//the new node is getting lost.
		if (*list) {
			if (HC_priority_queue_insert_ordered(list, new, FN_data_frqcmp) == NULL)
				fprintf(stderr, "%s:", __func__);
		} else 
			*list = new;
	}

	return list;
}

/*
 * HC_huffman_tree_free: Free huffman tree memory.
 */
int HC_huffman_tree_free(HC_HuffmanNode **tree)
{
	if (*tree == NULL) {
		fprintf(stderr, "%s(): tree node is NULL.", __func__);
		return 1;
	}
	else if ((*tree)->next != NULL) {
		fprintf(stderr, "%s(): Priority queue nodes still exist.", __func__);
		return 1;
	}

	if ((*tree)->left)
		HC_huffman_tree_free(&(*tree)->left);
	if ((*tree)->right)
		HC_huffman_tree_free(&(*tree)->right);

	free(*tree);

	return 0;
}

