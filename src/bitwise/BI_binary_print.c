#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Binary display
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * BI_binary_print: Display a binary representation of the given integer, with
 * a with of 'bits'.
 * TODO NOW write the bits to a buffer and then to a file (stdout ?) I am not
 * able to debug the binary in out ans I do not know what the value is that I
 * am trying to read on the other end. This binary write exercise should help
 * me to achieve this.
 */
void BI_binary_print(int i, size_t bits)
{
	if (bits == 0)
		bits = sizeof(int) * CHAR_BIT;

	char *str = malloc(bits + 1);
	if(!str)
		return;

	str[bits] = 0;

	// type punning because signed shift is implementation-defined
	unsigned u = *(unsigned *)&i;
	for(; bits--; u >>= 1)
		str[bits] = u & 1 ? '1' : '0';

	printf("%s\n", str);
	free(str);
}
