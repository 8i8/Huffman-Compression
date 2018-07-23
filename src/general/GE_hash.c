#include "general/GE_hash.h"

/*
 * hash: Hash value for string s
 */
unsigned long hash(char *s)
{
	unsigned long hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 3 * hashval;

	return hashval % MAP_LEN;
}

