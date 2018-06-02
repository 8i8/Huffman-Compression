#include "HC_struct.h"

#define BUF_LEN 32
#define MAP_LEN 2097152 		/* 2^21 max number of Unicode char */

/* map_create: Create char map from Huffman tree */
Data *map_create(Data *map, HC_HuffmanNode**tree);

/* print out char map list */
void print_char_map(Data *map);

/* map_read_char_to_binary: Returns binary value for char from the given map */
char *map_read_char_to_binary(Data *map, char *c);

/* map_read_char_to_binary_len: Returns the length of the binary value for the given char */
size_t map_read_char_to_binary_len(Data *map, char *c);
