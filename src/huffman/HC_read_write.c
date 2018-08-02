#include <string.h>
#include <stdlib.h>
#include "data_structures/DS_huffman_node.h"
#include "data_structures/DS_huffman_tree.h"
#include "huffman/HC_map_char.h"
#include "huffman/HC_priority_queue.h"
#include "lexer/LE_lexer.h"
#include "general/GE_utf8.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_string.h"
#include "general/GE_file_buffer.h"

/*
 * write_map_to_file: Write the frequency of each used characters repetition
 * used in the encoding of the file to the start of the file, so as to allow
 * for the recreation of the same Huffman tree for decompression.
 * TODO NEXT seperate out writing into a fulction.
 */
void write_map_to_file(Data **map, F_Buf *buf)
{
	char *num, *pt_num;
	size_t i;
	Data *cur;

	pt_num = num = malloc(256);
	GE_buffer_on(buf);
	GE_buffer_fwrite("<map>\n", 1, 6, buf);

	for (i = 0; i < MAP_LEN; i++) {
		if (map[i] != NULL) {
			GE_buffer_fwrite("\t", 1, 1, buf);
			GE_buffer_fwrite(map[i]->utf8_char, 1,
					strlen(map[i]->utf8_char), buf);
			GE_buffer_fwrite(" ", 1, 1, buf);
			sprintf(num, "%lu", map[i]->frq);
			GE_buffer_fwrite(num, 1, strlen(num), buf);
			GE_buffer_fwrite("\n", 1, 1, buf);
			num = pt_num;
			if ((cur = map[i]->next)) {
				while ((cur = cur->next)) {
					GE_buffer_fwrite("\t", 1, 1, buf);
					GE_buffer_fwrite(map[i]->utf8_char, 1,
							strlen(map[i]->utf8_char), buf);
					GE_buffer_fwrite(" ", 1, 1, buf);
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
	GE_buffer_fwrite(buf);
	GE_buffer_off(buf);
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
 * encode_file: Write compressed file.
 */
unsigned encode_file(Data **map, F_Buf **io, const unsigned state)
{
	unsigned bit_count, utf8_count;
	char byte;
	Data *data;

	char c, *ptr, utf8_char[5], *bin;
	size_t i, j;
	ptr = utf8_char;

	bit_count = byte = utf8_count = 0;

	GE_buffer_on(io[0]);
	GE_buffer_fwrite("<comp>\n", 1, 8, io[0]);

	for (i = is_set(state, COMPRESS); i < MAX_FILES && io[i]; i++) {
		GE_buffer_on(io[i]);
		while ((c = GE_buffer_fgetc(io[i])) != EOF && utf8_count < 4)
		{
			/* Get a character from the file buffer, test for
			 * multibyte char, if postive get n char from the
			 * buffer to compleete the character. */
			while ((*ptr++ = c)
					&& (utf8_count || (utf8_count = utf8_len(c)))
					&& utf8_count < 4)
				c = GE_buffer_fgetc(io[i]), utf8_count--;

			*ptr = '\0';

			/* Retreive huffman coding */
			if ((data = HC_map_lookup_data(map, utf8_char)) == NULL)
				fprintf(stderr, "%s: hashmap error.\n", __func__);
			bin = data->string;

			for (j = 0; j < data->len; j++, bin++)
				write_bit(io[0], bin[0], &byte, &bit_count);

			ptr = utf8_char;
		}

		GE_buffer_rewind(io[i]);
		GE_buffer_off(io[i]);

		if (utf8_count > 4)
			fprintf(stderr, "%s: utf8_countdown error.\n", __func__);
	}

	/* Add EOF char */
	if((data = HC_map_lookup_data(map, "EOF")) == NULL)
		fprintf(stderr, "%s: hashmap error.\n", __func__);
	bin = data->string;

	for (i = 0; i < data->len; i++, bin++)
		write_bit(io[0], bin[0], &byte, &bit_count);

	/* Pad any remaining bits in the last byte with zeroes */
	if(bit_count > 0) {
		 byte <<= 8 - bit_count;
		 GE_buffer_fwrite(&byte, 1, 1, io[0]);
	}

	GE_buffer_fwrite("\n</comp>\n", 1, 9, io[0]);
	GE_buffer_fwrite(io[0]);
	GE_buffer_off(io[0]);

	if (is_set(state, VERBOSE))
		printf("Compressed file written.\n");

	return state;
}

/*
 * read_file_meta_data: Readin file name and compression type.
 */
//int read_file_meta_data(F_Buf *buf)
//{
//}

/*
 * read_compressed_file: Read the compressed file and write the decompressed
 * data to the output file.
 * TODO NOW write this function
 */
FILE *read_compressed_file(FILE *in, FILE *out)
{
	in = out;

	return in;
}

/*
 * decode_file: Read and then decompress compressed file. Analyze file
 * stream with lexer to decompress the file.
 * TODO NOW get file name and open file for writing.
 */
unsigned decode_file(HC_HuffmanNode **list, F_Buf **io, unsigned state)
{
	char c;
	LE_lexer_init();

	if (is_set(state, VERBOSE))
		printf("Decompress file.\n");

	GE_buffer_on(io[0]);
	while (io[0] && (c = GE_buffer_fgetc(io[0])) != EOF)
	{
		/* Set the state */
		if ((state = LE_get_token(io[0], c, state)) == 0) {
			fprintf(stderr, "%s: Token failed.\n", __func__);
			state_set(state, ERROR);
			break;
		}

		if (is_set(state, LEX_META_DATA))
		{
			; //read_file_meta_data(io[0], &name);
		}

		/* Read and build the Huffman tree from the frequency
		 * map */
		if (is_set(state, LEX_MAP))
		{
			state = priority_queue_decompression(list, io[0], state);
			/* Skip over the end of line */
			c = GE_buffer_fgetc(io[0]);
			build_ordered_binary_tree(list, state);
		}

		/* Decompress the file */
		else if (is_set(state, LEX_DECOMPRESS))
		{
			read_compressed_file(io[0]->fp, io[0]->fp);
		} else
			break;
	}
	GE_buffer_rewind(io[0]);
	GE_buffer_off(io[0]);

	LE_lexer_free();

	return state;
}

