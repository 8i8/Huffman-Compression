#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "HC_struct.h"
#include "HC_state.h"
#include "HC_map_char.h"
#include "HC_utf8.h"
#include "HC_priority_queue.h"
#include "LE_lexer.h"
#include "GE_hash.h"
#include "GE_string.h"

/*
 * write_frq_map: Write the frequency of each used characters repetition used
 * in the encoding of the file to the start of the file, so as to allow for the
 * recreation of the same Huffman tree for decompression.
 * TODO NOW the string is not getting correctly made.
 */
void write_frq_map(Data **map, FILE *out)
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
int decompress_file(HC_HuffmanNode **list, FILE *in, FILE *out, int state)
{
	LE_lexer_init();
	char c;

	while ((c = fgetc(in)) != EOF)
	{
		/* Read frequency map from file start */
		state = state_set(state, LE_get_token(in, c, state));

		if (state & LE_MAP)
			build_priority_queue_from_file(list, in);
		else if (state & LE_DECOMP)
			continue;
		else
			break;
	}

	LE_lexer_free();

	return state;
}

