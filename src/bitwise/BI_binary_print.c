#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include "general/GE_error.h"
#include "general/GE_file_buffer.h"
#include "bitwise/BI_bitwise.h"

static F_Buf *buf = NULL;

/*
 * BI_buffer: Static buffer for logging use whilst debugging.
 */
static F_Buf *BI_buffer_init(void)
{
	static F_Buf b;
	buf = &b;

	GE_file_clear("binary_log");

	if (GE_file_open(buf, "binary_log", "a", 0))
		FAIL("File open failed");
	GE_buffer_on(buf);
	return buf;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Binary display
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * BI_int_to_binary: Returns the binary representation of 'i', with a word
 * length of 'bits'.
 */
char *BI_int_to_binary(size_t i, size_t bits)
{
	char *str;

	if (bits == 0)
		bits = sizeof(int) * CHAR_BIT;

	if ((str = malloc(bits + 1)) == NULL)
		return '\0';

	str[bits] = 0;

	for(; bits--; i >>= 1)
		str[bits] = i & 1 ? '1' : '0';

	return str;
}

/*
 * BI_binary_print: Display a binary representation of the given integer, with
 * a with of 'bits'.
 */
void BI_binary_print(size_t i, size_t bits)
{
	char *str;

	str = BI_int_to_binary(i, bits);

	printf("%s\n", str);
	free(str);
}

/*
 * BI_binary_log: Logs value of 'i' for a width of 'bits' in their binary form
 * to the file 'binary_log' in the root folder.
 */
F_Buf *BI_binary_log(size_t i, size_t bits)
{
	static int j;

	if (buf == NULL)
		BI_buffer_init();

	char *str;

	j %= 6;
	str = BI_int_to_binary(i, bits);
	GE_buffer_fwrite(str, bits, 1, buf);

	if (BIN_WRITE) BI_binary_print(*str, 8);

	if (j == 5)
		GE_buffer_fwrite("\n", 1, 1, buf);
	else
		GE_buffer_fwrite(" ", 1, 1, buf);

	j++;
	free(str);

	return buf;
}

/*
 * BI_binary_log_flush: Flush the remaining contents of the buffer, writing to
 * the F_Buf file pointer.
 */
int BI_binary_log_flush(void)
{
	GE_buffer_fwrite_FILE(buf);
	free(buf->buf);

	return 0;
}

