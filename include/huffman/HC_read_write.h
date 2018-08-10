#include <stdio.h>

/*
 * metadata_write_file_name: Write file name in the archive meta data at the
 * given tab indenatation.
 */
int metadata_write_file_name(F_Buf *in, F_Buf *out, const int st_prg);

/*
 * compression_write_file: Write compressed file.
 */
int compression_write_file(
						Data map[],
						F_Buf *buf_read,
						F_Buf *buf_write,
						const int st_prg);

/*
 * decompress_archive: Read and decompress compressed file.
 */
int decompress_archive(F_Buf **io, const int st_prg);

/*
 * metadata_write_map: Write the frequency of each used characters repetition used
 * in the encoding of the file to the start of the file, so as to allow for the
 * recreation of the same Huffman tree for decompression.
 */
void metadata_write_map(Data map[], F_Buf *buf, const int st_prg);

/*
 * metadata_read_create_table: Create a hashmap from the char binary pairs in the files metadata.
 */
int metadata_read_create_table(F_Buf **io, Data *map, int st_lex, const int st_prg);

/*
 * metadata_read_filename: Read the filename from the archive metadata.
 */
String metadata_read_filename(F_Buf *buf, String str, int *st_lex, const int st_prg);

/*
 * decompress_write_text_file: Inflate a text file from the given arvhive.
 */
int decompress_write_text_file(
						F_Buf *buf_read,
						F_Buf *buf_write,
						Data *map,
						const int st_prg);
