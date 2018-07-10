#include "HC_struct.h"
#include "HC_print.h"

/*
 * print_node: Print one node.
 */
static void print_node(HC_HuffmanNode *node)
{
	printf("%s ", node->data.utf8_char);
	if (node->data.len)
		printf("%s ", node->data.string);
	printf("%lu\n", node->data.frq);

}

/*
 * print_tree: Recursivly print a huffman tree.
 */
static void print_tree(HC_HuffmanNode *tree, int depth)
{
	int d, i;
	d = depth;

	if (tree != NULL)
	{
		if (tree->left)
			print_tree(tree->left, ++depth);

		for (i = 0; i < --d; i++)
			putchar(' ');

		print_node(tree);
		depth--;

		if (tree->right)
			print_tree(tree->right, ++depth);
	}
}

/*
 * print_frequency_map: Output the frequency linkedlist, if any treenodes exist
 * print those also, includes a verification for nodes joint by the left right
 * tree connections.
 */
void print_frequency_map(HC_HuffmanNode *list)
{
	while (list != NULL)
	{
		if (list->left != NULL)
			print_tree(list->left, 1);

		print_node(list);

		if (list->right != NULL)
			print_tree(list->right, 1);

		list = list->next;
	}
}

static void print_huffman_tree_rec(HC_HuffmanNode *tree, int depth)
{
	int i, d;
	d = depth;

	if (tree->left)
		print_huffman_tree_rec(tree->left, ++depth);

	for (i = 0; i < --d; i++)
		putchar(' ');

	printf("%s\n", tree->data.utf8_char);
	depth--;

	if (tree->right)
		print_huffman_tree_rec(tree->right, ++depth);
}

/*
 * print_huffman_tree: Print out huffman tree.
 */
void print_huffman_tree(HC_HuffmanNode *tree)
{
	print_huffman_tree_rec(tree, 0);
	putchar('\n');
}

