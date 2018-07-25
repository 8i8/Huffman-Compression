#include "data_structures/DS_huffman_node.h"

/* HC_map_init: Initialise array for char map */
Data **HC_map_init(void);

/* map_create: Create char map from Huffman tree */
Data **map_create(Data **map, HC_HuffmanNode**tree, const unsigned state);

/* HC_map_add: Add data struct to binary data hashmap */
int HC_map_add(void *m, void *d);

/* map_read_char_to_binary: Returns binary value for char from the given map */
char *map_read_char_to_binary(Data **map, char *c);

/* map_read_char_to_binary_len: Returns the length of the binary value for the given char */
size_t map_read_char_to_binary_len(Data **map, char *c);

/* print_char_map: Print out the char map made by the Huffman tree */
void print_char_map(Data **map);

/* HC_map_free: Free all memory used by the charatcer mapping */
void HC_map_free(Data **map);
