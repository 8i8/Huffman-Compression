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
	unsigned short st_dta;
	size_t frq;
	struct _data *next;
} Data;
#endif

/* GE_data_init: Returns a data struct with all values set to 0 */
Data GE_data_init(void);

/* GE_data_frqcmp: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int */
int GE_data_frqcmp(void *v1, void *v2);

/* GE_hashtable_init: Initialise array for char map */
Data *GE_hashtable_init(Data *map);

/* GE_hashtable_add_utf8_key: Add a new pair utf8 char key, binary value */
int GE_hashtable_add_utf8_key(Data *map, Data data);

/* GE_hashtable_add_binary_key: Add a new pair binary key, utf8 char value */
int GE_hashtable_add_binary_key(Data *map, Data data);

/* GE_map_add: Add data struct to binary data GE_hashmap */
int GE_map_add(void *m, void *d);

/* GE_hashtable_lookup_utf8: Returns the paired utf8 char value as a string,
 * for a given string key */
Data GE_hashtable_lookup_utf8(Data *map, char *str);

/* GE_hashtable_lookup_binary: Returns the paired binary string value of a
 * given utf8 char */
Data GE_hashtable_lookup_binary(Data *map, char *str);

/* GE_hashtable_clear: Reset the map to empty */
void GE_hashtable_clear(Data *map);

