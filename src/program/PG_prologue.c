#include "huffman/HC_huffman_node.h"
#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include <stdlib.h>
#include <ctype.h>

/*
 * prologue: Deal with all args at the program start and in consequence set the
 * programs initial state.
 */
unsigned prologue(int argc, char *argv[], F_Buf **io, unsigned state)
{
	FILE *fp;
	char c;
	int file;

	file = 0;

	while (--argc > 0) {
		if ((*++argv)[0] == '-') {
			while ((c = *++argv[0])) {
				switch (c)
				{
					case 'c':
						state_unset(state, DECOMPRESS);
						state_set(state, COMPRESS);
						break;
					case 'm':
						state_set(state, MONO);
						break;
					case 'p':
						state_set(state, PRINT);
						break;
					case 'v':
						state_set(state, VERBOSE);
						break;
					case 'x':
						state_unset(state, COMPRESS);
						state_set(state, DECOMPRESS);
						break;
					default :
						break;
				}
			}
			argv++;
		}

		/* Open a file with binary write enabled, to write the compressed data
		 * too */
		if (file < MAX_FILES && argc && is_set(state, COMPRESS) && file == 0) {
			if ((fp = fopen(*argv++, "wb")) == NULL) {
				fprintf(stderr, "file read error: %s\n", *argv);
				free(io[file]);
				return state_set(state, ERROR);
			} else if (file == MAX_FILES) {
				fwrite("file limit exceeded.", 1, 20, stderr);
				return state_set(state, ERROR);
			} else
				--argc, io[file++] = GE_buffer_init(fp);
		} 

		/* Open a file with text write enabled, to write the decompressed data
		 * too */
		if (file < MAX_FILES && argc && is_set(state, DECOMPRESS) && file == 0) {
			if ((fp = fopen(*argv++, "w")) == NULL) {
				fprintf(stderr, "file read error: %s\n", *argv);
				free(io[file]);
				return state_set(state, ERROR);
			} else if (file == MAX_FILES) {
				fwrite("file limit exceeded.", 1, 20, stderr);
				return state_set(state, ERROR);
			} else
				--argc, io[file++] = GE_buffer_init(fp);
		} 

		/* Open a readable file for every argument trailing the options given
		 * and the initial write file */
		if (file < MAX_FILES && argc && (fp = fopen(*argv, "r")) == NULL) {
			fprintf(stderr, "file read error: %s\n", *argv);
			return state_set(state, ERROR);
		} else if (file == MAX_FILES) {
			fwrite("file limit exceeded.", 1, 20, stderr);
			return state_set(state, ERROR);
		} else
			--argc, io[file++] = GE_buffer_init(fp);

	}

	return state;
}

