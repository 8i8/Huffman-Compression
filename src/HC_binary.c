#include <stdio.h>
#include "HC_Struct.h"
#include "HC_char_map.h"

/* how many bits in current byte */
int bit_counter;
/* current byte */
unsigned char cur_byte;

/* write 1 or 0 bit */
void _write_bit(FILE *out, unsigned char bit)
{
	if(++bit_counter == 8) {
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
int write_to_file(Data *map, FILE *in, char *file_out)
{
	FILE* out;
	int c;

	out = fopen(file_out, "w");

	cur_byte = 0;
	bit_counter = 0;

	while ((c = fgetc(in)) != EOF)
		_write_bit(out, map_to_binary(map, c);

	if(bit_counter > 0) {
		 /* pad the last byte with zeroes */
		 cur_byte <<= 8 - bit_counter;
		 fwrite(&cur_byte, 1, 1, out);
	}

	fclose(out);

	return 0;
}

