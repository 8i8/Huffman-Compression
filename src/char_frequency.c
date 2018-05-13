#include "../data_structures/DS_DLinkedList.h"
#include "../data_structures/DS_DListSort.h"
#include "../tests/DS_Struct.h"
#include "../general/DS_Error.h"

/*
 * _comp_test: Compare Data one and two, the value should be a single char and
 * the result alphabetical order as per the ASCII char numbering system.
 */
int _comp_test(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	return (char)*d1->str - (char)*d2->str;
}

/*
 * _insert_alphanum_count: If the char does not yet exist in the char map,
 * insert it in alphabetical order. If it exists already, add one to the count
 * for that character.
 */
DS_DLinkedList **_insert_alphanum_count(DS_DLinkedList **list, Data data,
						int(*func)(void*, void*))
{
	int test;

	if (list == NULL) {
		DS_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	} else if (*list == NULL) {
		DS_DLinkedList_add(list, data);
		return list;
	}

	while (*list != NULL)
	{
		if ((test = (*func)((void*)&data, (void*)&(*list)->data)) < 0) {
			if ((DS_DLinkedList_insert(list, data)) == NULL) {
				DS_Error_append("%s: ", __func__);
				return NULL;
			}
			break;
		} else if (test == 0) {
			(*list)->data.num++;
			break;
		} else if (test > 0)
			list = &(*list)->next;
		if (*list == NULL) {
			DS_DLinkedList_add(list, data);
			break;
		}
	}

	return list;
}

/*
 * frequency_count: Sort alphabetically and count occurrences of each character in
 * the document.
 */
DS_DLinkedList **_frequency_count(DS_DLinkedList **list, FILE *fp)
{
	char c;
	Data data;

	while ((c = getc(fp)) != EOF) {
		data.str[0] = c, data.str[1] = '\0', data.num = 1;
		_insert_alphanum_count(list, data, _comp_test);
	}

	return list;
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
 * build_frequency_table: Call the ListSort function and set the sort parameter.
 */
DS_DLinkedList **build_frequency_table(DS_DLinkedList **list, FILE *fp)
{
	list = _frequency_count(list, fp);
	return DS_DListSort(list, _comp_freq);
}

DS_DLinkedList **merge_nodes_to_tree(DS_DLinkedList **list)
{
	DS_DLinkedList *new, *one, *two;
	Data data;

	data->num = one->data->num + two->data->num;
	data->str = '\0';

	new = DS_DLinkedList_new_node(data);

	new->prev = one->prev, new->next = two->next;
	
}

/*
 * build_huffman_tree: Create a binary tree from the given linked list, make the
 * leaf node distance from the tree root inversely proportional to the Huffman
 * nodes character frequency.
 */
DS_DLinkedList **build_huffman_tree(DS_DLinkedList **list)
{
	DS_DLinkedList **list_pt;
	list_pt = list;

	while (*list)
		if ((*list)->next)
			merge_nodes_to_tree(list);

	return list_pt;
}

/*
 * print_frequency: Output the frequency map.
 */
void print_frequency(DS_DLinkedList **list)
{
	while (*list != NULL) {
		printf("%s ", (*list)->data.str);
		printf("%lu\n", (*list)->data.num);
		list = &(*list)->next;
	}
}

