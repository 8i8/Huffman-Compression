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
#include "GE_state.h"
#include "PG_prologue.h"
#include "HC_priority_queue.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include "HC_read_write.h"
#include "HC_print.h"
#include "PG_epilogue.h"

int main(int argc, char *argv[])
{
	HC_HuffmanNode *tree = NULL;
	Data **map = NULL;
	Files files, *io;
	io = &files;
	unsigned state;

	state = state_init();
	map = HC_map_init();

	/* Get commandline input and set program state */
	state = prologue(argc, argv, io, state);

	/* Run program */
	if (is_set(state, COMPRESS)) {
		create_priority_queue(&tree, io->in, state);
		build_huffman_tree(&tree, state);
		rewind(io->in);
		map_create(map, &tree, state);
	}

	if ((is_set(state, COMPRESS)) && (is_set(state, WRITE))) {
		write_map_to_file(map, io->out);
		compress_file(map, io->in, io->out, state);
	}

	if (is_set(state, DECOMPRESS))
		decompress_file(&tree, io->in, io->out, state);

	if (is_set(state, ERROR))
		fprintf(stderr, "%s: state error signaled.", __func__);

	/* Clean up */
	epilogue(io, &tree, map, state);

	return 0;
}

