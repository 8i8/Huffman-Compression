#include <stdlib.h>
#include "huffman/HC_hashtable.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   bitwise test
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* get_bits: Returns the right adjusted n-bit field of x that begins at
 * position p, position 0 is the least significant bit of the word */
long unsigned get_bits(long unsigned x, long unsigned p, long unsigned n);

void print_bits(unsigned char v);

/* mask: returns a mask of n bits from the right, all set to 1; used to mask
 * off the left bits introduced by a right shift */
long unsigned mask(long unsigned base, long unsigned n);

/* right_shift: Right shift a word n bits, then mask off that word from the new
 * left hand bits that were introduced and that are implimentation defined */
long unsigned right_shift(long unsigned word, long unsigned bits, long unsigned mask);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  binary printout
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* BI_binary_print: Display a binary representation of the given integer, with
 * a with of 'bits' */
void BI_binary_print(int i, size_t bits);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  bit_stream
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* write_bit: Set all of the bits in a byte, then write that byte to the given
 * file */
void BI_write_bit(
						F_Buf *buf,
						unsigned char bit,
						unsigned char *byte,
						int *bit_count);

 /* BI_read_bit: Read each bit of a char, checking each time to see if the
  * binary string that is being created is in the hash table, if it is found;
  * Write the corresponding char into the output file buffer and then refresh
  * the string */
char BI_read_bit(
						F_Buf *buf_read,
						F_Buf *buf_write,
						Data *map,
						String *str,
						char c,
						unsigned char *byte,
						int *bit_count);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  bit_stream
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ipow: Bitwise power function */
int ipow(int base, int exp);
