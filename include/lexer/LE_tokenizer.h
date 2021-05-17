#ifndef LE_TOKEN_CHAIN
#define LE_TOKEN_CHAIN

typedef struct token_chain {
	char *id;
	int flag;
	struct token_chain *left;
	struct token_chain *right;
} Token;

/*
 * LE_token_init: Initalise and load the hash map and its tokens.
 */
int LE_token_init(void);

/*
 * check_tree: Recursive function that walks the binary tree of hash collision
 * in the token hash map, looking for the given token.
 */
int check_tree(Token *tree, char *check);

/*
 * LE_check_token: Check token_table for the given token, if it is found
 * return the stored flag, else return 0.
 */
int LE_check_token(char *token);

/*
 * LE_token_free: Free all memory allovated to the hash map and its binary
 * trees.
 */
int LE_token_free(void);

#endif
