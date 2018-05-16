/*
 * 1) walk the file, count the characters, add one false EOF char, put in
 * priority cue.
 *
 * 2) Make Huffman tree: Pull out the two lowest count Huffman nodes from
 * priority cue, make the nodes into a binary tree with a new node as their
 * root, the new node has the sum of the two frequencies as its frequency.
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
#include "HC_frequency_map.h"

int main(int argc, char *argv[])
{
	FILE *fp;
	HC_HuffmanTree *list = NULL;
	HC_CharMap *map = NULL;

	while (--argc)
	{
		if ((fp = fopen(*++argv, "r")) == NULL)
			printf("Can not open '%s'.\n", argv[argc+1]);

		/* Make priority cue of Huffman nodes */
		create_priority_cue(&list, fp);
		print_frequency(&list);
		build_huffman_tree(&list);
		create_char_map(&map, &list);

		//HC_HuffmanTree_clear(&list);
	}

	return 0;
}

