#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <ctype.h>
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_hashtable.h"
#include "huffman/HC_priority_queue.h"
#include "general/GE_error.h"
#include "general/GE_utf8.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_string.h"
#include "general/GE_print.h"
#include "general/GE_file_buffer.h"
#include "lexer/LE_xml.h"
#include "lexer/LE_lexer.h"

/*
 * metadata_write_file_name: Write file name in the archive meta data at the
 * given tab indenatation.
 */
void metadata_write_file_name(F_Buf *read, F_Buf *write, const int st_prg)
{
	if (is_set(st_prg, VERBOSE))
		printf("metadata: writing filename.\n");

	GE_buffer_on(write);
	LE_xml_element_item(write, basename(read->name), "name");
	GE_buffer_fwrite_FILE(write);
	GE_buffer_off(write);
}

/*
 * metadata_write_map: Write the frequency of each used characters number of
 * repetitions, used in the encoding of the file to the start of the file, so
 * as to allow for the recreation of the same Huffman tree for decompression.
 */
void metadata_write_map(Data *map, F_Buf *buf, const int st_prg)
{
	size_t i;
	Data cur;

	if (is_set(st_prg, VERBOSE))
		printf("metadata: writing map.\n");

	GE_buffer_on(buf);
	LE_xml_element_open(buf, "map");

	for (i = 0; i < MAP_LEN; i++)
		if (map[i].binary[0] != '\0') {
			LE_xml_element_map(
								buf,
								map[i].utf8_char,
								map[i].binary);
			if (map[i].next != NULL) {
				cur = *map[i].next;
				while (cur.next != NULL) {
					LE_xml_element_map(
								buf,
								map[i].utf8_char,
								map[i].binary);
					cur = *cur.next;
				}
			}
		}
	
	LE_xml_element_close(buf, "map");
	GE_buffer_fwrite_FILE(buf);
	GE_buffer_off(buf);
}

/*
 * write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 */
static void write_bit(
						F_Buf *buf,
						unsigned char bit,
						unsigned char *byte,
						int *bit_count)
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
 * compression_write_file: Write compressed file.
	
 write_bit(buf_write, bin[0], &byte, &bit_count);

 */
int compression_write_file(
						Data map[MAX_FILES],
						F_Buf *buf_read,
						F_Buf *buf_write,
						const int st_prg)
{
	int bit_count, utf8_count;
	unsigned char byte;
	int bucket, err;
	bucket = err = 0;

	char c, *ptr, utf8_char[UTF8_LEN], *bin;
	size_t i, j;
	ptr = utf8_char;

	bit_count = byte = utf8_count = 0;

	if (is_set(st_prg, VERBOSE))
		printf("compression: file %s.\n", buf_read->name);

	GE_buffer_on(buf_write);
	LE_xml_element_open(buf_write, "comp");

	GE_buffer_on(buf_read);
	while ((c = GE_buffer_fgetc(buf_read)) != EOF && utf8_count < 4)
	{
		/* Get a character from the file buffer, test for
		 * multibyte char, if postive get n char from the
		 * buffer to compleete the character. */
		while ((*ptr++ = c)
				&& (utf8_count || (utf8_count = utf8_len(c)))
				&& utf8_count < 4)
			c = GE_buffer_fgetc(buf_read), utf8_count--;

		*ptr = '\0';

		/* Retreive huffman coding */
		bucket = hash(utf8_char[UTF8_LEN]);
		if (map[bucket].binary[0] == '\0')
			FAIL("hashmap");
		bin = map[bucket].binary;
		for (j = 0; j < map[bucket].len; j++, bin++)
			write_bit(buf_write, bin[0], &byte, &bit_count);

		ptr = utf8_char;
	}
	GE_buffer_rewind(buf_read);
	GE_buffer_off(buf_read);

	if (utf8_count > 4)
		FAIL("utf8_countdown");

	char eof[4] = "EOF";
	/* Add EOF char */
	bucket = hash(eof[4]);
	if (map[bucket].binary[0] == '\0')
		FAIL("hashmap");
	bin = map[bucket].binary;

	for (i = 0; i < map[bucket].len; i++, bin++)
		write_bit(buf_write, bin[0], &byte, &bit_count);

	/* Pad any remaining bits in the last byte with zeroes */
	if(bit_count > 0) {
		 byte <<= 8 - bit_count;
		 GE_buffer_fwrite((char*)&byte, 1, 1, buf_write);
	}
	GE_buffer_fwrite("\n", 1, 1, buf_write);

	LE_xml_element_close(buf_write, "comp");
	GE_buffer_fwrite_FILE(buf_write);
	GE_buffer_off(buf_write);

	return err;
}

/*
 * metadata_read_create_table: Create a hash table using the key value pairs
 * retreived from the archive metadata.
 */
int metadata_read_create_table(F_Buf **io, Data *map, int st_lex, const int st_prg)
{
	char c = ' ';
	char *ptr_bin, *ptr_utf8;
	Data data;
	data = HC_data_init();
	ptr_bin = data.binary;
	ptr_utf8 = data.utf8_char;

	if (is_set(st_prg, VERBOSE))
		printf("metadata: reading hashtable pairs.\n");

	while (io[0] && is_set(st_lex, LEX_MAP) && c != EOF)
	{
		if ((c = LE_get_token(io[0], c, &st_lex)) == 0)
			FAIL("Token failed");

 		// TODO NOW hash table for inflation created here
		if (is_set(st_lex, LEX_CHAR))
		{
			/* Read utf-8 char */
			c = LE_get_utf8_char(io[0], ptr_utf8);

			/* read binary representation */
			c = LE_get_string(io[0], c, ptr_bin);

			/* Store the data */
			HC_hashtable_add_binary_key(map, *(data.binary), *(data.utf8_char));
		}
	}

	return st_lex;
}

/*
 * metadata_read_filename: Read the filename from the archive metadata.
 */
String metadata_read_filename(F_Buf *buf, String str, int *st_lex, const int st_prg)
{
	char c = ' ';

	if (is_set(st_prg, VERBOSE))
		printf("metadata: reading filename.\n");

	while ((c = GE_buffer_fgetc(buf)) != '<' && c != EOF)
		GE_string_add_char(&str, c);

	if ((c = LE_get_token(buf, c, st_lex)) == 0)
		FAIL("Token failed expected </name>");

	return str;
}

/*
 * decompress_write_text_file: Inflate a text file from the given arvhive.
 */
int decompress_write_text_file(
						F_Buf *buf_read,
						F_Buf *buf_write,
						Data *map,
						const int st_prg)
{
	char c = ' ';
	char byte = ' ';
	int bit_count;
	int bit, bucket;
	Data data;
	data = HC_data_init();
	String *str = NULL;
	str = GE_string_init(str);
        bit = bucket = 0;

	if (is_set(st_prg, VERBOSE))
		printf("decompression: writing from archive %s to %s.\n",
				buf_read->name, buf_write->name);

	GE_buffer_on(buf_write);

	/*
	 * read_bit: Read each bit of a new character, checking each time to
	 * see if the binary string is in the hash tabel, if it is write the
	 * coresponding char to the output file and start a new string.
	 */
	while ((c = GE_buffer_fgetc(buf_read)) != '<' && c != EOF)
	{
		/* Reset */
		if (++bit_count == 8)
			byte = GE_buffer_fgetc(buf_read), bit_count = 0;

		// var bit = (b & (1 << bitNumber-1)) != 0;

		/* Write the state */
		bit = byte & (1 << 0);
		GE_string_add_char(str, (char)bit+'0');

		/* Look up the string in its current state in the hash tabel,
		 * when a char is found the strig is reset */
		// TODO NOW hash table is finding the string '0'
		data = HC_hashtable_lookup_string(map, *(str->str));
		if (data.utf8_char[0] != '\0')
		{
			GE_buffer_fwrite(
						map[bucket].utf8_char,
						1,
						utf8_len(map[bucket].utf8_char[0]),
						buf_write);
			GE_string_reset(str);
		}

		/* Shift right ready for the next bit */
		byte >>= 1;
	}

	GE_buffer_fwrite_FILE(buf_write);
	GE_buffer_off(buf_write);

	return 0;
}

