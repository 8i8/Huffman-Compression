#include <stdio.h>
#include <string.h>
#include "HC_struct.h"
#include "HC_map_char.h"
#include "HC_utf8.h"

static void _write_frq_map(Data *map, FILE *out)
{
	char buf[4000];
	char temp[4000];

	memcpy(buf, "<in>\0", 5);

	while (map++) {
		sprintf(temp, "%s~%lu~", map->multi_byte_char, map->frq);
		strcat(buf, temp);
	}

	sprintf(temp, "<out>");
	strcat(buf, temp);

	fwrite(buf, 1, strlen(buf), out);
}

/*
 * _write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 */
static void _write_bit(FILE *out, unsigned char bit, unsigned char *byte, int *count)
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
int compress_file(Data *map, FILE *in, FILE *out)
{
	int count;
	unsigned char byte;

	char c, *ptr, multi_byte_char[5], *bin;
	size_t len, i;
	ptr = multi_byte_char;

	byte = 0;
	count = 0;

	//_write_frq_map(map, out);

	while ((c = fgetc(in)) != EOF)
	{
		/* Get char for the lenght of what is possibly a multi-byte
		 * character */
		while (utf8_test_count(c) && (*ptr++ = c))
			c = fgetc(in);
		*ptr++ = c;
		*ptr = '\0';

		bin = map_read_char_to_binary(map, multi_byte_char);
		len = map_read_char_to_binary_len(map, multi_byte_char);
		for (i = 0; i < len; i++, bin++)
			_write_bit(out, bin[0], &byte, &count);

		ptr = multi_byte_char;
	}

	/* Add EOF char */
	bin = map_read_char_to_binary(map, "EOF");
	len = map_read_char_to_binary_len(map, multi_byte_char);
	for (i = 0; i < len; i++, bin++)
		_write_bit(out, bin[0], &byte, &count);

	/* Pad any remaining bits in the last byte with zeroes */
	if(count > 0) {
		 byte <<= 8 - count;
		 fwrite(&byte, 1, 1, out);
	}

	return 0;
}

/*
 * decompress_file: Read compressed file.
 */
//int decompress_file(Data *map, FILE *in, FILE *out)
//{
//}

