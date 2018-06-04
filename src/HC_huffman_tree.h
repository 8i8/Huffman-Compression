#include "HC_struct.h"

/* build_huffman_tree: Create a binary tree from the given linked list */
HC_HuffmanNode **build_huffman_tree(HC_HuffmanNode **list);

/* HC_huffman_tree_free: Free huffman tree memory */
int HC_huffman_tree_free(HC_HuffmanNode **tree);

/* print_huffman_tree: Print out huffman tree */
void print_huffman_tree(HC_HuffmanNode *tree);
