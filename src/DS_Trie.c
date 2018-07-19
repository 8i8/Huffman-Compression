#include "DS_Trie.h"
#include "GE_string.h"
#include <stdio.h>
#include <stdlib.h>

#define UCHAR 127 /* Length of character index for c_list */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Create and fill
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * new_c_arr: Create a new hash table, that is an array of possible characters
 * from the ASCII char set, all set to NULL ready for the next node.
 */
static DS_Trie **new_c_arr(DS_Trie **arr)
{
	int i;
	if ((arr = malloc(UCHAR * sizeof(DS_Trie*))) == NULL)
		return NULL;

	for (i = 0; i < UCHAR; i++)
		arr[i] = NULL;

	return arr;
}

/*
 * new_c_node: Create a new node with the given char.
 */
static DS_Trie *new_c_node(char c)
{
	DS_Trie *new_node;
	if((new_node = malloc(sizeof(DS_Trie))) == NULL)
		return NULL;
	new_node->c = c;
	new_node->next = NULL;
	new_node->end = 0;

	return new_node;
}

/*
 * add_word: Add a new word to the trie, a recursive function that adds a
 * string char by char, skipping over existing sub words and adding either
 * the wprd word else appensing the new suffix.
 */
static DS_Trie *add_word(DS_Trie *c_node, char *str)
{
	DS_Trie *ptr;
	ptr = c_node;

	while (*++str) { 
		/* Next node list */
		if (c_node->next == NULL)
			if ((c_node->next = new_c_arr(c_node->next)) == NULL)
				return NULL;
		/* Next node */
		if (c_node->next[(unsigned)*str] == NULL)
			if ((c_node->next[(unsigned)*str] = new_c_node(*str)) == NULL)
				return NULL;

		c_node = c_node->next[(unsigned)*str];
	}

	c_node->end = 1;

	return ptr;
}

/*
 * DS_Trieadd_word: Creates a new node for the first character of the string
 * and then calls the recursive function that adds the given string to the trie.
 */
DS_Trie **DS_Trie_add_word(DS_Trie **trie, char *str)
{
	if (trie[(unsigned)*str] == NULL)
		if ((trie[(unsigned)*str] = new_c_node(*str)) == NULL)
			return NULL;

	if ((add_word(trie[(unsigned)*str], str)) == NULL)
		return NULL;

	return trie;
}

/*
 * DS_Trie_add_n_char: Creates a new node for the first character of the string
 * and then calls the recursive function that adds the given length of the
 * given string to the trie.
 */
DS_Trie **DS_Trie_add_n_char(DS_Trie **trie, int len, char *string)
{
	char str[len+1];
	snprintf(str, len+1, "%s", string);

	if (trie[(unsigned)*str] == NULL)
		if ((trie[(unsigned)*str] = new_c_node(*str)) == NULL)
			return NULL;

	if ((add_word(trie[(unsigned)*str], str)) == NULL)
		return NULL;

	return trie;
}

/*
 * DS_Trie_init: Instantiate a new trie.
 */
DS_Trie **DS_Trie_init(DS_Trie **trie)
{
	return new_c_arr(trie);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Output
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * _func_print_trie: Printout the content of the tree from char root to current
 * node.
 */
int func_print_trie(void *input, void *var)
{
	int *n = var;
	String *Str = input;
	fwrite(Str->str, 1 ,*n, stdout);
	fwrite("\n", 1, 1, stdout);
	return 0;
}

/*
 * output_word: Recursive function that will perform it's given task on
 * arriving at each word ending.
 */
static void output_word(
				DS_Trie *trie,
				String *Str,
				int (*func)(void*, void*))
{
	int i;
	Str = GE_string_add_char(Str, trie->c);

	if (trie->end)
		(*func)(Str, &Str->len);

	if (trie->next != NULL)
		for (i = 0; i < UCHAR; i++)
			if (trie->next[i] != NULL) {
				output_word(trie->next[i], Str, func);
				Str = GE_string_rem_char(Str);
			}
}

/*
 * output_list: Iterates over every node in the array, performing the
 * recursive function on each element.
 */
static void output_list(
				DS_Trie **trie,
				String *Str,
				int (*func)(void*, void*))
{
	int i;

	for (i = 0; i < UCHAR; i++)
		if (trie[i] != NULL) {
			Str = GE_string_add_char(Str, trie[i]->c);
			output_word(trie[i], Str, func);
			Str = GE_string_rem_char(Str);
		}
}

/* 
 * DS_Trie_print_list: Output content of trie to the terminal.
 */
DS_Trie **DS_Trie_print_list(DS_Trie **trie, int (*func)(void*, void*))
{
	String *Str = NULL;
	Str = GE_string_init(Str);
	output_list(trie, Str, func);
	GE_string_free(Str);

	return trie;
}

/*
 * check_token: If the given token is found in the trie set the given state
 * with the given flag. Return 0 if the token is not found.
 */
static int check_token(
				DS_Trie *trie,
				String *Str)
{
	if (trie->end)
		return 1;

	if (trie->next && trie->next[(unsigned int)*Str->ptr])
		return check_token(trie->next[(unsigned int)*Str->ptr++], Str);
	else
		return 0;
}

/* 
 * DS_Trie_is_token: Iterate over the given string to assertain whether or not
 * it is present in the trie.
 */
int DS_Trie_is_token(DS_Trie **trie, char *token)
{
	String *Str = NULL;
	int res = 0;
	Str = GE_string_init(Str);
	GE_string_set(Str, token);

	/* Is the first character in the tries hash? */
	if (trie[(unsigned int)*Str->ptr])
		res = check_token(trie[(unsigned int)*Str->ptr++], Str);

	GE_string_free(Str);

	return res;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  Disposal
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
 * trie_free_word: Recursive function to free all the nodes in the trie.
 */
static void trie_free_word(DS_Trie *word)
{
	int i;

	if (word->next != NULL) {
		for (i = 0; i < UCHAR; i++)
			if (word->next[i] != NULL)
				trie_free_word(word->next[i]);
		free(word->next);
	}
	free(word);
}

/*
 * DS_Trie_free: Iterate over and free used slots in the char list.
 */
void DS_Trie_free(DS_Trie **list)
{
	int i;
	for (i = 0; i < UCHAR; i++)
		if (list[i] != NULL)
			trie_free_word(list[i]);
	free(list);
}

