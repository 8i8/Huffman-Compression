#include "data_structures/DS_huffman_tree.h"

/* HC_map_init: Initialise array for char map */
Data **HC_map_init(void);

/* hashmap_binary_out: Create char map from Huffman tree */
Data **hashmap_binary_out(Data **map, HC_HuffmanNode**tree, const int state);

/* HC_map_add: Add data struct to binary data hashmap */
int HC_map_add(void *m, void *d);

/* HC_map_lookup_data: Returns binary value for char from the given map */
Data *HC_map_lookup_data(Data **map, char *c);

/* HC_map_free: Free all memory used by the charatcer mapping */
void HC_map_free(Data **map);
