#include <bitwise/BI_bitwise.h>
#include <stdio.h>
//#include <math.h>

/*
 * get_bits: Returns the right adjusted n-bit field of x that begins at
 * position p, position 0 is the least significant bit of the word.
 */
long unsigned get_bits(long unsigned x, long unsigned p, long unsigned n)
{
	return (x >> (p+1-n)) & ~(~(long unsigned)0 << n);
}

/*
 */
//void print_bits(unsigned char v)
//{
//	printf("%*s", (int)ceil(log2(v)) + 1, ""); 
//	for (; v; v >>= 1)
//		printf("\x1b[2D%c",'0' + (v & 1));
//	putchar('\n');
//}

/*
 * mask: returns a mask of n bits from the right, all set to 1; used to mask
 * off the left bits introduced by a right shift.
 */
long unsigned mask_left_of_word(long unsigned word, long unsigned n)
{
	long unsigned mask = 0;
	mask = (1 << n) -1;

	BI_binary_print(mask, 24);

	return mask & word;
}

/*
 * right_shift: Right shift a word n bits, then mask off that word from the new
 * left hand bits that were introduced and that are implimentation defined.
 */
long unsigned right_shift(long unsigned word, long unsigned bits, long unsigned mask)
{
	word >>= bits;
	word = mask_left_of_word(word, mask);

	return word;
}

