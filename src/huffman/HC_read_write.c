#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "data_structures/DS_huffman_node.h"
#include "data_structures/DS_huffman_tree.h"
#include "huffman/HC_map_char.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_print.h"
#include "lexer/LE_lexer.h"
#include "general/GE_utf8.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_string.h"
#include "general/GE_file_buffer.h"

/*
 * write_char_map: Write the map as a dictionary format.
 * TODO NOW: char map written to file here, rewrite this this functionality as
 * JSON.
 */
static inline void write_char_map(
					Data *map,
					F_Buf *buf,
					char *num,
					char * const pt)
{
	GE_buffer_fwrite("\t", 1, 1, buf);
	GE_buffer_fwrite(map->utf8_char, 1,
			strlen(map->utf8_char), buf);
	GE_buffer_fwrite(" ", 1, 1, buf);
	sprintf(num, "%lu", map->frq);
	GE_buffer_fwrite(num, 1, strlen(num), buf);
	GE_buffer_fwrite("\n", 1, 1, buf);
	num = pt;
}

/*
 * write_map_to_file: Write the frequency of each used characters number of
 * repetitions, used in the encoding of the file to the start of the file, so
 * as to allow for the recreation of the same Huffman tree for decompression.
 */
void write_map_to_file(Data **map, F_Buf *buf)
{
	char *num, *ptr;
	size_t i;
	Data *cur;

	ptr = num = malloc(256);
	GE_buffer_on(buf);
	GE_buffer_fwrite("<map>\n", 1, 6, buf);

	for (i = 0; i < MAP_LEN; i++)
		if (map[i] != NULL) {
			write_char_map(map[i], buf, num, ptr);
			if ((cur = map[i]->next))
				while ((cur = cur->next))
					write_char_map(map[i], buf, num, ptr);
		}
	
	free(ptr);

	GE_buffer_fwrite("</map>\n", 1, 7, buf);
	GE_buffer_fwrite_FILE(buf);
	GE_buffer_off(buf);
}

/*
 * write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 * to.
 */
static void write_bit(
				F_Buf *buf,
				unsigned char bit,
				unsigned char *byte,
				unsigned *bit_count)
{
	if (++(*bit_count) == 8) {
		GE_buffer_fwrite((char*)byte, 1, 1, buf);
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
int encode_file(Data **map, F_Buf **io, const int st_prg)
{
	unsigned bit_count, utf8_count;
	unsigned char byte;
	Data *data;

	char c, *ptr, utf8_char[5], *bin;
	size_t i, j;
	ptr = utf8_char;

	bit_count = byte = utf8_count = 0;

	GE_buffer_on(io[0]);
	GE_buffer_fwrite("<comp>\n", 1, 8, io[0]);

	//TODO NEXT is this a good use of state; Is it even required?
	for (i = is_set(st_prg, COMPRESS); i < MAX_FILES && io[i]; i++)
	{
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
		 GE_buffer_fwrite((char*)&byte, 1, 1, io[0]);
	}

	GE_buffer_fwrite("\n</comp>\n", 1, 9, io[0]);
	GE_buffer_fwrite_FILE(io[0]);
	GE_buffer_off(io[0]);

	if (is_set(st_prg, VERBOSE))
		printf("Compressed file written.\n");

	return st_prg;
}

/*
 * metadata_get_filename: Call all functions required to retrieve archive meta
 * data.
 */
//static int metadata_get_filename(HC_HuffmanNode **list, F_Buf **io, int st_lex)
//{
//}

/*
 * decompress_mono: Open an archive encoded with one single character map made
 * from all the files.
 */
static int decompress_mono(HC_HuffmanNode **list, F_Buf **io, int st_prg, int st_lex)
{
	char c;
	if (is_set(st_lex, LEX_META))
	{
		if (is_set(st_prg, VERBOSE))
			fwrite("Opening archive metadata.\n", 1, 23, stdout);

		/* If there is no file specified as an output for an appended
		 * file, open a file with the name given in the archives meta
		 * data. */
		if (!io[1])
			GE_open_file(NULL, io, "wb", st_lex);

		st_lex = decompression_priority_queue(list, io[0], st_lex);
		/* Skip over the end of line */
		c = GE_buffer_fgetc(io[0]);
		build_ordered_binary_tree(list, st_lex);

		if (is_set(st_prg, PRINT)) {
			fwrite("print frequency map.\n", 1, 22, stdout);
			print_frequency_map(*list);
		}

	}

	return st_lex;
}

///*
// * decompress_multi: Open a multi file archive encoded with a differnt map for
// * each file.
// */
//static int decompress_multi(HC_HuffmanNode **list, F_Buf **io, int st_lex)
//{
//	char c;
//	/* Read and build the Huffman tree from the frequency
//	 * map */
//	if (is_set(st_lex, LEX_META))
//	{
//		st_lex = decompression_priority_queue(list, io[0], st_lex);
//		/* Skip over the end of line */
//		c = GE_buffer_fgetc(io[0]);
//		build_ordered_binary_tree(list, st_lex);
//	}
//
//	/* Decompress the file */
//	else if (is_set(st_lex, LEX_DECOMP))
//	{
//		decompress_text(io[0]->fp, io[0]->fp);
//	}
//	return st_lex;
//}

/*
 * decompress_archive: Read and then decompress compressed file. Analyze file
 * stream with lexer to decompress the file.
 * TODO NOW get file name and open file for writing.
 */
int decompress_archive(HC_HuffmanNode **list, F_Buf **io, const int st_prg)
{
	char c;
	/* Lexer state */
	int st_lex = 0;
	LE_lexer_init();

	if (is_set(st_prg, VERBOSE))
		printf("Opening archive.\n");

	GE_buffer_on(io[0]);
	while (io[0] && (c = GE_buffer_fgetc(io[0])) != EOF)
	{
		/* Set the state */
		if ((st_lex = LE_get_token(io[0], c, st_lex)) == 0) {
			fprintf(stderr, "%s: Token failed.\n", __func__);
			state_set(st_lex, ERROR);
			break;
		}

		if (is_set(st_lex, MONO))
			decompress_mono(list, io, st_prg, st_lex);

	//	else (is_set(st_lex, MULTI))
	//		decompress_multi(io, st_lex);
	}
	GE_buffer_rewind(io[0]);
	GE_buffer_off(io[0]);

	LE_lexer_free();

	return st_lex;
}

