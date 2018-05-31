#include "HC_Struct.h"

#define BUF_LEN 32
#define MAP_LEN 2097152 		/* 2^21 max number of Unicode char */

/* create_char_map: Create char map from Huffman tree */
Data *create_char_map(Data *map, HC_HuffmanNode**tree);

/* print out char map list */
void print_char_map(Data *map);
