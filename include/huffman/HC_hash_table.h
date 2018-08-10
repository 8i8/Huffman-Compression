#include "huffman/HC_huffman_tree.h"

#define MAP_LEN 256

//TODO NEXT change the nomenclature from hashmap to hash table.

/* HC_hash_table_init: Initialise array for char map */
Data *HC_hash_table_init(Data *map);

/* HC_hash_table_add_value: Add a new map node after hash collision */
int HC_hash_table_add_value(Data *map, int bucket, Data new_node);

/* HC_map_add: Add data struct to binary data hashmap */
int HC_map_add(void *m, void *d);

/*
 * HC_hash_table_lookup_string: Returns the paired string value for a given string key.
 */
Data HC_hash_table_lookup_string(Data *map, char *str);

/* HC_hash_table_clear: Reset the map to empty */
void HC_hash_table_clear(Data *map);

