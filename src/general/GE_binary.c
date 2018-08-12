#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Binary display
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static uint64_t word = 64;
static unsigned char *ptr, *stack;

static void write_next_bit(uint64_t i)
{
	/* add ascii representation of int, by adding 48 or '0' */
	*ptr++ = i + '0';
	*ptr = '\0';
}

/*
 * print_binary: Print the binary representation of an integer.
 */
static uint64_t print_binary(uint64_t i, uint64_t word)
{
	/* one bit, a 1 or a 0 */
	if (i < 2)
		write_next_bit(i);

	else if (i > 1)
	{
		/* dimishing by powers of 2, if wordlength is not 0 reduce by 1
		 * and call again print binary */
		word = print_binary(i/2, (word) ? word - 1 : 0);

		/* 1 or 2 */
		write_next_bit(i % 2);
	}

	return word;
}

/*
 * print_binary_of_int: Displays a binary representation of the given integer,
 * the width argument gives the number of bits to be displayed after the most
 * significant bit, including that which denotes the requested integer.
 */
void print_binary_of_int(uint64_t i, uint64_t width)
{
	/* make store for output */
	stack = ptr = malloc(word + 1);

	width = print_binary(i, width);

	/* fill remaining space with 0's */
	if (width > 1)
		while (width)
			putchar('0'), width--;

	/* output */
	printf("%s\n", stack);
	free(stack);
} 

