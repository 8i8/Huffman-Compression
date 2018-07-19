#include "HC_struct.h"
#include "GE_state.h"
#include "GE_file_buffer.h"
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
						state_set(state, COMPRESS);
						break;
					case 'x':
						state_set(state, DECOMPRESS);
						break;
					case 'p':
						state_set(state, PRINT);
						break;
					case 'v':
						state_set(state, VERBOSE);
						break;
					default :
						break;
				}
			}
			argv++;
		}

		/* Open a file with binary write enabled, to write the compressed data
		 * too */
		if (argc && is_set(state, COMPRESS) && file == 0) {
			if ((fp = fopen(*argv++, "wb")) == NULL) {
				fprintf(stderr, "file read error: %s\n", *argv);
				free(io[file]);
				return state_set(state, ERROR);
			} else
				--argc, io[file++] = GE_buffer_init(fp);
		} 

		/* Open a readable file for every argument trailing the options given
		 * and the initial write file */
		if ((fp = fopen(*argv, "r")) == NULL) {
			fprintf(stderr, "file read error: %s\n", *argv);
			return state_set(state, ERROR);
		} else
			--argc, io[file++] = GE_buffer_init(fp);
	}

	return state;
}

