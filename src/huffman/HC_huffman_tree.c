#include <stdlib.h>
#include <string.h>
#include "general/GE_error.h"
#include "general/GE_state.h"
#include "general/GE_string.h"
#include "general/GE_hash.h"
#include "program/PG_print.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_hashtable.h"

/*
 * DS_huffman_data_init: Initalize an empty Data struct.
 */
//Data *DS_huffman_data_init(Data *map)
//{
//	map->utf8_char[0] = '\0';
//	map->string[0] = '\0';
//	map->len = 0;
//	map->frq = 0;
//	map->next = NULL;
//	return map;
//}

/*
 * HC_data_init: Returns a data struct with all values set to 0.
 */
Data HC_data_init(void)
{
	Data map;
	map.utf8_char[0] = '\0';
	map.binary[0] = '\0';
	map.len_char = 0;
	map.len_bin = 0;
	map.frq = 0;
	map.next = NULL;
	return map;
}

/*
 * FN_data_frqcmp: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int.
 */
int FN_data_frqcmp(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	return (int)(d1->frq - d2->frq);
}


/*
 * DS_huffman_tree_new_node: Function for creating new linkedlist type nodes.
 */
HC_HuffmanNode *DS_huffman_tree_new_node(Data data)
{
	HC_HuffmanNode *new_node = NULL;
	if ((new_node = malloc(sizeof(HC_HuffmanNode))) == NULL) {
		FAIL("memory allocation failed");
		return NULL;
	}

	new_node->next = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->data = data;
	new_node->bit = '\0';

	return new_node;
}

/*
 * DS_huffman_tree_add: Create the next node in the list and add data.
 */
HC_HuffmanNode **DS_huffman_tree_add(HC_HuffmanNode **list, Data data)
{
	if (*list == NULL)
		*list = DS_huffman_tree_new_node(data);
	else {
		while ((*list)->next != NULL)
			list = &(*list)->next;

		if (((*list)->next = DS_huffman_tree_new_node(data)) == NULL) {
			FAIL("memory allocation failed");
			return NULL;
		}

		list = &(*list)->next;
		(*list)->data = data;
		(*list)->next = NULL;
	}

	return list;
}

/*
 * DS_huffman_tree_insert: Insert a new node at current location.
 */
HC_HuffmanNode **DS_huffman_tree_insert(HC_HuffmanNode** list, Data data)
{
	HC_HuffmanNode *new;

	if (list == NULL || *list == NULL) {
		FAIL("NULL pointer recieved");
		return NULL;
	}

	if ((new = DS_huffman_tree_new_node(data)) == NULL) {
		FAIL("NULL pointer returned");
		return NULL;
	}

	new->next = *list;
	*list = new;

	return list;
}

/*
 * DS_huffman_tree_insert_node: Insert a new node at current location.
 */
HC_HuffmanNode **DS_huffman_tree_insert_node(
						HC_HuffmanNode **list,
						HC_HuffmanNode *new_node)
{
	if (list == NULL || *list == NULL || new_node == NULL) {
		FAIL("NULL pointer recieved");
		return NULL;
	}

	new_node->next = *list;
	*list = new_node;

	return list;
}

/*
 * DS_huffman_tree_insert_ordered: Insert a new node conditionaly.
 */
HC_HuffmanNode **DS_huffman_tree_insert_ordered(
						HC_HuffmanNode **list,
						HC_HuffmanNode *new,
						int(*freq)(void*, void*))
{
	HC_HuffmanNode *start;
	start = *list;

	if (new == NULL) {
		FAIL("NULL pointer recieved");
		return NULL;
	}

	if (*list == NULL) {
		*list = new;
		return list;
	}

	if ((freq((void*)new, (void*)*list)) <= 0)
		start = new;
	else
		start = *list;

	while (*list && (*list)->next && (freq((void*)new, (void*)(*list)->next) > 0))
		list = &(*list)->next;

	new->next = (*list)->next;
	(*list)->next = new;
	list = &start;
	
	return list;
}

/*
 * DS_huffman_tree_insert_or_count: If the char does not yet exist in the char map,
 * insert it in alphabetical order. If it exists already, add one to the count
 * for that character.
 */
HC_HuffmanNode **DS_huffman_tree_insert_or_count(
						HC_HuffmanNode **list,
						Data data,
						int(*func)(void*, void*))
{
	int test;
	HC_HuffmanNode *rtn;

	if (list == NULL) {
		FAIL("NULL pointer recieved");
		return NULL;
	} else if (*list == NULL) {
		*list = DS_huffman_tree_new_node(data);
		return list;
	}

	rtn = *list;

	while (*list != NULL)
	{
		if ((test = (*func)((void*)&data, (void*)&(*list)->data)) < 0) {
			if ((DS_huffman_tree_insert(list, data)) == NULL) {
				FAIL("NULL pointer returned");
				return NULL;
			}
			break;
		} else if (test == 0) {
			(*list)->data.frq++;
			break;
		} else if (test > 0)
			list = &(*list)->next;
		if (*list == NULL) {
			DS_huffman_tree_add(list, data);
			break;
		}
	}

	list = &rtn;

	return list;
}

/*
 * DS_huffman_tree_pop: Pop the first node and return a pointer to the next node
 * of NULL if it does not exist.
 */
HC_HuffmanNode *DS_huffman_tree_pop(HC_HuffmanNode **list)
{
	HC_HuffmanNode *popped;

	if (list == NULL) {
		FAIL("NULL pointer recieved");
		return NULL;
	}

	/* Remove the node */
	popped = *list;
	*list = (*list)->next;

	return popped;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman tree
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * ordered_binary_tree: Create an ordered binary tree from the given
 * linkedlist.
 */
HC_HuffmanNode **ordered_binary_tree(HC_HuffmanNode **tree, const int st_prg)
{
	HC_HuffmanNode *new, *one, *two;
	Data data;
	data = HC_data_init();

	if (is_set(st_prg, VERBOSE))
		printf("metadata: buildiing binary tree.\n");

	while ((*tree)->next)
	{
		one = DS_huffman_tree_pop(tree);
		two = DS_huffman_tree_pop(tree);

		one->next = NULL, two->next = NULL;
		data.next = NULL;
		data.frq = one->data.frq + two->data.frq;

		/* Add leaves to new node and give a binary value */
		if ((new = DS_huffman_tree_new_node(data)) == NULL)
			FAIL("NULL pointer returned");

		/* Begin to build the binary tree, add relevent binary state */
		new->left = one, new->right = two;
		new->left->bit = '0', new->right->bit = '1';

		/* Insert the new node back into the priority queue */
		if (*tree) {
			if (DS_huffman_tree_insert_ordered(tree, new, FN_data_frqcmp) == NULL)
				FAIL("NULL pointer returned");
		} else 
			*tree = new;
	}

	if (is_set(st_prg, PRINT)) {
		printf("print: huffman tree.\n");
		print_huffman_tree(*tree);
	}

	return tree;
}

/*
 * DS_huffman_tree_extract_encoding: Recursive function to walk tree and
 * extract binary data for encoding.
 */
int DS_huffman_tree_extract_encoding(
						HC_HuffmanNode *tree,
						String* str,
						Data *map)
{
	/* Add binary bit data to string */
	if (tree->bit)
		if ((str = GE_string_add_char(str, tree->bit)) == NULL)
			return 1;

	/* Branch left, adds '0' */
	if (tree->left) {
		DS_huffman_tree_extract_encoding(tree->left, str, map);
		str = GE_string_rem_char(str);
	}

	/* Branch right, adds '1' */
	if (tree->right) {
		DS_huffman_tree_extract_encoding(tree->right, str, map);
		str = GE_string_rem_char(str);
	}

	/* Fill data struct and insert into hash map */
	if (tree->data.utf8_char[0] != '\0') {
		Data data = tree->data;
		data.len_bin = str->len;
		memcpy(data.binary, str->str, data.len_bin+1);
		HC_hashtable_add_utf8_key(map, data);
	}

	return 0;
}

/*
 * DS_huffman_tree_clear: Clear tree before reuse.
 */
int DS_huffman_tree_clear(HC_HuffmanNode **tree)
{
	if (*tree == NULL) {
		FAIL("tree node is NULL");
		return 1;
	} else if ((*tree)->next != NULL)
		FAIL("Priority queue nodes still exist");

	if ((*tree)->left) {
		DS_huffman_tree_free(&(*tree)->left);
		(*tree)->left = NULL;
	}
	if ((*tree)->right) {
		DS_huffman_tree_free(&(*tree)->right);
		(*tree)->right = NULL;
	}

	return 0;
}

/*
 * DS_huffman_tree_free: Free huffman tree memory.
 */
int DS_huffman_tree_free(HC_HuffmanNode **tree)
{
	if (*tree == NULL) {
		FAIL("tree node is NULL");
		return 1;
	}

	else if ((*tree)->next != NULL)
		FAIL("Priority queue nodes still exist");

	if ((*tree)->left)
		DS_huffman_tree_free(&(*tree)->left);
	if ((*tree)->right)
		DS_huffman_tree_free(&(*tree)->right);

	free(*tree);

	return 0;
}

