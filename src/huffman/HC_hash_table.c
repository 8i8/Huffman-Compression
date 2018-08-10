#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "huffman/HC_hash_table.h"
#include "huffman/HC_huffman_tree.h"
#include "general/GE_string.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_error.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman coding into hash map.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * HC_hash_table_init: Initialise array for char map.
 */
Data *HC_hash_table_init(Data *map)
{
	int i;
	for (i = 0; i < MAP_LEN; i++)
		map[i] = HC_data_init();
	return map;
}

/*
 * HC_hash_table_add_value: Add a new map node in the case of a hash collision.
 */
int HC_hash_table_add_value(Data *map, int bucket, Data new_node)
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
 * HC_hash_table_lookup_string: Returns the paired string value for a given string key.
 */
Data HC_hash_table_lookup_string(Data *map, char *str)
{
	int bucket = hash(str);
	Data cur = map[bucket];
	int res = 0;

	if (cur.next != NULL)
	{
		/* While str is not matched, move along if possible */
		while ((cur.next != NULL) && ((res = strcmp(cur.utf8_char, str)) != 0))
			cur = *cur.next;

		if (res)
			return HC_data_init();
	}
	
	return cur;
}

/*
 * free_collision_tree: Free the linked list that has been created to deal with hash
 * collisions.
 */
static void free_collision_tree(Data map)
{
	Data *old;
	if (map.next) {
		map = *map.next;
		while (map.next) {
			old = &map;
			map = *map.next;
			free(old);
		}
	}
}

/*
 * HC_hash_table_clear: Reset the map to empty.
 */
void HC_hash_table_clear(Data *map)
{
	for (int i = 0; i < MAP_LEN; i++) {
		free_collision_tree(map[i]);
		map[i] = HC_data_init();
	}
}

