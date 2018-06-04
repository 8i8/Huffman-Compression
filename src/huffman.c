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
#include "HC_state.h"
#include "HC_prologue.h"
#include "HC_compression.h"
#include "HC_epilogue.h"

int main(int argc, char *argv[])
{
	Main v, *var;
	var = &v;

	prologue(argc, argv, var);
	compress(var);
	epilogue(var);

	return 0;
}

