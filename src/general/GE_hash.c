#include "huffman/HC_hashtable.h"

/*
 * hash: Hash value for string s
 */
unsigned long hash(char str)
{
	unsigned long hashval;
	char *s = &str;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 3 * hashval;

	return hashval % MAP_LEN;
}

