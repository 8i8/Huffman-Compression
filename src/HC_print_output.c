#include "HC_priority_queue.h"
#include "HC_char_map.h"

void _print_huffman_tree(HC_HuffmanNode *tree, int depth)
{
	int i;
	if (tree->left)
		_print_huffman_tree(tree->left, ++depth);

	for (i = 0; i < --depth; i++)
		putchar(' ');
	printf("%s\n", tree->data.str);

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

/*
 * print_frequeuency: Output the frequency map.
 */
void print_frequeuency(HC_HuffmanNode **list)
{
	while (*list != NULL) {
		printf("%s ", (*list)->data.str);
		printf("%lu\n", (*list)->data.num);
		list = &(*list)->next;
	}
}

/*
 * print_char_map: Print out the char map made by the Huffman tree.
 */
void print_char_map(Data *map)
{
	size_t i;
	for (i = 0; i < MAP_LEN; i++)
		if (map[i].binary[0] != '\0')
			printf("%s %s\n", map[i].str, map[i].binary);
}

