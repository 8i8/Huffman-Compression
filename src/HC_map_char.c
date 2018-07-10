#include "HC_map_char.h"
#include "HC_struct.h"
#include "GE_hash.h"
#include "GE_string.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUF_LEN 32

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Char map
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * huffman_tree_walk: Recursive function to walk tree and perform (*func) on
 * every node.
 * TODO NOW check on the string and the map allocation
 */
static Data *huffman_tree_walk(
				HC_HuffmanNode **tree,
				Data** map,
				String* string)
{
	int bucket;
	Data *cur;

	if ((string = GE_string_add_char(string, (*tree)->bit)) == NULL)
		return NULL;

	if ((*tree)->left) {
		huffman_tree_walk(&(*tree)->left, map, string);
		string = GE_string_rem_char(string);
	}

	if ((*tree)->right) {
		huffman_tree_walk(&(*tree)->right, map, string);
		string = GE_string_rem_char(string);
	}

	if ((*tree)->data.utf8_char[0] != '\0') {
		memcpy((*tree)->data.string, string->str, string->len);
		(*tree)->data.len = string->len;

		bucket = hash((*tree)->data.utf8_char);

		if (map[bucket] != NULL) {
			cur = map[bucket];
			while (cur->next)
				cur = cur->next;
			cur->next = &(*tree)->data;
		} else
			map[bucket] = &(*tree)->data;
		}

	return *map;
}

/*
 * map_create: Create char map from Huffman tree.
 * TODO NOW How is the map arry getting alocated?
 */
Data **map_create(Data **map, HC_HuffmanNode **tree)
{
	String *str = NULL;
	Data *start;
	str = GE_string_init(str);
	map = calloc(MAP_LEN, sizeof(Data*));
	start = *map;
	huffman_tree_walk(tree, map, str);
	GE_string_free(str);
	*map = start;
	return map;
}

/*
 * map_read_char_to_binary: Returns binary value for char from the given map.
 */
char *map_read_char_to_binary(Data **map, char *c)
{
	int bucket = hash(c);
	Data *cur;
	cur = map[bucket];

	while ((cur->next != NULL) && strcmp(cur->utf8_char, c))
		cur = cur->next;

	if (strcmp(cur->utf8_char, c))
		return NULL;
	else
		return cur->string;
}

/*
 * map_read_char_to_binary_len: Returns the length of the binary value for the given char.
 */
size_t map_read_char_to_binary_len(Data **map, char *c)
{
	return map[hash(c)]->len;

	int bucket = hash(c);
	Data *cur;
	cur = map[bucket];

	while ((cur->next != NULL) && strcmp(cur->utf8_char, c))
		cur = cur->next;

	if (strcmp(cur->utf8_char, c))
		return 0;
	else
		return cur->len;
}

/*
 * print_char_map: Print out the char map made by the Huffman tree.
 */
void print_char_map(Data **map)
{
	size_t i;
	Data *cur;
	for (i = 0; i < MAP_LEN; i++)
		if (map[i]) {
			printf("%s %s\n", map[i]->utf8_char, map[i]->string);
			if ((cur = map[i]->next))
				printf("%s %s\n", cur->utf8_char, cur->string);
		}
}

/*
 * map_free_tree: Free the linked list that has been created to deal with hash
 * collisions.
 */
static void map_free_tree(Data *map)
{
	Data *old;
	while (map->next) {
		old = map;
		map = map->next;
		free(old);
	}
	free(map);
}

/*
 * HC_map_free: Free all memory used by the charatcer mapping.
 */
void HC_map_free(Data **map)
{
	size_t i;

	for (i = 0; i < MAP_LEN; i++)
		if (map[i])
			map_free_tree(map[i]);
	free(map);
}

