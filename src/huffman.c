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
#include "HC_state.h"
#include "PG_prologue.h"
#include "PG_program.h"
#include "PG_epilogue.h"

int main(int argc, char *argv[])
{
	HC_HuffmanNode *tree = NULL;
	Data **map = NULL;
	Files files, *io;
	io = &files;

	prologue(argc, argv, io);
	program(io, &tree, map);
	epilogue(io, &tree, map);

	return 0;
}

