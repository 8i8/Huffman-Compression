#include "HC_priority_queue.h"
#include "HC_mergesort.h"
#include "HC_comp_functions.h"
#include "HC_Error.h"
#include <stdlib.h>


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman tree
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * build_huffman_tree: Create a binary tree from the given linked list.
 */
HC_HuffmanNode **build_huffman_tree(HC_HuffmanNode **list)
{
	HC_HuffmanNode *new, *one, *two;
	Data data;
	data.num = 0, data.str[0] = '\0';

	while ((*list)->next)
	{
		one = *list;
		*list = HC_priority_queue_pop(*list);
		two = *list;
		*list = HC_priority_queue_pop(*list);

		data.num = one->data.num + two->data.num;
		data.str[0] = '\0';

		/* Add leaves to new node and give a binary value */
		if ((new = HC_priority_queue_new_node(data)) == NULL)
			HC_Error_print();
		new->left = one, new->right = two;
		new->left->bit = '0', new->right->bit = '1';

		/* Insert new node into priority cue */
		if (*list) {
			if (HC_priority_queue_insert_ordered(list, new, _comp_char) == NULL)
				HC_Error_print();
		} else 
			*list = new;
	}

	return list;
}

/*
 * HC_Huffman_tree_free: Free huffman tree memory.
 */
int HC_Huffman_tree_free(HC_HuffmanNode **tree)
{
	if ((*tree)->next != NULL) {
		HC_Error_set("%s(): Priority queue nodes still exist.");
		return 1;
	}

	if ((*tree)->left)
		HC_Huffman_tree_free(&(*tree)->left);
	if ((*tree)->right)
		HC_Huffman_tree_free(&(*tree)->right);

	free(*tree);

	return 0;
}

