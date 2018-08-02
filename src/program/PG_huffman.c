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
#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "program/PG_prologue.h"
#include "data_structures/DS_huffman_tree.h"
#include "data_structures/DS_huffman_node.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_map_char.h"
#include "huffman/HC_read_write.h"
#include "program/PG_epilogue.h"

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
	if (is_set(state, COMPRESS) && !is_set(state, ESC)) {

		/* Make priority queue from the input file of character
		 * frequncy and then construct an ordered binary tree from that
		 * queue */
		priority_queue_compression(&tree, io, state);
		build_ordered_binary_tree(&tree, state);

		/* Create hash map to binary representation of char */
		map_create(map, &tree, state);

		write_map_to_file(map, io[0]);
		encode_file(map, io, state);
	}

	if (is_set(state, DECOMPRESS) && !is_set(state, ESC)) {
		state = decode_file(&tree, io, state);
	}

	if (is_set(state, ERROR))
		fprintf(stderr, "%s: state error signaled.", __func__);

	/* Clean up */
	epilogue(io, &tree, map, state);

	return 0;
}

