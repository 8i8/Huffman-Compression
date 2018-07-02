#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HC_state.h"
#include "HC_struct.h"
#include "HC_map_char.h"
#include "HC_utf8.h"
#include "HC_priority_queue.h"
#include "LE_lexer.h"
#include "GE_hash.h"
#include "GE_string.h"

extern int state;

/*
 * Reverse the array.
 */
static void reverse(char *s)
{
	size_t i, j;
	int c;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[j];
		s[j] = s[i];
		s[i] = c;
	}
}

/*
 * Transform a value of the int type into a string.
 */ 
static char *itoa(size_t n, char *s)
{
	char *s_in;
	s_in = s;

	do
		/* make n positive and generate digits in reverse order */
		*s++ = (char)(n % 10) + '0';

	while ((n /= 10) != 0);

	*(s+1) = '\0';
	
	reverse(s_in);

	return s;
}

/*
 * write_frq_map: Write the frequency of each used characters repetition used
 * in the encoding of the file to the start of the file, so as to allow for the
 * recreation of the same Huffman tree for decompression.
 * TODO write a proper buffer here
 */
static void write_frq_map(Data **map, FILE *out)
{
	String *buf = NULL;
	char *num;
	size_t i, len;
	Data *cur;

	num = malloc(256);
	GE_string_init(buf);
	GE_string_concat(buf, "<map>\n", 6);

	//TODO NOW error here
	for (i = 0; i < MAP_LEN; i++) {
		if (map[i] != NULL) {
			GE_string_concat(buf, "\t", 1);
			len = utf8_len(map[i]->utf8_char);
			GE_string_concat(buf, map[i]->utf8_char, len);
			GE_string_concat(buf, "~", 1);
			num = itoa(map[i]->frq, num);
			GE_string_concat(buf, num, strlen(num));
			GE_string_concat(buf, "\n", 1);
			if ((cur = map[i]->next)) {
				while ((cur = cur->next)) {
					GE_string_concat(buf, "\t", 1);
					len = utf8_len(cur->utf8_char);
					GE_string_concat(buf, cur->utf8_char, len);
					GE_string_concat(buf, "~", 1);
					num = itoa(cur->frq, num);
					GE_string_concat(buf, num, strlen(num));
					GE_string_concat(buf, "\n", 1);
				}
			}
		}
	}

	GE_string_concat(buf, "</map>\n", 7);
	free(num);

	fwrite(buf, 1, buf->len, out);
}

/*
 * write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 */
static void write_bit(FILE *out, unsigned char bit, unsigned char *byte, int *count)
{
	if (++(*count) == 8) {
		fwrite(byte, 1, 1, out);
		*count = 0;
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
int compress_file(Data **map, FILE *in, FILE *out)
{
	int count;
	unsigned char byte;

	char c, *ptr, utf8_char[5], *bin;
	size_t len, i;
	ptr = utf8_char;

	byte = 0;
	count = 0;

	// TODO NOW called here
	write_frq_map(map, out);

	fwrite("<comp>\n\t", 1, 8, out);

	while ((c = fgetc(in)) != EOF)
	{
		/* Get char for the lenght of what is possibly a multi-byte
		 * character */
		while ((*ptr++ = c) && utf8_count(c))
			c = fgetc(in);

		*ptr = '\0';

		bin = map_read_char_to_binary(map, utf8_char);
		len = map_read_char_to_binary_len(map, utf8_char);
		for (i = 0; i < len; i++, bin++)
			write_bit(out, bin[0], &byte, &count);

		ptr = utf8_char;
	}

	/* Add EOF char */
	bin = map_read_char_to_binary(map, "EOF");
	len = map_read_char_to_binary_len(map, utf8_char);
	for (i = 0; i < len; i++, bin++)
		write_bit(out, bin[0], &byte, &count);

	/* Pad any remaining bits in the last byte with zeroes */
	if(count > 0) {
		 byte <<= 8 - count;
		 fwrite(&byte, 1, 1, out);
	}

	fwrite("\n</comp>\n", 1, 9, out);

	return 0;
}

/*
 * decompress_file: Read and then decompress compressed file. Analyze file
 * stream with lexer to decompress the file.
 */
int decompress_file(HC_HuffmanNode **list, FILE *in, FILE *out)
{
	LE_lexer_init();
	char c;

	while ((c = fgetc(in)) != EOF)
	{
		/* Read frequency map from file start */
		state |= LE_get_token(in, c);

		if (state & LE_MAP)
			build_priority_queue_from_file(list, in);
		else if (state & LE_DECOMP)
			continue;
		else
			break;
	}

	LE_lexer_free();

	return 0;
}

