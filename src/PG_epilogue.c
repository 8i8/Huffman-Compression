#include "HC_struct.h"
#include "HC_state.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include <stdlib.h>

/*
 * epilogue: Free all memory and close files.
 */
int epilogue(Files *io, HC_HuffmanNode **tree, Data **map, const int state)
{
	if (state & READ) {
		HC_huffman_tree_free(tree);
		free(map);
		fclose(io->in);
	}

	if (state & WRITE)
		fclose(io->out);

	if (state & ERROR)
		return 1;

	return 0;
}
