/*
 * TODO NEXT Close nicely on file open error. Also on file error for read in
 * compression.
 */
#include "general/GE_file_buffer.h"
#include "general/GE_state.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_huffman_tree.h"
#include "general/GE_hashtable.h"
#include "program/PG_prologue.h"
#include "program/PG_huffman.h"
#include "program/PG_epilogue.h"

int main(int argc, char *argv[])
{
	int st_prg;
	F_Buf **io = NULL;
	HC_HuffmanNode *tree = NULL;

	st_prg = state_init();
	io = GE_buffer_init_array();

	/* Get command line input and set program state */
	st_prg = prologue(argc, argv, io, st_prg);

	/* Run program code */
	st_prg = huffman(io, &tree, st_prg);

	/* Clean up */
	epilogue(io, &tree);

	return 0;
}

