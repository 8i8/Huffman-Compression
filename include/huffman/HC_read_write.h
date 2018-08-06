#include <stdio.h>

/*
 * metadata_write_file_name: Write file name in the archive meta data at the
 * given tab indenatation.
 */
int metadata_write_file_name(F_Buf *buf);

/* archive_write_file: Write compressed file */
int archive_write_file(Data **map, F_Buf **io, const int st_prg);

/* decompress_archive: Read and decompress compressed file */
int decompress_archive(HC_HuffmanNode **list, F_Buf **io, const int st_prg);

/* metadata_write_map: Write the frequency of each used characters repetition used
 * in the encoding of the file to the start of the file, so as to allow for the
 * recreation of the same Huffman tree for decompression */
void metadata_write_map(Data **map, F_Buf *buf);
