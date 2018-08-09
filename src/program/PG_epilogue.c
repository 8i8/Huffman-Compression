#include <stdlib.h>
#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_hashmap.h"

/*
 * epilogue: Free all memory and close files.
 */
int epilogue(F_Buf **io, HC_HuffmanNode **tree)
{
	size_t i = 0;

	if (*tree)
		DS_huffman_tree_free(tree);

	while (io[i] != NULL)
		free(io[i++]);

	free(io);

	return 0;
}
