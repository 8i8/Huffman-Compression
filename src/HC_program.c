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
int program(Main *var)
{
	if (state & READ)
		create_priority_queue(&var->tree, var->in);

	if ((state & READ) && (state & PRINT))
		print_frequency(&var->tree);

	if (state & READ) {
		build_huffman_tree(&var->tree);
		var->map = map_create(var->map, &var->tree);
		rewind(var->in);
	}

	if ((state & READ) && (state & PRINT)) {
		print_huffman_tree(var->tree);
		print_char_map(var->map);
	}

	if ((state & READ) && (state & WRITE))
		compress_file(var->map, var->in, var->out);

	if (state & DECOMP)
		decompress_file(&var->tree, var->in, var->out);

	if (state & ERROR) {
		fprintf(stderr, "%s: state error signaled.", __func__);
		return 1;
	}

	return 0;
}

