#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <ctype.h>
#include "huffman/HC_huffman_tree.h"
#include "general/GE_hashtable.h"
#include "huffman/HC_priority_queue.h"
#include "general/GE_error.h"
#include "general/GE_utf8.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_string.h"
#include "general/GE_hashtable.h"
#include "general/GE_file_buffer.h"
#include "program/PG_print.h"
#include "lexer/LE_xml.h"
#include "lexer/LE_lexer.h"
#include "bitwise/BI_bitwise.h"

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
	Data *cur;

	if (is_set(st_prg, VERBOSE))
		printf("metadata: writing map.\n");

	GE_buffer_on(buf);
	LE_xml_element_open(buf, "map");

	for (i = 0; i < MAP_LEN; i++)
		if (map[i].utf8_char[0] != '\0') {
			cur = &map[i];
			if (cur->next) {
				while (cur->next) {
					LE_xml_element_map(
							buf,
							cur->utf8_char,
							cur->binary);
					cur = cur->next;
				}
			}
			LE_xml_element_map(
						buf,
						cur->utf8_char,
						cur->binary);
		}
	
	LE_xml_element_close(buf, "map");
	GE_buffer_fwrite_FILE(buf);
	GE_buffer_off(buf);
}

/*
 * compression_write_archive: Write archive.
 */
int compression_write_archive(
						Data map[MAX_FILES],
						F_Buf *buf_read,
						F_Buf *buf_write,
						const int st_prg)
{
	Data data;
	int bit_count, utf8_count;
	unsigned char byte;
	char c, *ptr, utf8_char[UTF8_LEN], *bin_ptr;
	size_t i, j;
	ptr = utf8_char;
	bit_count = utf8_count = byte = 0;

	if (is_set(st_prg, VERBOSE))
		printf("compression: file %s.\n", buf_read->name);

	GE_buffer_on(buf_write);
	LE_xml_element_open(buf_write, "comp");
	GE_buffer_on(buf_read);

	/* Read binary representation of each char read and write that
	 * representation into the archive bit by bit */
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
		data = GE_hashtable_lookup_utf8(map, utf8_char);
		if (data.binary[0] == '\0')
			FAIL("hashmap");

		/* set the pointer to the start of the text representation of
		 * the binary value to be read */
		bin_ptr = data.binary;
		for (j = 0; j < data.len_bin; j++)
			BI_write_bit(
						buf_write,
						/* conver to int */
						(unsigned char)bin_ptr[j]-'0',
						&byte,
						&bit_count);
		ptr = utf8_char;
	}

	GE_buffer_rewind(buf_read);
	GE_buffer_off(buf_read);

	if (utf8_count > 3)
		FAIL("utf8_countdown");

	/* Add EOF char */
	data = GE_hashtable_lookup_utf8(map, "EOF");
	if (data.binary[0] == '\0')
		FAIL("hashmap");

	/* set the pointer for the binary value to be read */
	bin_ptr = data.binary;
	for (i = 0; i < data.len_bin; i++)
		BI_write_bit(
						buf_write,
						(unsigned)bin_ptr[i]-'0',
						&byte,
						&bit_count);

	/* Pad any remaining bits in the last byte with zeroes */
	if (bit_count > 0) {
		 byte <<= 8 - bit_count;
		 GE_buffer_fwrite((char*)&byte, 1, 1, buf_write);
		 if (BIN_LOG_IN) BI_binary_log(byte, 8);
	}
	GE_buffer_fwrite("\n", 1, 1, buf_write);

	if (BIN_LOG_IN) BI_binary_log_flush();
	LE_xml_element_close(buf_write, "comp");
	GE_buffer_fwrite_FILE(buf_write);
	GE_buffer_off(buf_write);

	if (is_set(st_prg, VERBOSE))
		printf("File: %s, appended to archive.\n", buf_read->name);

	return err;
}

/*
 * metadata_read_hash_table_data: Create a hash table using the key value
 * pairs.
 */
int metadata_read_hash_table_data(
						F_Buf *buf,
						Data *map,
						int st_lex,
						const int st_prg)
{
	char c = ' ';
	Data data;
	data = GE_data_init();

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
		if (is_set(st_lex, LEX_CHAR))
		{
			/* Read utf-8 char */
			c = LE_get_utf8_char(buf, &data);

			/* read binary representation */
			c = LE_get_string(buf, c, &data);

			/* Store the data */
			//TODO NEXT the length data should be stored in the
			//archive
			data.len_bin = strlen(data.binary);
			data.len_char = strlen(data.utf8_char);
			GE_hashtable_add_binary_key(map, data);
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
 * decompress_write_file: Inflate a text file from the given archive.
 */
char decompress_write_file(
						F_Buf *buf_read,
						F_Buf *buf_write,
						Data *map,
						int *st_lex,
						const int st_prg)
{
	char c = ' ';
	unsigned char byte;
	int bit_count, ignore;
	bit_count = ignore = 0;
	String *str = NULL;
	str = GE_string_init(str);

	if (is_set(st_prg, VERBOSE))
		printf("decompression: writing from archive %s to %s.\n",
				buf_read->name, buf_write->name);

	GE_buffer_on(buf_write);

	/*
	 * TODO NEXT pushback added to LE_get_token now changed to read ahead,
	 * should the pushback be left in get_token, it must at least be tested.
	 */
	c = GE_buffer_fgetc(buf_read); /* get the '\n' after the <comp> tag */
	c = byte = GE_buffer_fgetc(buf_read); /* get the first byte of compressed data */
	while (is_set(*st_lex, LEX_DECOMPRESS))
	{
		if (c == '<' && !ignore) {
		//if (c == '<' && !is_set(*st_lex, LEX_DECOMPRESS)) {
			/* Check forwars for sign of a token, if not found then
			 * ignore the '<' untill the next char is read */
			if (LE_look_ahead(buf_read, '>', '<', TOKEN_MAX)) {
				if ((c = LE_get_token(buf_read, c, st_lex)))
					break;
			} else
				ignore++;
		}
		 /* 
		  * BI_read_bit: Read each bit of a char, checking each time to
		  * see if the binary string that is being created is in the
		  * hash table, if it is found; Write the corresponding char
		  * into the output file buffer and then refresh the string.
		  */
		c = BI_read_bit(
						buf_read, buf_write, map,
						str, c, &byte, &bit_count,
						&ignore, st_lex);
	}

	GE_buffer_fwrite_FILE(buf_write);
	GE_buffer_off(buf_write);
	GE_buffer_fclose(buf_write);
	GE_string_free(str);

	return c;
}

