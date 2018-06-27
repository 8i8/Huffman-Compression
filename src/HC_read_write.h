#include <stdio.h>
#include "HC_struct.h"

/* compress_file: Write compressed file */
int compress_file(Data **map, FILE *in, FILE *out);

/* decompress_file: Read and decompress compressed file */
int decompress_file(HC_HuffmanNode **list, FILE *in, FILE *out);

