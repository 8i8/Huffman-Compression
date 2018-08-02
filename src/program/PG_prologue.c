#include "general/GE_state.h"
#include "general/GE_file_buffer.h"

/*
 * prologue: Deal with all args at the program start and in consequence set the
 * programs initial state.
 */
unsigned prologue(int argc, char *argv[], F_Buf **io, unsigned state)
{
	char c;

	while (--argc > 0) {
		if ((*++argv)[0] == '-') {
			while ((c = *++argv[0])) {
				switch (c)
				{
					case 'c':
						state_unset(state, DECOMPRESS);
						state_set(state, COMPRESS);
						break;
					case 'f':
						state_set(state, FORCE);
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
			--argc, ++argv;
		}

		/* Open a file for writing an archive */
		if (argc && is_set(state, COMPRESS)) {
			if (GE_open_file(*argv, io, "wb", state))
				return state_set(state, ESC);
			else
				--argc, ++argv;
		}

		/* Open a readable file for every argument following the options given
		 * and the initial write file */
		if (argc && is_set(state, COMPRESS)) {
			if (GE_open_file(*argv, io, "r", state))
				return state_set(state, ESC);
			else
				--argc, ++argv;
		}

		/* Open a file with text write enabled, to write the decompressed data
		 * too */
		if (argc && is_set(state, DECOMPRESS)) {
			if (GE_open_file(*argv, io, "rb", state))
				return state_set(state, ESC);
			else
				--argc, ++argv;
		}
	}

	return state;
}

