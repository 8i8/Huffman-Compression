#include "HC_map_char.h"
#include "HC_error.h"
#include "HC_struct.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUF_LEN 32
#define MAP_LEN 2097153		/* 2^21 max number of Unicode char + 1 to stop
				   a while loop */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Char map
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * hash: Hash value for string s
 */
unsigned long hash(char *s)
{
	unsigned long hashval;

	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 3 * hashval;

	return hashval % MAP_LEN;
}

/*
 * _copy_data_to_map: Copy over the binary values from the Huffman tree into
 * the character map.
 */
int _copy_data_to_map(void* ma, void* tr)
{
	Data *map = ma;
	HC_HuffmanNode*node = tr;

	assert((map[hash(node->data.multi_byte_char)].len) == 0);
	map[hash(node->data.multi_byte_char)] = node->data;

	return 0;
}

/*
 * _populate_map: Set all string in the map array to '\0'.
 */
Data *_populate_map(Data *map, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++) {
		map[i].multi_byte_char[0] = '\0';
		map[i].string[0] = '\0';
		map[i].len = 0;
		map[i].frq = 0;
	}
	//memcpy(map[MAP_LEN - 1].string, "END", 4);
	return map;
}

/*
 * _expand_string: If the string that stores the binary data becomes to long,
 * enlarge it.
 */
char *_expand_string(String *string, size_t len)
{
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
 * _add_to_string: Tracks the binary data that is bing created within the 'trie'
 * structure amongst the Huffman nodes.
 */
String *_add_to_string(HC_HuffmanNode **tree, String *string, size_t len)
{
	if (len >= string->lim) {
		if (len > SIZE_MAX/2) {
			HC_error_set("%s: memory allocation failed.", __func__);
			return NULL;
		}
		_expand_string(string, len);
	}

	char c[2] = {'\0'};
	c[0] = (*tree)->bit;
	c[1] = '\0';
	strcat(string->str, c);

	return string;
}

/*
 * _huffman_tree_walk: Recursive function to walk tree and perform (*func) on
 * every node.
 */
Data *_huffman_tree_walk(
				HC_HuffmanNode **tree,
				void* map,
				int(*func)(void*, void*),
				String* string, int len)
{
	if ((string = _add_to_string(tree, string, len)) == NULL)
		return NULL;

	if ((*tree)->left) {
		_huffman_tree_walk(&(*tree)->left, map, func, string, ++len);
		string->str[--len] = '\0';
	}

	if ((*tree)->right) {
		_huffman_tree_walk(&(*tree)->right, map, func, string, ++len);
		string->str[--len] = '\0';
	}

	if ((*tree)->data.multi_byte_char[0] != '\0') {
		memcpy((*tree)->data.string, string->str, len);
		//TODO is this value redundant?
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
	map = _populate_map(map, MAP_LEN);
	map = _huffman_tree_walk(tree, map, _copy_data_to_map, &string, 1);
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
			printf("%s %s\n", map->multi_byte_char, map->string);
}

