#include "huffman/HC_huffman_node.h"
#include <string.h>

/*
 * FN_data_strcmp: Compare Data one and two, the value should be a single char and
 * the result alphabetical order as per the ASCII char numbering system.
 */
int FN_data_strcmp(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	return strcmp(d1->utf8_char, d2->utf8_char);
}

/*
 * FN_data_frqcmp: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int.
 */
int FN_data_frqcmp(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	return (int)(d1->frq - d2->frq);
}

