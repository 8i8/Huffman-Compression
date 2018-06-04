#include "HC_struct.h"
#include "HC_state.h"
#include "HC_huffman_tree.h"
#include <stdlib.h>

extern unsigned state;

/*
 * epilogue: Free all memory and close files.
 */
int epilogue(Main *var)
{
	if (state & READ) {
		HC_huffman_tree_free(&var->tree);
		free(var->map);
		fclose(var->in);
	}

	if (state & WRITE)
		fclose(var->out);

	if (state & ERROR)
		return 1;

	return 0;
}
