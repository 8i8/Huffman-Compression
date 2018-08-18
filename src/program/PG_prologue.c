#include <stdio.h>
#include "general/GE_state.h"
#include "general/GE_error.h"
#include "general/GE_file_buffer.h"

/*
 * prologue: Deal with all args at the program start and in consequence set the
 * programs initial state.
 */
int prologue(int argc, char *argv[], F_Buf **io, int st_prg)
{
	char c;
	int files = 0;

	while (--argc > 0) {
		if ((*++argv)[0] == '-') {
			while ((c = *++argv[0])) {
				switch (c)
				{
					case 'c':
						state_unset(st_prg, DECOMPRESS);
						state_set(st_prg, COMPRESS);
						break;
					case 'f':
						state_set(st_prg, FORCE);
						break;
					case 'm':
						state_set(st_prg, MONO);
						break;
					case 'p':
						state_set(st_prg, PRINT);
						break;
					case 'v':
						state_set(st_prg, VERBOSE);
						break;
					case 'x':
						state_unset(st_prg, COMPRESS);
						state_set(st_prg, DECOMPRESS);
						break;
					default :
						break;
				}
			}
			--argc, ++argv;

			if (!argc && is_set(st_prg, DECOMPRESS)) {
				fprintf(stdout, "Please provide an archive name and at least one file to compress.\n");
				return state_set(st_prg, ESC);
			} else if (!argc && is_set(st_prg, DECOMPRESS)) {
				fprintf(stdout, "Please provide an archive to decompress.\n");
				return state_set(st_prg, ESC);
			}

			/* Open the first file for writing as an archive */
			if (argc > 2 && is_set(st_prg, COMPRESS)) {
				if (GE_buffer_fopen_array(io, *argv, "wb", st_prg))
					return state_set(st_prg, ESC);
				else
					--argc, ++argv, ++files;
			} else if (is_set(st_prg, COMPRESS)) {
				fprintf(stdout, "Please provide an archive name along with the files to be compressed.\n");
				return state_set(st_prg, ESC);
			}
		}

		/* Open a readable file for every argument following the options given
		 * and the initial write file */
		if (argc && is_set(st_prg, COMPRESS)) {
			if (GE_buffer_fopen_array(io, *argv, "r", st_prg))
				return state_set(st_prg, ESC);
			else
				++files;
		}

		/* Open a file with text write enabled, to write the decompressed data
		 * too */
		if (argc && is_set(st_prg, DECOMPRESS)) {
			if (GE_buffer_fopen_array(io, *argv, "rb", st_prg))
				return state_set(st_prg, ESC);
			else
				++files;
		}
	}

	if (is_set(st_prg, DECOMPRESS) && files > 1) {
		fprintf(stdout, "Only one file expected for decompression operation.\n");
		return state_set(st_prg, ESC);
	}

	return st_prg;
}

