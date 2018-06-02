#include <stdio.h>
#include "HC_struct.h"
#include "HC_map_char.h"
#include "HC_utf8.h"

/* write 1 or 0 bit */
void _write_bit(FILE *out, unsigned char bit, unsigned char byte, int count)
{
	if (++count == 8) {
		fwrite(&byte, 1, 1, out);
		count = 0;
		byte = 0;
	}

	/* Shift left ready for the next bit */
	byte <<= 1;
	/* Set first bit to 1 or 0 */
	byte |= bit;
}

/*
 * write_binary_to_file: Write compressed file.
 */
int write_binary_to_file(Data *map, FILE *in, FILE *out)
{
	unsigned char byte;	/* current byte */
	int count;		/* how many bits in current byte */
	char c, *ptr, str[5], *bin;
	size_t len, i;
	ptr = str;

	byte = 0;
	count = 0;

	while ((c = fgetc(in)) != EOF)
	{
		/* Get char for the lenght of the multi-byte character */
		while (utf8_test_count(c) && (*ptr++ = c))
				c = fgetc(in);
		*ptr++ = c;
		*ptr = '\0';

		bin = map_read_char_to_binary(map, str);
		len = map_read_char_to_binary_len(map, str);
		for (i = 0; i < len; i++, bin++)
			_write_bit(out, bin[0], byte, count);

		ptr = str;
	}

	/* Add EOF char */
	bin = map_read_char_to_binary(map, "EOF");
	len = map_read_char_to_binary_len(map, str);
	for (i = 0; i < len; i++, bin++)
		_write_bit(out, bin[0], byte, count);

	if(count > 0) {
		 /* pad the last byte with zeroes */
		 byte <<= 8 - count;
		 fwrite(&byte, 1, 1, out);
	}

	return 0;
}

