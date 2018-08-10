#include "huffman/HC_huffman_tree.h"

#define MAP_LEN 256

//TODO NEXT change the nomenclature from hashmap to hash table.

/* HC_hashtable_init: Initialise array for char map */
Data *HC_hashtable_init(Data *map);

/*
 * HC_hashtable_add_utf8_key: Add a new pair utf8 char key, binary value.
 */
int HC_hashtable_add_utf8_key(Data *map, char utf8_char, char binary);

/*
 * HC_hashtable_add_binary_key: Add a new pair binary key, utf8 char value.
 */
int HC_hashtable_add_binary_key(Data *map, char binary, char utf8_char);

/* HC_map_add: Add data struct to binary data hashmap */
int HC_map_add(void *m, void *d);

/*
 * HC_hashtable_lookup_string: Returns the paired string value for a given string key.
 */
Data HC_hashtable_lookup_string(Data *map, char str);

/* HC_hashtable_clear: Reset the map to empty */
void HC_hashtable_clear(Data *map);

