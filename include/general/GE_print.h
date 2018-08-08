#include "huffman/HC_huffman_tree.h"

/*
 * print_frequency_map: Output the frequency map, if any treenodes exist print
 * those also, includes a verification for nodes joint by the left right tree
 * connections.
 */
void print_frequency_map(HC_HuffmanNode *list);

/*
 * print_huffman_tree: Print out huffman tree.
 */
void print_huffman_tree(HC_HuffmanNode *tree);

/*
 * print_char_map: Print out the char map made by the Huffman tree.
 */
void print_char_map(Data *map);
