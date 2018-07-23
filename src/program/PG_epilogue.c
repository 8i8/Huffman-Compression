#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "huffman/HC_huffman_node.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_map_char.h"
#include <stdlib.h>

/*
 * epilogue: Free all memory and close files.
 */
int epilogue(F_Buf **io, HC_HuffmanNode **tree, Data **map, const unsigned state)
{
	size_t i = 0;

	HC_huffman_tree_free(tree);
	free(map);

	while (io[i] != NULL)
		free(io[i++]);

	free(io);

	if (state & ERROR)
		return 1;

	return 0;
}