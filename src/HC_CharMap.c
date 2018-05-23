#include "HC_HuffmanTree.h"
#include "HC_Error.h"
#include "HC_Struct.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUF_LEN 32
#define MAP_LEN 2097152 		/* 2^21 max num of unicode char */

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


int _copy_data_to_map(void* ma, void* tr)
{
	Data *map = ma;
	HC_HuffmanTree *node = tr;

	printf("test map  ~ %s\n", map[hash(node->data.str)].str);
	printf("test tree ~ %s\n", node->data.str);

	assert(*(map+hash(node->data.str))->binary == '\0');

	*(map+hash(node->data.str)) = node->data;

	return 0;
}

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
	new = malloc(len+1);
	memcpy(new, string->str, len);
	free(string->str);
	string->str = new;
	string->lim = len;
	return new;
}

/*
 * _add_to_string: Tracks the binary data that is bing created within the 'trie'
 * structure amongst the Huffman nodes.
 */
String *_add_to_string(HC_HuffmanTree **tree, String *string, size_t len)
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
	strcat(string->str, c);

	return string;
}

/*
 * _huffman_tree_walk: Recursive function to walk tree and perform (*func) on
 * every node.
 */
Data *_huffman_tree_walk(
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
	}

	if ((*tree)->right) {
		_huffman_tree_walk(&(*tree)->right, map, func, string, ++len);
		string->str[--len] = '\0';
	}

	if ((*tree)->data.str[0] != '\0') {
		// TODO problem here in the code.
		memcpy((*tree)->data.binary, string->str, len);
		(*tree)->data.len = len;
		func(map, *tree);
	}

	return map;
}

/*
 * create_char_map: Create char map from Huffman tree.
 */
Data *create_char_map(Data *map, HC_HuffmanTree **tree)
{
	String string;
	string.str = calloc(BUF_LEN, 1), string.lim = BUF_LEN-1;
	map = malloc(sizeof(Data) * MAP_LEN);
	map = _populate_map(map, MAP_LEN);
	map = _huffman_tree_walk(tree, map, _copy_data_to_map, &string, 1);
	free(string.str);
	return map;
}

void print_char_map(Data *map)
{
	size_t i;
	for (i = 0; i < MAP_LEN; i++)
		if (map[i].binary[0] != '\0')
			printf("%s %s\n", map[i].str, map[i].binary);
}

