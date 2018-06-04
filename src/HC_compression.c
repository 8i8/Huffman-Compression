#include "HC_struct.h"
#include "HC_state.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include "HC_read_write.h"
#include "HC_print.h"

extern int state;

/*
 * compress: Compress input and write file out if specified.
 */
int compress(Main *var)
{
	if (state & READ)
		create_priority_cue(&var->tree, var->in);

	if (state & PRINT)
		print_frequency(&var->tree);

	if (state & READ) {
		build_huffman_tree(&(var->tree));
		var->map = map_create(var->map, &var->tree);
		rewind(var->in);
	}

	if (state & PRINT) {
		print_huffman_tree(var->tree);
		print_char_map(var->map);
	}

	if (state & (READ | WRITE))
		compress_file(var->map, var->in, var->out);

	if (state & DECOMP)
		decompress_file(var->map, var->in, var->out);

	if (state & ERROR)
		return 1;

	return 0;
}

