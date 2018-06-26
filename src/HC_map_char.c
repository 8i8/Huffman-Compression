#include "HC_map_char.h"
#include "HC_error.h"
#include "HC_struct.h"
#include "GE_hash.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUF_LEN 32

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Char map
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * copy_data_to_map: Copy over the binary values from the Huffman tree into
 * the character map.
 */
static int copy_data_to_map(void* ma, void* tr)
{
	Data *map = ma;
	HC_HuffmanNode*node = tr;
	int bucket = hash(node->data.utf8_char);

	/* If assert fails, there has been a hash collision */
	assert((map[bucket].len) == 0);
	//map[bucket] = node->data;
	memcpy(&map[bucket], &node->data, sizeof(Data));

	return 0;
}

/*
 * populate_map: Set all string in the map array to '\0'.
 */
static Data *populate_map(Data *map, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++) {
		map[i].utf8_char[0] = '\0';
		map[i].string[0] = '\0';
		map[i].len = 0;
		map[i].frq = 0;
	}
	return map;
}

/*
 * expand_string: If the string that stores the binary data becomes to long,
 * enlarge it.
 */
static char *expand_string(String *string, size_t len)
{
	if (len > SIZE_MAX/2) {
		fprintf(stderr, "%s: memory allocation failed.", __func__);
		return string->str;
	}

	char *new;
	/* Same as len * 2 */
	len <<= 1;
	new = malloc(len);
	memcpy(new, string->str, string->lim);
	free(string->str);
	string->str = new;
	string->lim = len;
	return new;
}

/*
 * add_to_string: Tracks the binary data that is bing created within the 'trie'
 * structure amongst the Huffman nodes.
 */
static String *add_to_string(HC_HuffmanNode **tree, String *string, size_t len)
{
	if (len >= string->lim) {
		expand_string(string, len);
	}

	char c[2] = {'\0'};
	c[0] = (*tree)->bit;
	c[1] = '\0';
	strcat(string->str, c);

	return string;
}

/*
 * huffman_tree_walk: Recursive function to walk tree and perform (*func) on
 * every node.
 */
static Data *huffman_tree_walk(
				HC_HuffmanNode **tree,
				void* map,
				int(*func)(void*, void*),
				String* string, int len)
{
	if ((string = add_to_string(tree, string, len)) == NULL)
		return NULL;

	if ((*tree)->left) {
		huffman_tree_walk(&(*tree)->left, map, func, string, ++len);
		string->str[--len] = '\0';
	}

	if ((*tree)->right) {
		huffman_tree_walk(&(*tree)->right, map, func, string, ++len);
		string->str[--len] = '\0';
	}

	if ((*tree)->data.utf8_char[0] != '\0') {
		memcpy((*tree)->data.string, string->str, len);
		// TODO 09 check necesity of len here, could this code be more
		// efficient?
		(*tree)->data.len = len;
		func(map, *tree);
	}

	return map;
}

/*
 * map_create: Create char map from Huffman tree.
 */
Data *map_create(Data *map, HC_HuffmanNode **tree)
{
	String string;
	string.str = calloc(BUF_LEN, 1), string.lim = BUF_LEN-1;
	map = malloc(sizeof(Data) * MAP_LEN);
	map = populate_map(map, MAP_LEN);
	map = huffman_tree_walk(tree, map, copy_data_to_map, &string, 1);
	free(string.str);
	return map;
}

/*
 * map_read_char_to_binary: Returns binary value for char from the given map.
 */
char *map_read_char_to_binary(Data *map, char *c)
{
	return map[hash(c)].string;
}

/*
 * map_read_char_to_binary_len: Returns the length of the binary value for the given char.
 */
size_t map_read_char_to_binary_len(Data *map, char *c)
{
	return map[hash(c)].len;
}

/*
 * print_char_map: Print out the char map made by the Huffman tree.
 */
void print_char_map(Data *map)
{
	size_t i;
	for (i = 0; i < MAP_LEN; i++, map++)
		if (*(map->string) != '\0')
			printf("%s %s\n", map->utf8_char, map->string);
}

