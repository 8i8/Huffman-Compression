#include "HC_HuffmanTree.h"

HC_HuffmanTree **create_priority_cue(HC_HuffmanTree **list, FILE *fp);
HC_HuffmanTree **build_huffman_tree(HC_HuffmanTree **list);

void print_frequency(HC_HuffmanTree **list);

/* print_huffman_tree: Print out huffman tree */
void print_huffman_tree(HC_HuffmanTree *tree);

