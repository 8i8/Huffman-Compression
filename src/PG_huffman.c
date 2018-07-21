/*
 * 1) walk the file, count the characters, add one false EOF char, put in
 * priority queue.
 *
 * 2) Make Huffman tree: Pull out the two lowest count Huffman nodes from
 * priority queue, make the nodes into a binary tree with a new node as their
 * root, the new node has the sum of the two frequencies as its frequency.
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
#include "GE_file_buffer.h"
#include "PG_prologue.h"
#include "HC_priority_queue.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include "HC_read_write.h"
#include "PG_epilogue.h"

int main(int argc, char *argv[])
{
	HC_HuffmanNode *tree = NULL;
	Data **map = NULL;
	F_Buf **io = NULL;
	unsigned state;

	state = state_init();
	map = HC_map_init();
	io = GE_buffer_array_init();

	/* Get command line input and set program state */
	state = prologue(argc, argv, io, state);

	/* If required, write compressed data */
	if (is_set(state, COMPRESS)) {
		/* Create huffman tree, if COMPRESS is set start at 1 else start at 0,
		 * the first argument is used as the file output when required */
		create_priority_queue(&tree, io, state);
		build_huffman_tree(&tree, state);

		/* Create hash map to binary representation of char */
		map_create(map, &tree, state);

		write_map_to_file(map, io[0]->fp);
		compress_file(map, io, state);
	}

	if (is_set(state, DECOMPRESS)) {
		decompress_file(&tree, io, state);
	}

	if (is_set(state, ERROR))
		fprintf(stderr, "%s: state error signaled.", __func__);

	/* Clean up */
	epilogue(io, &tree, map, state);

	return 0;
}

