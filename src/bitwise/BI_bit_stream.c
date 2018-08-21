#include <stdio.h>
#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "bitwise/BI_bitwise.h"

/*
 * write_bit: Set all of the bits in a byte, then write that byte to the given
 * file.
 */
void BI_write_bit(
						F_Buf *buf,
						unsigned char bit,
						unsigned char *byte,
						int *bit_count)
{
	/* Shift left ready for the next bit */
	*byte <<= 1;

	/* Set bit to 1 or 0 */
	*byte |= (int)bit;

	if (BIN_IN)
		printf("%d ", *bit_count);
	if (BIN_IN)
		BI_binary_print(*byte, 8);
	
	if ((*bit_count)++ == 7)
	{
		if (BIN_IN)
			BI_binary_print(*byte, 8);
		if (BIN_LOG_IN)
			BI_binary_log((size_t)*byte, 8);
		GE_buffer_fwrite((char*)byte, 1, 1, buf);
		*bit_count = 0;
	}
}

 /* 
  * BI_read_bit: Read each bit of a char, checking each time to see if the
  * binary string that is being created is in the hash table, if it is found;
  * Write the corresponding char into the output file buffer and then refresh
  * the string.
  */
char BI_read_bit(
						F_Buf *buf_read,
						F_Buf *buf_write,
						Data *map,
						String *str,
						char c,
						unsigned char *byte,
						int *bit_count,
						int *ignore,
						int *st_lex)
{
	unsigned bit;
	Data data;
	bit = 0;

	/* Read for a bit, was that a type pun? Add the bit to the string and
	 * then shift the byte left fot the next read */
	bit = *byte & 128;
	bit >>= 7;

	GE_string_add_char(str, (char)bit+'0');
	*byte <<= 1;
	if (BIN_OUT)
		printf("%s\n", str->str);

	/* Keep write to the string untill it returns a char from the hash table */
	data = GE_hashtable_lookup_binary(map, str->str);
	if (data.utf8_char[0] != '\0') {
		if (BIN_OUT)
			printf("%s\n", str->str);
		if (data.utf8_char[0] == 'E') // TODO set memory record here
			if (is_set(data.st_dta, DTA_EOF)) {
				GE_string_reset(str);
				state_set(*st_lex, LEX_EOF);
				return state_unset(*st_lex, LEX_DECOMPRESS);
			}
		GE_buffer_fwrite(
						data.utf8_char, 1,
						data.len_char, buf_write);
		if (BIN_OUT)
			printf(">>> '%s'\n", data.utf8_char);
		GE_string_reset(str);
	}

	/* Reset byte */
	if ((*bit_count)++ == 7) {
		c = *byte = GE_buffer_fgetc(buf_read);
		*ignore = *bit_count = 0;
	}

	return c;
}

