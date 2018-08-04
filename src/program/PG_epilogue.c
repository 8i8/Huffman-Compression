#include <stdlib.h>
#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "data_structures/DS_huffman_tree.h"
#include "data_structures/DS_huffman_node.h"
#include "huffman/HC_map_char.h"

/*
 * epilogue: Free all memory and close files.
 */
int epilogue(F_Buf **io, HC_HuffmanNode **tree, Data **map, const int state)
{
	size_t i = 0;

	if (*tree)
		DS_huffman_tree_free(tree);

	if (map)
		free(map);

	while (io[i] != NULL)
		free(io[i++]);

	free(io);

	if (state & ERROR)
		return 1;

	return 0;
}
