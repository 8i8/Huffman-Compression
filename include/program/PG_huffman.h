#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_hashtable.h"

/*
 * huffman: Run main program code.
 */
int huffman(F_Buf **io, HC_HuffmanNode **tree, int st_prg);

/*
 * hashmap_for_compression: Create char map from Huffman tree.
 */
void hashmap_for_compression(Data *map, HC_HuffmanNode**tree, const int state);

