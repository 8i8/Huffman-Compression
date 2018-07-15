#include "HC_struct.h"
#include "GE_state.h"
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

/*
 * prologue: Deal with all args at the program start and in consequence set the
 * programs initial state.
 */
unsigned prologue(int argc, char *argv[], Files *io, unsigned state)
{
	char c;

	io->in = io->out = NULL;

	while ((c = getopt(argc, argv, "pvc:")) != -1)
		switch (c)
		{
			case 'c':
				if ((io->out = fopen(optarg, "wb")) == NULL) {
					fprintf(stderr, "file read error: %s\n", optarg);
					return state_set(state, ERROR);
				}
				state_set(state, COMPRESS);
				state_set(state, WRITE);
				break;
			case 'x':
				if ((io->out = fopen(optarg, "r")) == NULL) {
					fprintf(stderr, "file read error: %s\n", optarg);
					return state_set(state, ERROR);
				}
				state_set(state, DECOMPRESS);
				break;
			case 'p':
				state_set(state, PRINT);
				break;
			case 'v':
				state_set(state, VERBOSE);
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return state_set(state, ERROR);
			default :
				break;
		}

	if (optind != argc && ((io->in = fopen(argv[optind], "r")) == NULL)) {
		fprintf(stderr, "file read error: %s\n", argv[optind]);
		return state_set(state, ERROR);
	}

	if (io->in == NULL)
		state_set(state, COMPRESS);

	return state;
}

