#include "HC_struct.h"
#include "HC_state.h"
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

/*
 * prologue: Deal with all args at the program start and in consequence set the
 * programs initial state.
 */
//TODO NOW var is initalised here to NULL
int prologue(int argc, char *argv[], Main *var)
{
	char c;

	state_init();

	var->tree = NULL;
	var->map = NULL;
	var->in = var->out = NULL;

	while ((c = getopt(argc, argv, "pc:")) != -1)
		switch (c)
		{
			case 'c':
				if ((var->out = fopen(optarg, "wb")) == NULL) {
					fprintf(stderr, "file read error: %s\n", optarg);
					return state_set(ERROR);
				}
				state_set(WRITE);
				break;
			case 'x':
				if ((var->out = fopen(optarg, "r")) == NULL) {
					fprintf(stderr, "file read error: %s\n", optarg);
					return state_set(ERROR);
				}
				state_set(DECOMP);
				break;
			case 'p':
				state_set(PRINT);
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				return state_set(ERROR);
			default :
				break;
		}

	if (optind != argc && ((var->in = fopen(argv[optind], "r")) == NULL)) {
		printf("file read error: %s\n", argv[optind]);
		return state_set(ERROR);
	}

	if (var->in != NULL)
		state_set(READ);

	return 0;
}

