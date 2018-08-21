#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "huffman/HC_huffman_tree.h"
#include "general/GE_hashtable.h"
#include "general/GE_string.h"
#include "general/GE_state.h"
#include "general/GE_error.h"

// TODO change the collision data structure into a btree

/*
 * GE_hash: Hash value for string s
 */
unsigned long GE_hash(char *s)
{
	unsigned long GE_hashval;

	for (GE_hashval = 0; *s != '\0'; s++)
		GE_hashval = *s + 3 * GE_hashval;

	return GE_hashval % MAP_LEN;
}

/*
 * GE_data_init: Returns a data struct with all values set to 0.
 */
Data GE_data_init(void)
{
	Data map;
	map.utf8_char[0] = '\0';
	map.binary[0] = '\0';
	map.len_char = 0;
	map.len_bin = 0;
	map.st_dta = 0;
	map.frq = 0;
	map.next = NULL;
	return map;
}

/*
 * GE_data_frqcmp: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int.
 */
int GE_data_frqcmp(void *v1, void *v2)
{
	Data *d1, *d2;
	d1 = (Data*) v1;
	d2 = (Data*) v2;

	return (int)(d1->frq - d2->frq);
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Huffman coding into GE_hash map.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * GE_hashtable_init: Initialise array for char map.
 */
Data *GE_hashtable_init(Data *map)
{
	int i;
	for (i = 0; i < MAP_LEN; i++)
		map[i] = GE_data_init();
	return map;
}

/*
 * GE_hashtable_add_utf8_key: Add a new pair utf8 char key, binary value.
 */
int GE_hashtable_add_utf8_key(Data *map, Data data)
{

	int bucket = GE_hash(data.utf8_char);
	Data *cur;

	/* If the bucket is empty */
	if (map[bucket].utf8_char[0] == '\0') {
		map[bucket] = data;
		return 0;
	}

	/* if already present return 0 */
	if (strcmp(map[bucket].utf8_char, data.utf8_char) == 0)
		return 0;

	cur = &map[bucket];

	/* Add a node to the ajoined btree */
	if (cur->next) {
		while (cur->next) {
			if (strcmp(cur->utf8_char, data.utf8_char) == 0)
				return 0;
			cur = cur->next;
		}
		if (strcmp(cur->utf8_char, data.utf8_char) == 0)
			return 0;
	}

	/* Add the new value to either the bucket or the end of the list */
	if ((cur->next = malloc(sizeof(Data))) == NULL)
		FAIL("malloc failed");
	*(cur->next) = data;

	return 0;
}

/*
 * GE_hashtable_add_binary_key: Add a new pair binary key, utf8 char value.
 */
int GE_hashtable_add_binary_key(Data *map, Data data)
{
	int bucket = GE_hash(data.binary);
	Data *cur;

	/* If the bucket is empty */
	if (map[bucket].utf8_char[0] == '\0') {
		map[bucket] = data;
		return 0;
	}

	/* if already present return 0 */
	if (strcmp(map[bucket].utf8_char, data.utf8_char) == 0)
		return 0;

	cur = &map[bucket];

	/* Check all nodes for the value */
	if (cur->next) {
		while (cur->next) {
			if (strcmp(cur->binary, data.binary) == 0)
				return 0;
			cur = cur->next;
		}
		if (strcmp(cur->binary, data.binary) == 0)
			return 0;
	}

	/* Add the new value to either the bucket or the end of the list */
	if ((cur->next = malloc(sizeof(Data))) == NULL)
		FAIL("malloc failed");
	*cur->next = data;

	return 0;
}

/*
 * GE_hashtable_lookup_utf8: Returns the paired utf8 char value as a string,
 * for a given string key.
 */
Data GE_hashtable_lookup_utf8(Data *map, char *str)
{
	int bucket = GE_hash(str);
	Data cur;

	/* If the bucket is empty */
	if (map[bucket].utf8_char[0] == '\0')
		return GE_data_init();

	/* Is it here ? */
	if (strcmp(map[bucket].utf8_char, str) == 0)
		return map[bucket];

	/* Check all nodes in tree */
	if (map[bucket].next) {
		cur = map[bucket];
		while (cur.next != NULL) {
			if (strcmp(cur.utf8_char, str) == 0)
				return cur;
			cur = *cur.next;
		}
		if (strcmp(cur.utf8_char, str) == 0)
			return cur;
	}

	return GE_data_init();
}

/*
 * GE_hashtable_lookup_binary: Returns the paired binary string value of a
 * given utf8 char.
 */
Data GE_hashtable_lookup_binary(Data *map, char *str)
{
	int bucket = GE_hash(str);
	Data cur;

	/* If the bucket is empty */
	if (map[bucket].binary[0] == '\0')
		return GE_data_init();

	/* Is it here ? */
	if (strcmp(map[bucket].binary, str) == 0)
		return map[bucket];

	/* Check all nodes in tree */
	if (map[bucket].next) {
		cur = map[bucket];
		while (cur.next != NULL) {
			if (strcmp(cur.binary, str) == 0)
				return cur;
			cur = *cur.next;
		}
		if (strcmp(cur.binary, str) == 0)
			return cur;
	}

	return GE_data_init();
}

/*
 * free_collision_tree: Free the linked list that has been created to deal with GE_hash
 * collisions.
 */
static void free_collision_tree(Data *node)
{
	Data *old;
	while (node->next) {
		old = node;
		node = node->next;
		free(old);
	}
	free(node);
}

/*
 * GE_hashtable_clear: Reset the map to empty.
 */
void GE_hashtable_clear(Data *map)
{
	int i;
	for (i = 0; i < MAP_LEN; i++) {
		if (map[i].next) {
			free_collision_tree(map[i].next);
		}
		map[i] = GE_data_init();
	}
}

