#include "general/GE_file_buffer.h"
#include "general/GE_string.h"
#include <stdio.h>

#define UTF8_LEN 5
#define BIN_MAX 128	/* Max depth of ordered binary tree, binary string */

#ifndef _data_
#define _data_
typedef struct _data {
	char utf8_char[UTF8_LEN];
	char binary[BIN_MAX];
	unsigned short len_char;
	unsigned short len_bin;
	size_t frq;
	struct _data *next;
} Data;
#endif

#ifndef _hc_huffman_node_
#define _hc_huffman_node_
typedef struct _hc_huffman_node {
	Data data;
	char bit;
	struct _hc_huffman_node *next;
	struct _hc_huffman_node *right;
	struct _hc_huffman_node *left;
} HC_HuffmanNode;
#endif

/* DS_huffman_data_init: Initalize an empty Data struct */
//Data *DS_huffman_data_init(Data *map);

/* HC_data_init: Returns a data struct with all values set to 0 */
Data HC_data_init(void);

/* FN_data_frqcmp: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int */
int FN_data_frqcmp(void *v1, void *v2);

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

