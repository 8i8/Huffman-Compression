#include "data_structures/DS_huffman_node.h"
#include "general/GE_file_buffer.h"
#include "general/GE_string.h"

/* DS_huffman_tree_new_node: Internal function for creating new list nodes */
HC_HuffmanNode *DS_huffman_tree_new_node(Data data);

/* DS_huffman_tree_add: Create the next node in the list and add data */
HC_HuffmanNode **DS_huffman_tree_add(HC_HuffmanNode **list, Data data);

/* DS_huffman_tree_insert: Insert a new node at tyhe current location */
HC_HuffmanNode **DS_huffman_tree_insert(HC_HuffmanNode **list, Data data);

/* DS_huffman_tree_insert_ordered: Insert a new node conditionaly */
HC_HuffmanNode **DS_huffman_tree_insert_ordered(
						HC_HuffmanNode **list,
						HC_HuffmanNode*newList,
						int(*func)(void*, void*));

/* DS_huffman_tree_pop: Pop the first node and return a pointer to the new list
 * address. */
HC_HuffmanNode *DS_huffman_tree_pop(HC_HuffmanNode **list);

/* build_ordered_binary_tree: Create a binary tree from the given linked list */
HC_HuffmanNode **build_ordered_binary_tree(HC_HuffmanNode **list, const unsigned state);

/* DS_huffman_tree_extract_encoding: Recursive function to walk tree and retrieve the
 * binary data */
Data **DS_huffman_tree_extract_encoding(
						HC_HuffmanNode *tree,
						String* string,
						Data** map);

/* DS_huffman_tree_free: Free huffman tree memory */
int DS_huffman_tree_free(HC_HuffmanNode **tree);

/* print_huffman_tree: Print out huffman tree */
void print_huffman_tree(HC_HuffmanNode *tree);

