#include <stdlib.h>
#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_hashtable.h"

/*
 * epilogue: Free all memory and close files.
 */
int epilogue(F_Buf **io, HC_HuffmanNode **tree)
{
	size_t i = 0;

	if (*tree)
		DS_huffman_tree_free(tree);

	for (i = 0; i < MAX_FILES && io[i] != NULL; i++)
		free(io[i]);

	free(io);

	return 0;
}
