/*
 * 1) walk the file, count the characters, add one false EOF char, put in
 * priority queue.
 *
 * 2) Make Huffman tree: Pull out the two lowest count Huffman nodes from
 * priority queue, make the nodes into a binary tree with a new node as their
 * root, the new node has the sum of the two frequeuencies as its frequency.
 *
 * 3) Place that root back into the priority queue with the two Huffman nodes
 * as it children, put it into its correct position in the priority queue.
 *
 * 4) Walk the Huffman tree and list all the binary mappings for all of the
 * required char; This char map will replace the binary tree.
 *
 * 5) Encode the file.
 *
 */
#include "HC_struct.h"
#include "PG_prologue.h"
#include "PG_epilogue.h"
#include "HC_state.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include "HC_priority_queue.h"
#include "HC_read_write.h"
#include "HC_print.h"

int main(int argc, char *argv[])
{
	HC_HuffmanNode *tree = NULL;
	Data **map = NULL;
	Files files, *io;
	io = &files;
	int state;

	state = state_init();
	map = HC_map_init();

	/* Get commandline input and set program state */
	state = prologue(argc, argv, io);

	/* Run program */
	if (state & READ) {
		printf("Create priority queue.\n");
		create_priority_queue(&tree, io->in);
	}

	if ((state & READ) && (state & PRINT)) {
		printf("print frequeancy map.\n");
		print_frequency_map(tree);
	}

	if (state & READ) {
		printf("Build huffman tree.\n");
		build_huffman_tree(&tree);
		rewind(io->in);
	}

	if ((state & READ) && (state & PRINT)) {
		printf("Print huffman tree.\n");
		print_huffman_tree(tree);
	}

	if (state & READ) {
		printf("Make character hashmap.\n");
		map_create(map, &tree);
	}

	if ((state & READ) && (state & PRINT)) {
		printf("Print char map.\n");
		print_char_map(map);
	}

	if ((state & READ) && (state & WRITE)) {
		printf("Compress file.\n");
		write_frq_map(map, io->out);
		compress_file(map, io->in, io->out);
	}

	if (state & DECOMP) {
		printf("Decompress file.\n");
		decompress_file(&tree, io->in, io->out);
	}

	if (state & ERROR) {
		fprintf(stderr, "%s: state error signaled.", __func__);
		return NULL;
	}
	
	/* Clean up */
	printf("Free memory.\n");
	epilogue(io, &tree, map, state);

	return 0;
}

