#include "huffman/HC_huffman_tree.h"

#define MAP_LEN 256

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
Data HC_hashtable_lookup_utf8(Data *map, char str);

/* HC_hashtable_lookup_binary: Returns the paired binary string value of a
 * given utf8 char */
Data HC_hashtable_lookup_binary(Data *map, char str);

/* HC_hashtable_clear: Reset the map to empty */
void HC_hashtable_clear(Data *map);

