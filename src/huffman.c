/*
 * 1) walk the file, count the characters, add one false EOF char, put in
 * priority cue.
 *
 * 2) Make Huffman tree: Pull out the two lowest count Huffman nodes from
 * priority cue, make the nodes into a binary tree with a new node as their
 * root, the new node has the sum of the two frequeuencies as its frequency.
 *
 * 3) Place that root back into the priority cue with the two Huffman nodes
 * as it children, put it into its correct position in the priority cue.
 *
 * 4) Walk the Huffman tree and list all the binary mappings for all of the
 * required char; This char map will replace the binary tree.
 *
 * 5) Encode the file.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "HC_Huffman_tree.h"
#include "HC_priority_queue.h"
#include "HC_char_map.h"
#include "HC_print_output.h"

int main(int argc, char *argv[])
{
	FILE *fp;
	HC_HuffmanNode*tree = NULL;
	Data *map = NULL;

	while (--argc)
	{
		if ((fp = fopen(*++argv, "r")) == NULL) {
			printf("Failed to open '%s'.\n", argv[argc+1]);
			return 1;
		}

		create_priority_cue(&tree, fp);
		print_frequeuency(&tree);
		build_huffman_tree(&tree);
		print_huffman_tree(tree);
		map = create_char_map(map, &tree);
		print_char_map(map);
		rewind(fp);

		HC_HuffmanTree_free(&tree);
		free(map);
	}

	return 0;
}

