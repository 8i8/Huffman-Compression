#include "huffman/HC_huffman_node.h"
#include "huffman/HC_print.h"
#include "general/GE_hash.h"

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

	if (tree->data.utf8_char[0] != '\0')
		printf("\'%s\'\n", tree->data.utf8_char);
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
	while (tree->next) {
		tree = tree->next;
		printf("Error ~ ");
		print_huffman_tree_rec(tree->left, 0);
	}
	putchar('\n');
}

/*
 * print_char_map: Print out the char map made by the Huffman tree.
 */
void print_char_map(Data **map)
{
	size_t i;
	Data *cur;
	for (i = 0; i < MAP_LEN; i++)
		if (map[i]) {
			printf("%s %s\n", map[i]->utf8_char, map[i]->string);
			cur = map[i]->next;
			while (cur) {
				printf("%s %s\n", cur->utf8_char, cur->string);
				cur = map[i]->next;
			}
		}
}
