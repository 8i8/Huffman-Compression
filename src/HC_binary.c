#include <stdio.h>
#include "HC_Struct.h"
#include "HC_char_map.h"
#include "HC_utf8.h"

/* how many bits in current byte */
int bit_counter;
/* current byte */
unsigned char cur_byte;

/* write 1 or 0 bit */
void _write_bit(FILE *out, unsigned char bit)
{
	if (++bit_counter == 8) {
		fwrite(&cur_byte, 1, 1, out);
		bit_counter = 0;
		cur_byte = 0;
	}

	/* Shift left ready for the next bit */
	cur_byte <<= 1;
	/* Set first bit to 1 or 0 */
	cur_byte |= bit;
}

/*
 * write_to_file: Write compressed file.
 */
int write_to_file(Data *map, FILE *in, FILE *out)
{
	char c, *ptr, str[5], *bin;
	size_t len, i;
	ptr = str;

	cur_byte = 0;
	bit_counter = 0;

	while ((c = fgetc(in)) != EOF) {

		/* Get char for the lenght of the multi-byte character */
		while (utf8_test_count(c) && (*ptr++ = c))
				c = fgetc(in);
		*ptr++ = c;
		*ptr = '\0';

		bin = map_to_binary(map, str);
		len = map_binary_len(map, str);
		for (i = 0; i < len; i++, bin++)
			_write_bit(out, bin[0]);

		ptr = str;
	}

	/* Add EOF char */
	bin = map_to_binary(map, "EOF");
	len = map_binary_len(map, str);
	for (i = 0; i < len; i++, bin++)
		_write_bit(out, bin[0]);

	if(bit_counter > 0) {
		 /* pad the last byte with zeroes */
		 cur_byte <<= 8 - bit_counter;
		 fwrite(&cur_byte, 1, 1, out);
	}

	return 0;
}

