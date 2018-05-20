#include "HC_HuffmanTree.h"

HC_HuffmanTree **create_priority_cue(HC_HuffmanTree **list, FILE *fp);
HC_HuffmanTree **build_huffman_tree(HC_HuffmanTree **list);

/* create_char_map: Create char map from Huffman tree */
HC_CharMap *create_char_map(HC_CharMap *map, HC_HuffmanTree **tree);

void print_frequency(HC_HuffmanTree **list);

