#include "HC_HuffmanTree.h"
#include "HC_Error.h"
#include "HC_Struct.h"
#include <stdlib.h>
#include <string.h>

int _copy_data_to_map(void* ma, void* st)
{
	HC_CharMap *map = ma;
	String *string = st;

	if (strcmp(string->str, "EOF") == 0)
		sprintf(map[(int)string->str[256]].binary, string->str);
	else
		sprintf(map[(int)string->str[0]].binary, string->str);

	return 0;
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
char *_add_to_string(HC_HuffmanTree **tree, String *string, size_t len)
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

	return string->str;
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
	if (_add_to_string(tree, string, len) == NULL)
		return NULL;

	if ((*tree)->left) {
		_huffman_tree_walk(&(*tree)->left, map, func, string, ++len);
		string->str[--len] = '\0';
	} if ((*tree)->right) {
		_huffman_tree_walk(&(*tree)->right, map, func, string, ++len);
		string->str[--len] = '\0';
	} else
		func(map, string);

	return map;
}

/*
 * create_char_map: Create char map from Huffman tree.
 */
HC_CharMap *create_char_map(HC_CharMap *map, HC_HuffmanTree **tree)
{
	map = malloc(sizeof(HC_CharMap)*257);
	String string;
	string.str = calloc(33, 1), string.len = 32;
	map = _huffman_tree_walk(tree, map, _copy_data_to_map, &string, 1);
	free(string.str);
	return map;
	return map;
}

