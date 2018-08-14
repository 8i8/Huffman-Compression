#include <stdio.h>
#include "general/GE_file_buffer.h"
#include "bitwise/BI_bitwise.h"

#define BIN_IN  1
#define BIN_OUT 1

/*
 * write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
  * TODO NOW binary write
 */
void BI_write_bit(
						F_Buf *buf,
						unsigned char bit,
						unsigned char *byte,
						int *bit_count)
{
	if (BIN_IN) BI_binary_print(bit, 8);

	if (++(*bit_count) == 8) {
		if (BIN_IN) BI_binary_print((unsigned long) *byte, 8);
		GE_buffer_fwrite((char*)byte, 1, 1, buf);
		*bit_count = 0;
	}

	/* Shift left ready for the next bit */
	*byte <<= 1;

	/* Set bit to 1 or 0 */
	*byte |= bit;

	if (BIN_IN) BI_binary_print(*byte, 8);
}

 /* 
  * BI_read_bit: Read each bit of a char, checking each time to see if the
  * binary string that is being created is in the hash table, if it is found;
  * Write the corresponding char into the output file buffer and then refresh
  * the string.
  * TODO NOW binary read
  */
char BI_read_bit(
						F_Buf *buf_read,
						F_Buf *buf_write,
						Data *map,
						String *str,
						char c,
						unsigned char *byte,
						int *bit_count)
{
	unsigned bit;
	Data data;
	bit = 0;

	/* Reset byte */
	if (++(*bit_count) == 8) {
		c = *byte = GE_buffer_fgetc(buf_read);
		*bit_count = 0;
	}

	/* Read for a bit, was that a type pun? Add the bit to the string and
	 * then shift the byte left fot the next read */
	if (BIN_OUT) BI_binary_print(*byte, 8);
	bit = *byte & 1;
	if (BIN_OUT) BI_binary_print(bit, 8);
	GE_string_add_char(str, (char)bit+'0');
	if (BIN_OUT) printf("%s\n", str->str);
	*byte >>= 1;

	/* write to file when ready */
	data = HC_hashtable_lookup_binary(map, *str->str);
	if (data.utf8_char[0] != '\0') {
		if (BIN_OUT) BI_binary_print(*byte, 8);
		GE_buffer_fwrite(
						data.utf8_char, 1,
						data.len_char, buf_write);
		GE_string_reset(str);
	}

	return c;
}

