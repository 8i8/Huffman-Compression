#include "general/GE_state.h"
#include "general/GE_file_buffer.h"
#include "data_structures/DS_huffman_tree.h"
#include "data_structures/DS_huffman_tree.h"
#include "huffman/HC_priority_queue.h"
#include "huffman/HC_map_char.h"
#include "huffman/HC_read_write.h"
#include "lexer/LE_lexer.h"

/*
 * huffman: Run main program code.
 */
int huffman(F_Buf **io, HC_HuffmanNode **tree, Data **map, int st_prg)
{
	/* If required, write compressed data */
	if (is_set(st_prg, COMPRESS) && !is_set(st_prg, ESC))
	{
		if (!is_set(st_prg, MONO)) {
			/* Make priority queue from the input file of character
			 * frequncy and then construct an ordered binary tree from that
			 * queue, and then a hashmap with the binary represetation of
			 * each char */
			priority_queue(tree, io, st_prg);
			ordered_binary_tree(tree, st_prg);
			hashmap_binary_out(map, tree, st_prg);

			/* Write file metadata */
			metadata_write_file_name(io[0]);
			metadata_write_map(map, io[0]);
			archive_write_file(map, io, st_prg);
		}
	}

	if (is_set(st_prg, DECOMPRESS) && !is_set(st_prg, ESC)) {
		st_prg = decompress_archive(tree, io, st_prg);
	}

	if (is_set(st_prg, ERROR))
		fprintf(stderr, "%s: state error signaled.", __func__);
	return 0;
}

/*
 * decompress_archive: Read and then decompress compressed file. Analyze file
 * stream with lexer to decompress the file.
 * TODO NOW get file name and open file for writing.
 */
int decompress_archive(HC_HuffmanNode **list, F_Buf **io, const int st_prg)
{
	char c;
	/* Lexer state */
	int st_lex = 0;
	LE_lexer_init();

	if (is_set(st_prg, VERBOSE))
		printf("Opening archive.\n");

	GE_buffer_on(io[0]);
	while (io[0] && (c = GE_buffer_fgetc(io[0])) != EOF)
	{
		/* Set the state */
		if ((st_lex = LE_get_token(io[0], c, st_lex)) == 0) {
			fprintf(stderr, "%s: Token failed.\n", __func__);
			state_set(st_lex, ERROR);
			break;
		}

		if (is_set(st_lex, LEX_MAP)) {
			;//decompress_mono(list, io, st_prg, st_lex);
		}

	//	else (is_set(st_lex, MULTI))
	//		decompress_multi(io, st_lex);
	}
	GE_buffer_rewind(io[0]);
	GE_buffer_off(io[0]);

	LE_lexer_free();

	return st_lex;
}

