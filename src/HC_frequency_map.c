#include "HC_HuffmanTree.h"
#include "HC_ListSort.h"
#include "HC_frequency_map.h"
#include "HC_utf8.h"
#include "HC_Struct.h"
#include "HC_Error.h"
#include "stdlib.h"

/*
 * _comp_char: Compare Data one and two, the value should be a single char and
 * the result alphabetical order as per the ASCII char numbering system.
 */
int _comp_char(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	//return d1->str[0] - d2->str[0];
	return strcmp(d1->str, d2->str);
}

/*
 * _comp_freq: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int.
 */
int _comp_freq(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	return (int)d1->num - (int)d2->num;
}

/*
 * _insert_or_count: If the char does not yet exist in the char map,
 * insert it in alphabetical order. If it exists already, add one to the count
 * for that character.
 */
HC_HuffmanTree **_insert_or_count(HC_HuffmanTree **list, Data data,
						int(*func)(void*, void*))
{
	int test;

	if (list == NULL) {
		HC_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	} else if (*list == NULL) {
		list = HC_HuffmanTree_add(list, data);
		return list;
	}

	while (*list != NULL)
	{
		if ((test = (*func)((void*)&data, (void*)&(*list)->data)) < 0) {
			if ((HC_HuffmanTree_insert(list, data)) == NULL) {
				HC_Error_append("%s: ", __func__);
				return NULL;
			}
			break;
		} else if (test == 0) {
			(*list)->data.num++;
			break;
		} else if (test > 0)
			list = &(*list)->next;
		if (*list == NULL) {
			HC_HuffmanTree_add(list, data);
			break;
		}
	}

	return list;
}

/*
 * _compile_frequency_list: Sort alphabetically and count occurrences of each character in
 * the document.
 */
HC_HuffmanTree **_compile_frequency_list(HC_HuffmanTree **list, FILE *fp)
{
	char c, *ptr;
	Data data;

	/* Scan document */
	while ((c = fgetc(fp)) != EOF)
	{
		ptr = data.str;
		/* Keep getting char for the lenght of the multi-byte character */
		while (utf8_test_count(c) && (*ptr++ = c))
			c = fgetc(fp);
		*ptr = c;
		ptr[1] = '\0';
		data.num = 1;
		_insert_or_count(list, data, _comp_char);
	}

	/* Add EOF char */
	memcpy(data.str, "EOF", 4), data.num = 0;
	_insert_or_count(list, data, _comp_char);

	return list;
}

/*
 * create_priority_cue: Call the ListSort function and set the sort parameter.
 */
HC_HuffmanTree **create_priority_cue(HC_HuffmanTree **list, FILE *fp)
{
	/* Count */
	list = _compile_frequency_list(list, fp);
	/* Sort by frequency */
	list = HC_ListSort(list, _comp_freq);

	return list;
}

/*
 * build_huffman_tree: Create a binary tree from the given linked list.
 */
HC_HuffmanTree **build_huffman_tree(HC_HuffmanTree **list)
{
	HC_HuffmanTree *new, *one, *two;
	Data data;
	data.num = 0, data.str[0] = '\0';

	while ((*list)->next)
	{
		one = *list;
		*list = HC_HuffmanTree_pop(*list);
		two = *list;
		*list = HC_HuffmanTree_pop(*list);

		data.num = one->data.num + two->data.num;
		data.str[0] = '\0';

		/* Add leaves to new node and give a binary value */
		if ((new = HC_HuffmanTree_new_node(data)) == NULL)
			HC_Error_print();
		new->left = one, new->right = two;
		new->left->bit = '0', new->right->bit = '1';

		/* Insert new node into priority cue */
		if (*list) {
			//if (HC_HuffmanTree_insert_huffman_node(list, new) == NULL)
			if (HC_HuffmanTree_insert_ordered(list, new, _comp_freq) == NULL)
				HC_Error_print();
		} else 
			*list = new;
		//HC_ListSort(list, _comp_freq);
	}

	return list;
}

void _print_huffman_tree(HC_HuffmanTree *tree, int depth)
{
	int i;
	if (tree->left)
		_print_huffman_tree(tree->left, ++depth);

	for (i = 0; i < --depth; i++)
		putchar(' ');
	printf("%s\n", tree->data.str);

	if (tree->right)
		_print_huffman_tree(tree->right, ++depth);
}

/*
 * print_huffman_tree: Print out huffman tree.
 */
void print_huffman_tree(HC_HuffmanTree *tree)
{
	_print_huffman_tree(tree, 0);
	putchar('\n');
}

/*
 * print_frequency: Output the frequency map.
 */
void print_frequency(HC_HuffmanTree **list)
{
	while (*list != NULL) {
		printf("%s ", (*list)->data.str);
		printf("%lu\n", (*list)->data.num);
		list = &(*list)->next;
	}
}

