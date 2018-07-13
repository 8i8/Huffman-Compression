#include "HC_struct.h"
#include "HC_state.h"
#include "HC_huffman_tree.h"
#include "HC_map_char.h"
#include "HC_priority_queue.h"
#include "HC_read_write.h"
#include "HC_print.h"

extern int state;

Data **PG_map_init(void)
{
	return HC_map_init();
}

/*
 * program: Compress input and write file out if specified.
 */
Data **program(Files *io, HC_HuffmanNode **tree, Data **map)
{
	if (state & READ) {
		printf("Create priority queue.\n");
		create_priority_queue(tree, io->in);
	}

	if ((state & READ) && (state & PRINT)) {
		printf("print frequeancy map.\n");
		print_frequency_map(*tree);
	}

	if (state & READ) {
		printf("Build huffman tree.\n");
		build_huffman_tree(tree);
		rewind(io->in);
	}

	if ((state & READ) && (state & PRINT)) {
		printf("Print huffman tree.\n");
		print_huffman_tree(*tree);
	}

	if (state & READ) {
		printf("Make character hashmap.\n");
		map = map_create(map, tree);
	}

	if ((state & READ) && (state & PRINT)) {
		printf("Print char map.\n");
		print_char_map(map);
	}

	if ((state & READ) && (state & WRITE)) {
		printf("Compress file.\n");
		//compress_file(map, io->in, io->out);
	}

	if (state & DECOMP) {
		printf("Decompress file.\n");
		//decompress_file(tree, io->in, io->out);
	}

	if (state & ERROR) {
		fprintf(stderr, "%s: state error signaled.", __func__);
		return NULL;
	}
	
	printf("Free memory.\n");

	return map;
}

