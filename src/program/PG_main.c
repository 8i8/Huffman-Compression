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
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_hashmap.h"
#include "program/PG_prologue.h"
#include "program/PG_huffman.h"
#include "program/PG_epilogue.h"

int main(int argc, char *argv[])
{
	int st_prg;
	F_Buf **io = NULL;
	HC_HuffmanNode *tree = NULL;

	st_prg = state_init();
	io = GE_buffer_array_init();

	/* Get command line input and set program state */
	st_prg = prologue(argc, argv, io, st_prg);

	/* Run program code */
	st_prg = huffman(io, &tree, st_prg);

	/* Clean up */
	epilogue(io, &tree);

	return 0;
}

