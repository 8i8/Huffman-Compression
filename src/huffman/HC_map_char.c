#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "huffman/HC_map_char.h"
#include "huffman/HC_huffman_tree.h"
#include "general/GE_string.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_print.h"
#include "general/GE_error.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman coding into hash map.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * HC_map_init: Initialise array for char map.
 */
Data *HC_map_init(Data *map)
{
	int i;
	for (i = 0; i < MAP_LEN; i++)
		map[i] = HC_data_init();
	return map;
}

/*
 * hashmap_for_compression: Create char map from Huffman tree.
 */
void hashmap_for_compression(Data *map, HC_HuffmanNode **tree, const int st_prg)
{
	if (is_set(st_prg, VERBOSE))
		printf("Make character hashmap.\n");

	String *str = NULL;
	str = GE_string_init(str);
	DS_huffman_tree_extract_encoding(*tree, str, map);
	GE_string_free(str);

	if (is_set(st_prg, PRINT)) {
		printf("Print char map.\n");
		print_char_map(map);
	}
}

/*
 * HC_map_add_node: Add a new map node after hash collision.
 */
int HC_map_add_node(Data *map, int bucket, Data new_node)
{
	Data *cur;

	if (map[bucket].next)
		cur = map[bucket].next;
	while (cur->next != NULL)
		cur = cur->next;

	if ((cur->next = malloc(sizeof(Data))) == NULL)
		FAIL("malloc failed");
	*cur->next = new_node;

	return 0;
}

/*
 * HC_map_lookup_data: Returns binary value for char from the given map.
 */
Data HC_map_lookup_data(Data *map, char *c)
{
	int bucket = hash(c);
	Data cur;
	cur = map[bucket];

	if (map[bucket].next != NULL) {
			while ((cur.next != NULL) && strcmp(cur.utf8_char, c))
				cur = *cur.next;

		if (strcmp(cur.utf8_char, c))
			return HC_data_init();
		else
			return cur;
	} else
		return map[bucket];
}

/*
 * map_free_tree: Free the linked list that has been created to deal with hash
 * collisions.
 */
//static void map_free_tree(Data *map)
//{
//	Data *old;
//	if (map->next) {
//		map = map->next;
//		while (map->next) {
//			old = map;
//			map = map->next;
//			free(old);
//		}
//	}
//}

/*
 * HC_map_clear: Reset the map tp empty.
 */
void HC_map_clear(Data *map)
{
	for (int i = 0; i < MAP_LEN; i++)
		map[i] = HC_data_init();
}

