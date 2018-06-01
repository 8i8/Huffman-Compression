#include "HC_char_map.h"
#include "HC_Error.h"
#include "HC_Struct.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

	//*(map+hash(node->data.str)) = node->data;
	map[hash(node->data.str)] = node->data;

	return 0;
}

/*
 * _populate_map: Set all string in the map array to '\0'.
 */
Data *_populate_map(Data *map, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++)
		map[i].binary[0] = '\0';
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
String *_add_to_string(HC_HuffmanNode**tree, String *string, size_t len)
{
	if (len >= string->lim) {
		if (len > SIZE_MAX/2) {
			HC_Error_set("%s: memory allocation failed.", __func__);
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
				HC_HuffmanNode**tree,
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

	if ((*tree)->data.str[0] != '\0') {
		memcpy((*tree)->data.binary, string->str, len);
		//TODO is this value redundant?
		(*tree)->data.len = len;
		func(map, *tree);
	}

	return map;
}

/*
 * create_char_map: Create char map from Huffman tree.
 */
Data *create_char_map(Data *map, HC_HuffmanNode **tree)
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
 * map_to_binary: Returns binary value for char from the given map.
 */
char *map_to_binary(Data *map, char *c)
{
	return map[hash(c)].binary;
}

/*
 * map_binary_len: Returns the length of the binary value for the given char.
 */
size_t map_binary_len(Data *map, char *c)
{
	return map[hash(c)].len;
}

