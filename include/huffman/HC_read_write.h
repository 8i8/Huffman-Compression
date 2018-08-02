#include <stdio.h>

/* encode_file: Write compressed file */
unsigned encode_file(Data **map, F_Buf **io, const unsigned state);

/* decode_file: Read and decompress compressed file */
unsigned decode_file(HC_HuffmanNode **list, F_Buf **io, unsigned state);

/* write_map_to_file: Write the frequency of each used characters repetition used
 * in the encoding of the file to the start of the file, so as to allow for the
 * recreation of the same Huffman tree for decompression */
void write_map_to_file(Data **map, F_Buf *buf);
