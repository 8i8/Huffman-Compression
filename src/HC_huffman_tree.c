#include "HC_priority_queue.h"
#include "HC_mergesort.h"
#include "HC_func_comp.h"
#include "HC_error.h"
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
	data.frq = 0, data.utf8_char[0] = '\0';

	while ((*list)->next)
	{
		one = *list;
		*list = HC_priority_queue_pop(*list);
		two = *list;
		*list = HC_priority_queue_pop(*list);

		data.frq = one->data.frq + two->data.frq;
		data.utf8_char[0] = '\0';

		/* Add leaves to new node and give a binary value */
		if ((new = HC_priority_queue_new_node(data)) == NULL)
			HC_error_print();
		new->left = one, new->right = two;
		new->left->bit = '0', new->right->bit = '1';

		/* Insert new node into priority queue */
		if (*list) {
			if (HC_priority_queue_insert_ordered(list, new, FN_data_frqcmp) == NULL)
				HC_error_print();
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
	if ((*tree)->next != NULL) {
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

static void _print_huffman_tree(HC_HuffmanNode *tree, int depth)
{
	int i;
	if (tree->left)
		_print_huffman_tree(tree->left, ++depth);

	for (i = 0; i < --depth; i++)
		putchar(' ');

	printf("%s\n", tree->data.utf8_char);

	if (tree->right)
		_print_huffman_tree(tree->right, ++depth);
}

/*
 * print_huffman_tree: Print out huffman tree.
 */
void print_huffman_tree(HC_HuffmanNode *tree)
{
	_print_huffman_tree(tree, 0);
	putchar('\n');
}

