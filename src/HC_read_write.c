#include <stdio.h>
#include "HC_struct.h"
#include "HC_map_char.h"
#include "HC_utf8.h"

//void _write_map(Data *map, FILE *out)
//{
//}

/*
 * _write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 */
void _write_bit(FILE *out, unsigned char bit, unsigned char *byte, int *count)
{
	if (++(*count) == 8) {
		fwrite(byte, 1, 1, out);
		*count = 0;
		*byte = 0;
	}

	/* Shift left ready for the next bit */
	*byte <<= 1;
	/* Set first bit to 1 or 0 */
	*byte |= bit;
}

/*
 * compress_file: Write compressed file.
 */
int compress_file(Data *map, FILE *in, FILE *out)
{
	int count;
	unsigned char byte;

	char c, *ptr, str[5], *bin;
	size_t len, i;
	ptr = str;

	byte = 0;
	count = 0;

	while ((c = fgetc(in)) != EOF)
	{
		/* Get char for the lenght of what is possibly a multi-byte
		 * character */
		while (utf8_test_count(c) && (*ptr++ = c))
			c = fgetc(in);
		*ptr++ = c;
		*ptr = '\0';

		bin = map_read_char_to_binary(map, str);
		len = map_read_char_to_binary_len(map, str);
		for (i = 0; i < len; i++, bin++)
			_write_bit(out, bin[0], &byte, &count);

		ptr = str;
	}

	/* Add EOF char */
	bin = map_read_char_to_binary(map, "EOF");
	len = map_read_char_to_binary_len(map, str);
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

