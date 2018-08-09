#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "general/GE_hash.h"
#include "general/GE_error.h"
#include "general/GE_string.h"
#include "huffman/HC_huffman_tree.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_mergesort.h"
#include "huffman/HC_hashmap.h"
#include "huffman/HC_read_write.h"
#include "lexer/LE_lexer.h"

/*
 * decompress_archive: Read and then decompress compressed file. Analyze file
 * stream with lexer to know the decompression procedure for the particular
 * file.
 * TODO NOW this is the main state machine
 */
int decompress_archive(F_Buf **io, const int st_prg)
{
	char c = ' ';
	int st_lex = 0;
	Data map[MAP_LEN];
	String str;

	if (is_set(st_prg, VERBOSE))
		printf("Opening archive.\n");

	LE_lexer_init();
	GE_buffer_on(io[0]);
	while (io[0] && c != EOF && is_set(st_prg, DECOMPRESS))
	{
		/* Set the state */
		if ((c = LE_get_token(io[0], c, &st_lex)) == 0)
			FAIL("Token failed");

		/* Decompress that thing */
		switch (st_lex)
		{
			case LEX_MAP:
				st_lex = metadata_read_map(io, map, st_lex, st_prg);
				break;
			case LEX_FILENAME:
				str = GE_string_stack_init(str);
				str = metadata_read_filename(io[0], str, st_prg);
				GE_open_file(str.str, io, "w", st_prg);
				break;
			case LEX_DECOMPRESS:
				//write_text_file();
				break;
			default :
				WARNING("default clause reached");
				break;
		}
	}
	GE_buffer_rewind(io[0]);
	GE_buffer_off(io[0]);
	LE_lexer_free();

	return st_lex;
}

int write_archive_MULTI(F_Buf **io, HC_HuffmanNode **tree, int st_prg)
{
	int i;
	Data map[MAP_LEN];
	HC_map_init(map);

	for (i = 1; i < MAX_FILES && io[i]; i++) {
		frequency_list_compression(tree, io[i], st_prg);

		if (HC_mergesort(tree, FN_data_frqcmp) == NULL)
			FAIL("mergsort failed");

		ordered_binary_tree(tree, st_prg);
		hashmap_for_compression(map, tree, st_prg);
		metadata_write_map(map, io[0], st_prg);
		metadata_write_file_name(io[0], io[1], st_prg);
		compression_write_file(map, io[0], io[1], st_prg);
		DS_huffman_tree_clear(tree);
		HC_map_clear(map);
	}

	return st_prg;
}

int write_archive_MONO(F_Buf **io, HC_HuffmanNode **tree, int st_prg)
{
	int i;
	Data map[MAP_LEN];
	HC_map_init(map);

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
		compression_write_file(map, io[i], io[0], st_prg);
	}

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
		FAIL("state signaled error");

	return st_prg;
}

