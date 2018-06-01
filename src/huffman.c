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
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "HC_Huffman_tree.h"
#include "HC_priority_queue.h"
#include "HC_char_map.h"
#include "HC_print_output.h"

short COMPRESS = 0;
short READ = 0;

int main(int argc, char *argv[])
{
	FILE *in, *out;
	char c;
	HC_HuffmanNode *tree = NULL;
	Data *map = NULL;

	while ((c = getopt(argc, argv, "c:")) != -1)
		switch (c)
		{
			case 'c':
				if ((out = fopen(optarg, "w")) == NULL) {
					printf("file read error: %s\n", optarg);
					return 1;
				}
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return 1;
		}

	if ((in = fopen(argv[optind], "r")) == NULL && COMPRESS++) {
		printf("file read error: %s\n", argv[optind]);
		return 1;
	}

	if (COMPRESS) {
		create_priority_cue(&tree, in);
		print_frequeuency(&tree);
		build_huffman_tree(&tree);
		print_huffman_tree(tree);
		map = create_char_map(map, &tree);
		print_char_map(map);
		rewind(in);
	}

	HC_Huffman_tree_free(&tree);
	free(map);

	return 0;
}

