#include "HC_HuffmanTree.h"
#include "HC_Error.h"
#include "HC_Struct.h"
#include <stdlib.h>
#include <string.h>

#define MAP_LEN 257	/* Standard charset plus EOF character */

int _copy_data_to_map(void* ma, void* tr)
{
	HC_CharMap *map = ma;
	HC_HuffmanTree *tree = tr;

	if (strcmp(tree->data.str, "EOF") == 0)
		sprintf(map[256].binary, tree->data.binary);
	else
		sprintf(map[(int)tree->data.str[0]].binary, tree->data.binary);

	return 0;
}

HC_CharMap *_populate_map(HC_CharMap *map, size_t len)
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
	len <<= len;
	new = malloc(len+1);
	memcpy(new, string->str, string->len);
	free(string->str);
	string->str = new;
	string->len = len;
	return new;
}

/*
 * _add_to_string: Tracks the binary data that is bing created within the 'trie'
 * structure amongst the Huffman nodes.
 */
String *_add_to_string(HC_HuffmanTree **tree, String *string, size_t len)
{
	if (len >= string->len) {
		if (len > SIZE_MAX/2) {
			HC_Error_set("%s: memory allocation failed.", __func__);
			return NULL;
		}
		_expand_string(string, len);
	}

	char c[2] = {'\0'};
	c[0] = (*tree)->bit;
	strcat(string->str, c);
	string->len++;

	return string;
}

/*
 * _huffman_tree_walk: Recursive function to walk tree and perform (*func) on
 * every node.
 */
HC_CharMap *_huffman_tree_walk(
				HC_HuffmanTree **tree,
				void* map,
				int(*func)(void*, void*),
				String* string, int len)
{
	if ((string = _add_to_string(tree, string, len)) == NULL)
		return NULL;

	if ((*tree)->left) {
		_huffman_tree_walk(&(*tree)->left, map, func, string, ++len);
		string->str[--len] = '\0';
	} if ((*tree)->right) {
		_huffman_tree_walk(&(*tree)->right, map, func, string, ++len);
		string->str[--len] = '\0';
	} else {
		memcpy((*tree)->data.binary, string->str, string->len);
		func(map, *tree);
	}

	return map;
}

/*
 * create_char_map: Create char map from Huffman tree.
 */
HC_CharMap *create_char_map(HC_CharMap *map, HC_HuffmanTree **tree)
{
	String string;
	string.str = calloc(33, 1), string.len = 32;
	map = malloc(sizeof(HC_CharMap) * MAP_LEN);
	map = _populate_map(map, MAP_LEN);
	map = _huffman_tree_walk(tree, map, _copy_data_to_map, &string, 1);
	free(string.str);
	return map;
}

void print_char_map(HC_CharMap *map)
{
	size_t i;
	for (i = 0; i < 257; i++)
		if (map[i].binary[0] != '\0')
			printf("%s\n", map[i].binary);
}

