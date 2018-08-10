#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "huffman/HC_hashtable.h"
#include "huffman/HC_huffman_tree.h"
#include "general/GE_string.h"
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "general/GE_error.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman coding into hash map.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * HC_hashtable_init: Initialise array for char map.
 */
Data *HC_hashtable_init(Data *map)
{
	int i;
	for (i = 0; i < MAP_LEN; i++)
		map[i] = HC_data_init();
	return map;
}

/*
 * HC_hashtable_add_utf8_key: Add a new pair utf8 char key, binary value.
 */
int HC_hashtable_add_utf8_key(Data *map, char utf8_char, char binary)
{
	int bucket = hash(utf8_char);
	Data *cur;

	/* If the bucket is empty */
	if (map[bucket].utf8_char[0] == '\0' ) {
		memcpy(map[bucket].utf8_char, &utf8_char, UTF8_LEN);
		memcpy(map[bucket].binary, &binary, BIN_MAX);
	}
	/* if already present return 0 */
	else if (strcmp(map[bucket].utf8_char, &utf8_char) == 0)
		return 0;
	/* Add a node to the ajoined btree */
	// TODO NEXT Make this a btree
	else if (map[bucket].next) {
		cur = map[bucket].next;

		while (cur->next != NULL) {
			if (strcmp(cur->utf8_char, &utf8_char) == 0)
				return 0;
			cur = cur->next;
		}

		if ((cur->next = malloc(sizeof(Data))) == NULL)
			FAIL("malloc failed");

		cur = cur->next;
		*cur = HC_data_init();
		memcpy(map[bucket].utf8_char, &utf8_char, UTF8_LEN);
		memcpy(map[bucket].binary, &binary, BIN_MAX);
	}

	return 0;
}

/*
 * HC_hashtable_add_binary_key: Add a new pair binary key, utf8 char value.
 */
int HC_hashtable_add_binary_key(Data *map, char binary, char utf8_char)
{
	int bucket = hash(binary);
	Data *cur;

	/* If the bucket is empty */
	if (map[bucket].utf8_char[0] == '\0' ) {
		memcpy(map[bucket].binary, &binary, BIN_MAX);
		memcpy(map[bucket].utf8_char, &utf8_char, UTF8_LEN);
	}
	/* if already present return 0 */
	else if (strcmp(map[bucket].utf8_char, &utf8_char) == 0)
		return 0;
	/* Add a node to the ajoined btree */
	// TODO NEXT Make this a btree
	else {
		cur = map[bucket].next;

		while (cur->next != NULL) {
			if (strcmp(cur->binary, &binary) == 0)
				return 0;
			cur = cur->next;
		}

		if ((cur->next = malloc(sizeof(Data))) == NULL)
			FAIL("malloc failed");

		cur = cur->next;
		memcpy(map[bucket].binary, &binary, BIN_MAX);
		memcpy(map[bucket].utf8_char, &utf8_char, UTF8_LEN);
	}

	return 0;
}

/*
 * HC_hashtable_lookup_string: Returns the paired string value for a given string key.
 */
Data HC_hashtable_lookup_string(Data *map, char str)
{
	int bucket = hash(str);
	Data cur = map[bucket];
	int res = 0;

	if (cur.next != NULL)
	{
		/* While str is not matched, move along if possible */
		while ((cur.next != NULL) && ((res = strcmp(cur.utf8_char, &str)) != 0))
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
 * HC_hashtable_clear: Reset the map to empty.
 */
void HC_hashtable_clear(Data *map)
{
	for (int i = 0; i < MAP_LEN; i++) {
		free_collision_tree(map[i]);
		map[i] = HC_data_init();
	}
}

