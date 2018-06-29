#include "HC_struct.h"
#include "HC_state.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include "HC_priority_queue.h"
#include "HC_read_write.h"

extern int state;

/*
 * program: Compress input and write file out if specified.
 */
//TODO how is the functioning of the var->tree* working here?
HC_HuffmanNode **program(Files *io, HC_HuffmanNode **tree, Data **map)
{
	if (state & READ)
		create_priority_queue(tree, io->in);

	if ((state & READ) && (state & PRINT))
		print_frequency(*tree);

	if (state & READ) {
		build_huffman_tree(tree);
		map = map_create(map, tree);
		rewind(io->in);
	}

	if ((state & READ) && (state & PRINT)) {
		print_huffman_tree(*tree);
		print_char_map(map);
	}

	if ((state & READ) && (state & WRITE))
		compress_file(map, io->in, io->out);

	if (state & DECOMP)
		decompress_file(tree, io->in, io->out);

	if (state & ERROR) {
		fprintf(stderr, "%s: state error signaled.", __func__);
		return NULL;
	}

	return tree;
}

