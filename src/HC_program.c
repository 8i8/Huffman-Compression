#include "HC_struct.h"
#include "HC_state.h"
#include "HC_error.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include "HC_priority_queue.h"
#include "HC_read_write.h"

extern int state;

/*
 * program: Compress input and write file out if specified.
 */
int program(Main *var)
{
	if (state & READ)
		create_priority_cue(&var->tree, var->in);

	if (state & (PRINT & READ))
		print_frequency(&var->tree);

	if (state & READ) {
		build_huffman_tree(&(var->tree));
		var->map = map_create(var->map, &var->tree);
		rewind(var->in);
	}

	if (state & (PRINT & READ)) {
		print_huffman_tree(var->tree);
		print_char_map(var->map);
	}

	if (state & (READ & WRITE))
		compress_file(var->map, var->in, var->out);

	if (state & DECOMP)
		;//decompress_file(var->map, var->in, var->out);

	if (state & ERROR) {
		HC_error_print();
		return 1;
	}

	return 0;
}

