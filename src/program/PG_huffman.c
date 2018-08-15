#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "general/GE_hash.h"
#include "general/GE_error.h"
#include "general/GE_string.h"
#include "general/GE_hashtable.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_mergesort.h"
#include "lexer/LE_lexer.h"
#include "program/PG_print.h"
#include "program/PG_read_write.h"

/*
 * hashmap_for_compression: Fills char hash table from an ordered binary tree
 * with each characters binary representation.
 */
void hashmap_for_compression(Data *map, HC_HuffmanNode **tree, const int st_prg)
{
	if (is_set(st_prg, VERBOSE))
		printf("metadata: make hash table from binary tree.\n");

	String *str = NULL;
	str = GE_string_init(str);
	DS_huffman_tree_extract_encoding(*tree, str, map);
	GE_string_free(str);

	if (is_set(st_prg, PRINT)) {
		printf("Print char map.\n");
		print_hashtable(map);
	}
}

/*
 * decompress_archive: Read and then decompress compressed file. Analyze file
 * stream with lexer to know the decompression procedure for the particular
 * file.
 */
int decompress_archive(F_Buf **io, const int st_prg)
{
	char c = ' ';
	int i, st_lex;
        i = st_lex = 0;
	Data map[MAP_LEN];
	HC_hashtable_init(map);
	String str;

	if (is_set(st_prg, VERBOSE))
		printf("Opening archive.\n");

	LE_lexer_init();
	GE_buffer_on(io[0]);
	c = GE_buffer_fgetc(io[0]);
	while (io[0] && c != EOF && !is_set(st_lex, LEX_ERROR))
	{
		/* Change state */
		if ((c = LE_get_token(io[0], c, &st_lex)) == 0) {
			FAIL("decompression");
			break;
		}

		/* Decompress */
		if (is_set(st_lex, LEX_MAP))
			st_lex = metadata_read_hash_table_data(io[0], map, st_lex, st_prg);

		else if (is_set(st_lex, LEX_FILENAME)) {
			str = GE_string_stack_init(str);
			str = metadata_read_filename(io[0], str, &st_lex, st_prg);
			GE_file_open_array(io, str.str, "w", st_prg);
		}
		else if (is_set(st_lex, LEX_DECOMPRESS))
			st_lex = decompress_write_file(io[0], io[++i], map, st_lex, st_prg);

	}

	if (is_set(st_lex, LEX_ERROR))
		FAIL("archive corrrupted");

	GE_buffer_rewind(io[0]);
	GE_buffer_off(io[0]);
	LE_lexer_free();

	return st_lex;
}

int write_archive_MULTI(F_Buf **io, HC_HuffmanNode **tree, int st_prg)
{
	int i;
	Data map[MAP_LEN];
	HC_hashtable_init(map);

	for (i = 1; i < MAX_FILES && io[i]; i++) {
		frequency_list_compression(tree, io[i], st_prg);

		if (HC_mergesort(tree, FN_data_frqcmp) == NULL)
			FAIL("mergsort failed");

		ordered_binary_tree(tree, st_prg);
		hashmap_for_compression(map, tree, st_prg);
		metadata_write_map(map, io[0], st_prg);
		metadata_write_file_name(io[0], io[1], st_prg);
		compression_write_archive(map, io[0], io[1], st_prg);
		DS_huffman_tree_clear(tree);
		HC_hashtable_clear(map);
	}

	return st_prg;
}

int write_archive_MONO(F_Buf **io, HC_HuffmanNode **tree, int st_prg)
{
	int i;
	Data map[MAP_LEN];
	HC_hashtable_init(map);

	/* Make priority queue from the input file's character count and then
	 * construct an ordered binary tree from that queue, and then finaly a
	 * hashmap with a binary representation for each char */

	/* Scan all of the files counting incidences of each char */
	for (i = 1; i < MAX_FILES && io[i]; i++)
		frequency_list_compression(tree, io[i], st_prg);

	/* Sort by frequency */
	if (HC_mergesort(tree, FN_data_frqcmp) == NULL)
		FAIL("mergsort failed");
	
	/* Build a hashmap by way of an ordered binary tree */
	ordered_binary_tree(tree, st_prg);
	hashmap_for_compression(map, tree, st_prg);

	/* Write metadata */
	metadata_write_map(map, io[0], st_prg);

	/* Write each file name then the compressed data consecutively */
	for (i = 1; i < MAX_FILES && io[i]; i++) {
		metadata_write_file_name(io[i], io[0], st_prg);
		compression_write_archive(map, io[i], io[0], st_prg);
	}

	HC_hashtable_clear(map);

	return st_prg;
}

/*
 * huffman: Run main program code.
 */
int huffman(F_Buf **io, HC_HuffmanNode **tree, int st_prg)
{
	/* If required, write compressed data */
	if (is_set(st_prg, COMPRESS) && !is_set(st_prg, ESC))
	{
		if (is_set(st_prg, MULTI))
			st_prg = write_archive_MULTI(io, tree, st_prg);
		else
			st_prg = write_archive_MONO(io, tree, st_prg);
	}

	if (is_set(st_prg, DECOMPRESS) && !is_set(st_prg, ESC))
		st_prg = decompress_archive(io, st_prg);

	if (is_set(st_prg, ERROR))
		FAIL("program runtime error");

	return st_prg;
}

