#include <stdio.h>
#include "HC_struct.h"

/* compress_file: Write compressed file */
int compress_file(Data **map, FILE *in, FILE *out, const unsigned state);

/* decompress_file: Read and decompress compressed file */
unsigned decompress_file(HC_HuffmanNode **list, FILE *in, FILE *out, unsigned state);

/* write_frq_map: Write the frequency of each used characters repetition used
 * in the encoding of the file to the start of the file, so as to allow for the
 * recreation of the same Huffman tree for decompression */
void write_frq_map(Data **map, FILE *out);
