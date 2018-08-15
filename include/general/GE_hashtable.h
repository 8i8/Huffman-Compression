#include <stdio.h>

#define MAP_LEN 256
#define UTF8_LEN 5
#define BIN_MAX 128	/* Max depth of ordered binary tree, binary string */

#ifndef _data_
#define _data_
typedef struct _data {
	char utf8_char[UTF8_LEN];
	char binary[BIN_MAX];
	unsigned short len_char;
	unsigned short len_bin;
	size_t frq;
	struct _data *next;
} Data;
#endif

/* HC_data_init: Returns a data struct with all values set to 0 */
Data HC_data_init(void);

/* FN_data_frqcmp: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int */
int FN_data_frqcmp(void *v1, void *v2);

/* HC_hashtable_init: Initialise array for char map */
Data *HC_hashtable_init(Data *map);

/* HC_hashtable_add_utf8_key: Add a new pair utf8 char key, binary value */
int HC_hashtable_add_utf8_key(Data *map, Data data);

/* HC_hashtable_add_binary_key: Add a new pair binary key, utf8 char value */
int HC_hashtable_add_binary_key(Data *map, Data data);

/* HC_map_add: Add data struct to binary data hashmap */
int HC_map_add(void *m, void *d);

/* HC_hashtable_lookup_utf8: Returns the paired utf8 char value as a string,
 * for a given string key */
Data HC_hashtable_lookup_utf8(Data *map, char *str);

/* HC_hashtable_lookup_binary: Returns the paired binary string value of a
 * given utf8 char */
Data HC_hashtable_lookup_binary(Data *map, char *str);

/* HC_hashtable_clear: Reset the map to empty */
void HC_hashtable_clear(Data *map);

