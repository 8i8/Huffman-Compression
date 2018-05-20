/*
 * HC_HuffmanTree
 */
#include <stdio.h>
#include "HC_Struct.h"

/* HC_HuffmanTree_new_node: Internal function for creating new list nodes */
HC_HuffmanTree *HC_HuffmanTree_new_node(Data data);

/* HC_HuffmanTree_add: Create the next node in the list and add data */
HC_HuffmanTree **HC_HuffmanTree_add(HC_HuffmanTree **list, Data data);

/* HC_HuffmanTree_insert: Insert a new node at tyhe current location */
HC_HuffmanTree **HC_HuffmanTree_insert(HC_HuffmanTree **list, Data data);

/* HC_HuffmanTree_insert_huffman_node: Insert a new node at current location */
HC_HuffmanTree **HC_HuffmanTree_insert_huffman_node(HC_HuffmanTree **list, HC_HuffmanTree *new_node);

/* HC_HuffmanTree_insert_ordered: Insert a new node conditionaly */
HC_HuffmanTree **HC_HuffmanTree_insert_ordered(HC_HuffmanTree **list, HC_HuffmanTree *newList,
						int(*func)(void*, void*));

/* HC_HuffmanTree_pop: Pop the first node and return a pointer to the new list
 * address. */
HC_HuffmanTree *HC_HuffmanTree_pop(HC_HuffmanTree *list);

/* HC_HuffmanTree_walk: Walk tree and perform (*func) on every node */
HC_CharMap *HC_HuffmanTree_walk(HC_HuffmanTree **tree, void* store, int(*func)(void*, void*));

/* HC_HuffmanTree_free: Free huffman tree memory */
void HC_HuffmanTree_free(HC_HuffmanTree **tree);
