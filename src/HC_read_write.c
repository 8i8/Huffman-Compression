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

/*
 * write_map_to_file: Write the frequency of each used characters repetition
 * used in the encoding of the file to the start of the file, so as to allow
 * for the recreation of the same Huffman tree for decompression.
 */
void write_map_to_file(Data **map, FILE *out)
{
	String *buf = NULL;
	char *num, *pt_num;
	size_t i;
	Data *cur;

	pt_num = num = malloc(256);
	buf = GE_string_init(buf);
	buf = GE_string_concat(buf, "<map>\n", 6);

	for (i = 0; i < MAP_LEN; i++) {
		if (map[i] != NULL) {
			GE_string_concat(buf, "\t", 1);
			GE_string_concat(buf, map[i]->utf8_char,
					strlen(map[i]->utf8_char));
			GE_string_concat(buf, " ~ ", 3);
			sprintf(num, "%lu", map[i]->frq);
			GE_string_concat(buf, num, strlen(num));
			GE_string_concat(buf, "\n", 1);
			num = pt_num;
			if ((cur = map[i]->next)) {
				while ((cur = cur->next)) {
					GE_string_concat(buf, "\t", 1);
				      	GE_string_concat(buf, map[i]->utf8_char,
							strlen(map[i]->utf8_char));
					GE_string_concat(buf, " ~ ", 3);
					sprintf(num, "%lu", map[i]->frq);
					GE_string_concat(buf, num, strlen(num));
					GE_string_concat(buf, "\n", 1);
					num = pt_num;
				}
			}
		}
	}
	free(num);

	GE_string_concat(buf, "</map>\n", 7);
	fwrite(buf->str, 1, buf->len, out);

	GE_string_free(buf);
}

/*
 * write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 */
static void write_bit(
				FILE *out,
				unsigned char bit,
				unsigned char *byte,
				unsigned *bit_count)
{
	if (++(*bit_count) == 8) {
		fwrite(byte, 1, 1, out);
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
unsigned compress_file(Data **map, FILE *in, FILE *out, const unsigned state)
{
	unsigned bit_count, utf8_count;
	unsigned char byte;

	char c, *ptr, utf8_char[5], *bin;
	size_t len, i;
	ptr = utf8_char;

	bit_count = byte = utf8_count = 0;

	fwrite("<comp>\n\t", 1, 8, out);

	//TODO NEXT Buffer this input to make more efficient.
	while ((c = fgetc(in)) != EOF && utf8_count < 4)
	{
		/* Get char for the lenght of what is possibly a multi-byte
		 * character */
		while ((*ptr++ = c) && (utf8_count = utf8_countdown(c)) && utf8_count < 4)
			c = fgetc(in);

		*ptr = '\0';

		bin = map_read_char_to_binary(map, utf8_char);
		len = map_read_char_to_binary_len(map, utf8_char);
		for (i = 0; i < len; i++, bin++)
			write_bit(out, bin[0], &byte, &bit_count);

		ptr = utf8_char;
	}

	if (utf8_count > 4)
		fprintf(stderr, "%s: utf8_countdown error.\n", __func__);

	/* Add EOF char */
	bin = map_read_char_to_binary(map, "EOF");
	len = map_read_char_to_binary_len(map, utf8_char);
	for (i = 0; i < len; i++, bin++)
		write_bit(out, bin[0], &byte, &bit_count);

	/* Pad any remaining bits in the last byte with zeroes */
	if(bit_count > 0) {
		 byte <<= 8 - bit_count;
		 fwrite(&byte, 1, 1, out);
	}

	fwrite("\n</comp>\n", 1, 9, out);

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
 */
unsigned decompress_file(HC_HuffmanNode **list, FILE *in, FILE *out, unsigned state)
{
	LE_lexer_init();
	char c;

	if (is_set(state, VERBOSE))
		printf("Decompress file.\n");

	while ((c = fgetc(in)) != EOF)
	{
		/* Read frequency map from file start */
		state_set(state, LE_get_token(in, c, state));

		if (is_set(state, LEX_MAP))

			build_priority_queue_from_file(list, in);

		else if (is_set(state, LEX_DECOMPRESS))

			read_compressed_file(in, out);

		else
			break;
	}

	LE_lexer_free();

	return state;
}

