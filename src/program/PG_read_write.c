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
#include "general/GE_file_buffer.h"
#include "program/PG_print.h"
#include "lexer/LE_xml.h"
#include "lexer/LE_lexer.h"
#include "general/GE_binary.h"

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
	cur = HC_data_init();

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
#define BIN_OUT 0
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


	if (BIN_OUT) print_binary_of_int((unsigned long) *byte, 8);

	/* Shift left ready for the next bit */
	*byte <<= 1;

	if (BIN_OUT) print_binary_of_int((unsigned long) *byte, 8);
	if (BIN_OUT) print_binary_of_int((unsigned long) bit, 8);

	/* Set bit to 1 or 0 */
	*byte |= bit;

	if (BIN_OUT) print_binary_of_int((unsigned long) *byte, 8);
}

/*
 * compression_write_file: Write compressed file.
 */
int compression_write_file(
						Data map[MAX_FILES],
						F_Buf *buf_read,
						F_Buf *buf_write,
						const int st_prg)
{
	int bit_count, utf8_count;
	unsigned char byte;
	int bucket;
	bucket = err = 0;

	char c, *ptr, utf8_char[UTF8_LEN], *bin_ptr;
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

		/* Retreive huffman encoding */
		bucket = hash(utf8_char);
		if (map[bucket].binary[0] == '\0')
			FAIL("hashmap");
		bin_ptr = map[bucket].binary;
		for (j = 0; j < map[bucket].len_bin; j++)
			write_bit(
						buf_write,
						(unsigned)bin_ptr[j]-'0',
						&byte, &bit_count);

		ptr = utf8_char;
	}
	GE_buffer_rewind(buf_read);
	GE_buffer_off(buf_read);

	if (utf8_count > 4)
		FAIL("utf8_countdown");

	/* Add EOF char */
	bucket = hash("EOF");
	if (map[bucket].binary[0] == '\0')
		FAIL("hashmap");
	bin_ptr = map[bucket].binary;

	for (i = 0; i < map[bucket].len_bin; i++)
		write_bit(
						buf_write,
						(unsigned)bin_ptr[i]-'0',
						&byte, &bit_count);

	/* Pad any remaining bits in the last byte with zeroes */
	if(bit_count > 0) {
		 byte <<= 8 - bit_count;
		 GE_buffer_fwrite((char*)&byte, 1, 1, buf_write);
	}
	GE_buffer_fwrite("\n", 1, 1, buf_write);

	LE_xml_element_close(buf_write, "comp");
	GE_buffer_fwrite_FILE(buf_write);
	GE_buffer_off(buf_write);

	if (is_set(st_prg, VERBOSE))
		printf("archive: %s written.\n", buf_write->name);

	return err;
}

/*
 * metadata_read_hash_table_data: Create a hash table using the key value pairs
 */
int metadata_read_hash_table_data(
						F_Buf *buf,
						Data *map,
						int st_lex,
						const int st_prg)
{
	char c = ' ';
	char *ptr_bin, *ptr_utf8;
	Data data;
	data = HC_data_init();
	ptr_bin = data.binary;
	ptr_utf8 = data.utf8_char;

	if (is_set(st_prg, VERBOSE))
		printf("metadata: reading hashtable pairs.\n");

	while (buf && is_set(st_lex, LEX_MAP) && c != EOF)
	{
		/* remove spaces */
		c = LE_goto_glyph(buf, c, '<');

		if ((c = LE_get_token(buf, c, &st_lex)) == 0) {
			FAIL("failed to read metadata");
			state_set(st_lex, LEX_ERROR);
			return st_lex;
		}

 		/* hash table for inflation created here */
		//TODO NEXT read in length values here
		if (is_set(st_lex, LEX_CHAR))
		{
			/* Read utf-8 char */
			c = LE_get_utf8_char(buf, ptr_utf8);

			/* read binary representation */
			c = LE_get_string(buf, c, ptr_bin);

			/* Store the data */
			//TODO NEXT the length data should be stored in the archive
			data.len_bin = strlen(data.binary);
			data.len_char = strlen(data.utf8_char);
			HC_hashtable_add_binary_key(map, data);
		}
	}

	return st_lex;
}

/*
 * metadata_read_filename: Read the filename from the archive metadata.
 */
String metadata_read_filename(
						F_Buf *buf,
						String str,
						int *st_lex,
						const int st_prg)
{
	char c = ' ';

	if (is_set(st_prg, VERBOSE))
		printf("metadata: reading filename.\n");

	while ((c = GE_buffer_fgetc(buf)) != '<' && c != EOF)
		GE_string_add_char(&str, c);

	if ((c = LE_get_token(buf, c, st_lex)) == 0) {
		FAIL("read metadata");
		state_set(*st_lex, LEX_ERROR);
		return str;
	}

	return str;
}

/*
 * decompress_write_text_file: Inflate a text file from the given archive.
 */
int decompress_write_text_file(
						F_Buf *buf_read,
						F_Buf *buf_write,
						Data *map,
						int st_lex,
						const int st_prg)
{
	unsigned char c = ' ';
	unsigned char byte = ' ';
	unsigned bit_count;
	unsigned bit, bucket;
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
	 * see if the binary string is in the hash table, if it is found write the
	 * corresponding char into the output file buffer and refresh the string.
	 * TODO NOW pushback added to LE_get_token now changed to read ahead,
	 * should the pushback be left in get_token, it must at least be tested.
	 */
	c = GE_buffer_fgetc(buf_read);
	bit_count = 7;
	while (is_set(st_lex, LEX_DECOMPRESS))
	{
		if (c == '<' && ((LE_read_ahead(buf_read, c, &st_lex)) == 0)) {
			FAIL("writing text file");
			break;
		}

		/* Reset */
		if (++bit_count == 8)
			c = byte = GE_buffer_fgetc(buf_read), bit_count = 0;

		/* Read binary */
		bit = byte & 1;
		unsigned mask = (1 << bit_count) - 1;
		byte &= mask;
		GE_string_add_char(str, (char)bit+'0');

		/* Look up the string in its current state in the hash table,
		 * when a char is found the string is reset */
		data = HC_hashtable_lookup_string(map, *(str->str));
		if (data.utf8_char[0] != '\0') {
			GE_buffer_fwrite(
							data.utf8_char, 1,
							data.len_char, buf_write);
			GE_string_reset(str);
		}

		/* Shift right ready for the next bit */
		byte >>= 1;
	}

	GE_buffer_fwrite_FILE(buf_write);
	GE_buffer_off(buf_write);

	return st_lex;
}

