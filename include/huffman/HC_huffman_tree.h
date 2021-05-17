#ifndef HC_HUFFMAN_NODE
#define HC_HUFFMAN_NODE

#include "general/GE_file_buffer.h"
#include "general/GE_string.h"
#include "general/GE_hashtable.h"
#include <stdio.h>

typedef struct hc_huffman_node {
	Data data;
	char bit;
	struct hc_huffman_node *next;
	struct hc_huffman_node *right;
	struct hc_huffman_node *left;
} HC_HuffmanNode;

/* DS_huffman_tree_new_node: Function for creating new linkedlist type nodes */
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

/* DS_huffman_tree_insert_or_count: If the char does not yet exist in the char map,
 * insert it in alphabetical order. If it exists already, add one to the count
 * for that character */
HC_HuffmanNode **DS_huffman_tree_insert_or_count(
						HC_HuffmanNode **list,
						Data data,
						int(*func)(void*, void*));

/* DS_huffman_tree_pop: Pop the first node and return a pointer to the new list
 * address. */
HC_HuffmanNode *DS_huffman_tree_pop(HC_HuffmanNode **list);

/* ordered_binary_tree: Create a binary tree from the given linked list */
HC_HuffmanNode **ordered_binary_tree(HC_HuffmanNode **list, const int state);

/* DS_huffman_tree_extract_encoding: Recursive function to walk tree and retrieve the
 * binary data */
int DS_huffman_tree_extract_encoding(
						HC_HuffmanNode *tree,
						String *string,
						Data *map);

/* DS_huffman_tree_clear: Clear tree before reuse */
int DS_huffman_tree_clear(HC_HuffmanNode **tree);

/* DS_huffman_tree_free: Free huffman tree memory */
int DS_huffman_tree_free(HC_HuffmanNode **tree);

#endif
