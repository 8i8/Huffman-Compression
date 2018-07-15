#include "HC_priority_queue.h"
#include "HC_mergesort.h"
#include "HC_func_comp.h"
#include "HC_print.h"
#include "HC_state.h"
#include <stdlib.h>


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman tree
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * HC_data_init: Initalize an empty Data struct.
 */
Data *HC_data_init(Data *map)
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
 */
HC_HuffmanNode **build_huffman_tree(HC_HuffmanNode **tree, const unsigned state)
{
	HC_HuffmanNode *new, *one, *two;
	Data data;
	HC_data_init(&data);

	if (is_set(state, VERBOSE))
		printf("Build huffman tree.\n");

	while ((*tree)->next)
	{
		one = HC_priority_queue_pop(tree);
		two = HC_priority_queue_pop(tree);

		one->next = NULL, two->next = NULL;
		data.next = NULL;
		data.frq = one->data.frq + two->data.frq;

		/* Add leaves to new node and give a binary value */
		if ((new = HC_priority_queue_new_node(data)) == NULL)
			fprintf(stderr, "%s:", __func__);

		new->left = one, new->right = two;
		new->left->bit = '0', new->right->bit = '1';

		/* Insert new node into priority queue */
		if (*tree) {
			if (HC_priority_queue_insert_ordered(tree, new, FN_data_frqcmp) == NULL)
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

