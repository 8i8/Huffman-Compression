#include "huffman/HC_huffman_tree.h"

/* HC_map_init: Initialise array for char map */
Data *HC_map_init(Data *map);

/* hashmap_for_compression: Create char map from Huffman tree */
void hashmap_for_compression(Data *map, HC_HuffmanNode**tree, const int state);

/* HC_map_add_node: Add a new map node after hash collision */
int HC_map_add_node(Data *map, int bucket, Data new_node);

/* HC_map_add: Add data struct to binary data hashmap */
int HC_map_add(void *m, void *d);

/* HC_map_lookup_data: Returns binary value for char from the given map */
Data HC_map_lookup_data(Data *map, char *c);

/* HC_map_clear: Reset the map tp empty */
void HC_map_clear(Data *map);

/* HC_map_free: Free all memory used by the charatcer mapping */
void HC_map_free(Data *map);
