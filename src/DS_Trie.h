#include <stdio.h>

#define LIST  1
#define CHECK 2

#ifndef _ds_trie_
#define _ds_trie_
typedef struct _ds_Trie {
	char c;
	struct _ds_Trie **next;
	int end;
} DS_Trie;
#endif

#ifndef _ds_trie_state_
#define _ds_trie_state_
typedef struct _ds_state {
	int *state;
	int *flag;
} DS_State;
#endif

/*
 * Create a new trie
 */
DS_Trie **DS_Trie_init(DS_Trie **trie);

/*
 * Add a new word to the trie
 */
DS_Trie **DS_Trie_add_word(DS_Trie **trie, char *string);

/*
 * Add a n char of a string to the trie
 */
DS_Trie **DS_Trie_add_n_char(DS_Trie **trie, int len, char *string);

/* 
 * DS_Trie_print_list: Output content of trie to the terminal.
 */
DS_Trie **DS_Trie_print_list(DS_Trie **trie, int (*func)(void*, void*));

/* 
 * DS_Trie_is_token: Iterate over the given string to assertain whether or not
 * it is present in the trie, returns 1 or 0.
 */
int DS_Trie_is_token(DS_Trie **trie, char *token);

/*
 * Dispose of trie
 */
void DS_Trie_free(DS_Trie **trie);

/* 
 * _func_print: Printout the content of the tree from char root to current
 * node
 */
int _func_print_trie(void *trie, void *var);

