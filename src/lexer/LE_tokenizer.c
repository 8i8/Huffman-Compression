#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "general/GE_state.h"
#include "general/GE_hash.h"
#include "lexer/LE_lexer.h"
#include "lexer/LE_tokenizer.h"

#define TABLE_LEN 256

static Token **token_table;

/*
 * token_to_tree: Add a new token to the binary tree after a hash collision.
 */
static void token_to_tree(Token *tree, Token *new_token)
{
	int res = strcmp(tree->id, new_token->id);

	if (res < 0) {

		if (tree->left) {
			token_to_tree(tree->left, new_token);
			return;
		} else
			tree->left = new_token;

	} else if (res > 0) {

		if (tree->right) {
			token_to_tree(tree->right, new_token);
			return;
		} else
			tree->right = new_token;
	}
}

/*
 * token_add: Add a new token to the hashmap.
 */
static int token_add(char *token, int flag)
{
	int bucket = hash(token);
	Token *new_token;
       
	if ((new_token = malloc(sizeof(Token))) == NULL)
		return 1;

	new_token->id = strdup(token);
	new_token->flag = flag;
	new_token->left = NULL;
	new_token->right = NULL;

	if (token_table[bucket])
		token_to_tree(token_table[bucket], new_token);
	else
		token_table[bucket] = new_token;

	return 0;
}

/*
 * LE_token_init: Initalise and load the hash map and its tokens.
 */
int LE_token_init(void)
{
	if ((token_table = calloc(TABLE_LEN, sizeof(Token))) == NULL)
		return 1;

	token_add("map", LEX_MAP);
	token_add("comp", LEX_DECOMPRESS);
	token_add("name", LEX_FILENAME);
	token_add("ch", LEX_CHAR);
	token_add("break", DECOMPRESS);

	return 0;
}

/*
 * check_tree: Recursive function that walks the binary tree created in the
 * case of hash collisions, looking for the given token.
 */
int check_tree(Token *tree, char *check)
{
	int res = strcmp(tree->id, check);

	if (res == 0)
		return tree->flag;

	if (res < 0)
		if (tree->left)
			return check_tree(tree->left, check);
	if (res > 0)
		if (tree->right)
			return check_tree(tree->right, check);
	return 0;
}

/*
 * LE_check_token: Check token_table for the given token, if it is found
 * return the stored flag, else return 0.
 */
int LE_check_token(char *token)
{
	int bucket = hash(token);
	Token *current;

	if ((current = token_table[bucket]))
		return check_tree(current, token);

	return 0;
}

/*
 * free_tree: Free binary tree.
 */
static int free_tree(Token *tree)
{
	free(tree->id);

	if (tree->left)
		free_tree(tree->left);
	if (tree->right)
		free_tree(tree->right);
	free(tree);

	return 0;
}

/*
 * LE_token_free: Free all memory allovated to the hash map and its binary
 * trees.
 */
int LE_token_free(void)
{
	size_t i;
	if (token_table)
		for (i = 0; i < TABLE_LEN; i++)
			if (token_table[i])
				free_tree(token_table[i]);
	return 0;
}

