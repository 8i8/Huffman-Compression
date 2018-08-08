#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <ctype.h>
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_map_char.h"
#include "huffman/HC_priority_queue.h"
#include "general/GE_error.h"
#include "general/GE_utf8.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_string.h"
#include "general/GE_print.h"
#include "general/GE_file_buffer.h"
#include "lexer/LE_xml.h"

/*
 * metadata_write_file_name: Write file name in the archive meta data at the
 * given tab indenatation.
 */
void metadata_write_file_name(F_Buf *buf)
{
	GE_buffer_on(buf);
	LE_xml_element_item(buf, basename(buf->name), "name");
	GE_buffer_fwrite_FILE(buf);
	GE_buffer_off(buf);
}

/*
 * metadata_write_map: Write the frequency of each used characters number of
 * repetitions, used in the encoding of the file to the start of the file, so
 * as to allow for the recreation of the same Huffman tree for decompression.
 */
void metadata_write_map(Data *map, F_Buf *buf)
{
	size_t i;
	Data cur;

	GE_buffer_on(buf);
	LE_xml_element_open(buf, "map");

	for (i = 0; i < MAP_LEN; i++)
		if (map[i].binary[0] != '\0') {
			LE_xml_element_map(buf, map[i].utf8_char, map[i].binary);
			if (map[i].next != NULL) {
				cur = *map[i].next;
				while (cur.next != NULL) {
					LE_xml_element_map(buf, map[i].utf8_char, map[i].binary);
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
 * compression_write_file: Write compressed file.
 */
int compression_write_file(
						Data map[MAX_FILES],
						F_Buf *buf_read,
						F_Buf *buf_write,
						const int st_prg)
{
	unsigned bit_count, utf8_count;
	unsigned char byte;
	int bucket, err;
	bucket = err = 0;

	char c, *ptr, utf8_char[5], *bin;
	size_t i, j;
	ptr = utf8_char;

	bit_count = byte = utf8_count = 0;

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
		bucket = hash(utf8_char);
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

	/* Add EOF char */
	bucket = hash("EOF");
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

	if (is_set(st_prg, VERBOSE))
		printf("Compressed file written.\n");

	return err;
}

