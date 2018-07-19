//#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HC_struct.h"
#include "GE_state.h"
#include "HC_map_char.h"
#include "HC_utf8.h"
#include "HC_priority_queue.h"
#include "LE_lexer.h"
#include "GE_hash.h"
#include "GE_string.h"
#include "GE_file_buffer.h"

/*
 * write_map_to_file: Write the frequency of each used characters repetition
 * used in the encoding of the file to the start of the file, so as to allow
 * for the recreation of the same Huffman tree for decompression.
 */
void write_map_to_file(Data **map, FILE *out)
{
	F_Buf *buf = NULL;
	char *num, *pt_num;
	size_t i;
	Data *cur;

	pt_num = num = malloc(256);
	buf = GE_buffer_init(out);
	GE_buffer_on(buf);
	GE_buffer_fwrite("<map>\n", 1, 6, buf);

	for (i = 0; i < MAP_LEN; i++) {
		if (map[i] != NULL) {
			GE_buffer_fwrite("\t", 1, 1, buf);
			GE_buffer_fwrite(map[i]->utf8_char, 1,
					strlen(map[i]->utf8_char), buf);
			GE_buffer_fwrite(" ~ ", 1, 3, buf);
			sprintf(num, "%lu", map[i]->frq);
			GE_buffer_fwrite(num, 1, strlen(num), buf);
			GE_buffer_fwrite("\n", 1, 1, buf);
			num = pt_num;
			if ((cur = map[i]->next)) {
				while ((cur = cur->next)) {
					GE_buffer_fwrite("\t", 1, 1, buf);
					GE_buffer_fwrite(map[i]->utf8_char, 1,
							strlen(map[i]->utf8_char), buf);
					GE_buffer_fwrite(" ~ ", 1, 3, buf);
					sprintf(num, "%lu", map[i]->frq);
					GE_buffer_fwrite(num, 1, strlen(num), buf);
					GE_buffer_fwrite("\n", 1, 1, buf);
					num = pt_num;
				}
			}
		}
	}
	free(pt_num);

	GE_buffer_fwrite("</map>\n", 1, 7, buf);
	GE_buffer_empty(buf);
	GE_buffer_off(buf);
	GE_buffer_free(buf);
}

/*
 * write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 * to.
 */
static void write_bit(
				F_Buf *buf,
				char bit,
				char *byte,
				unsigned *bit_count)
{
	if (++(*bit_count) == 8) {
		GE_buffer_fwrite(byte, 1, 1, buf);
		*bit_count = 0;
		*byte = 0;
	}

	/* Shift left ready for the next bit */
	*byte <<= 1;
	/* Set bit to 1 or 0 */
	*byte |= bit;
}

/*
 * compress_file: Write compressed file.
 */
unsigned compress_file(Data **map, F_Buf **io, const unsigned state)
{
	unsigned bit_count, utf8_count;
	char byte;

	char c, *ptr, utf8_char[5], *bin;
	size_t len, i, j;
	ptr = utf8_char;

	bit_count = byte = utf8_count = 0;

	GE_buffer_on(io[0]);
	GE_buffer_fwrite("<comp>\n", 1, 8, io[0]);

	for (i = is_set(state, COMPRESS); i < MAX_FILES && io[i]; i++) {
		GE_buffer_on(io[i]);
		while ((c = GE_buffer_fgetc(io[i])) != EOF && utf8_count < 4)
		{
			/* Get char for the lenght of what is possibly a multi-byte
			 * character */
			while ((*ptr++ = c) && (utf8_count = utf8_countdown(c)) && utf8_count < 4)
				c = GE_buffer_fgetc(io[i]);

			*ptr = '\0';

			bin = map_read_char_to_binary(map, utf8_char);
			len = map_read_char_to_binary_len(map, utf8_char);
			for (j = 0; j < len; j++, bin++)
				write_bit(io[0], bin[0], &byte, &bit_count);

			ptr = utf8_char;
		}

		GE_buffer_rewind(io[i]);
		GE_buffer_off(io[i]);

		if (utf8_count > 4)
			fprintf(stderr, "%s: utf8_countdown error.\n", __func__);
	}

	/* Add EOF char */
	bin = map_read_char_to_binary(map, "EOF");
	len = map_read_char_to_binary_len(map, utf8_char);
	for (i = 0; i < len; i++, bin++)
		write_bit(io[0], bin[0], &byte, &bit_count);

	/* Pad any remaining bits in the last byte with zeroes */
	if(bit_count > 0) {
		 byte <<= 8 - bit_count;
		 GE_buffer_fwrite(&byte, 1, 1, io[0]);
	}

	GE_buffer_fwrite("\n</comp>\n", 1, 9, io[0]);
	GE_buffer_empty(io[0]);
	GE_buffer_off(io[0]);

	if (is_set(state, VERBOSE))
		printf("Compressed file writen.\n");

	return state;
}

/*
 * read_compressed_file: Read the compressed file and write the decompressed
 * data to the output file.
 */
FILE *read_compressed_file(FILE *in, FILE *out)
{
	in = out;

	return in;
}

/*
 * decompress_file: Read and then decompress compressed file. Analyze file
 * stream with lexer to decompress the file.
 * TODO NEXT
 */
unsigned decompress_file(HC_HuffmanNode **list, F_Buf **io, unsigned state)
{
	LE_lexer_init();
	char c;
	size_t i;

	if (is_set(state, VERBOSE))
		printf("Decompress file.\n");

	for (i = is_set(state, DECOMPRESS); i < MAX_FILES && io[i]; i++) {
		while (io[i] && (c = fgetc(io[i]->fp)) != EOF)
		{
			/* Read frequency map from file start */
			state_set(state, LE_get_token(io[0]->fp, c, state));

			if (is_set(state, LEX_MAP))
				build_priority_queue_from_file(list, io);
			else if (is_set(state, LEX_DECOMPRESS))
				read_compressed_file(io[i]->fp, io[0]->fp);
			else
				break;
		}
	}

	LE_lexer_free();

	return state;
}

