#include "huffman/HC_huffman_tree.h"
#include "general/GE_file_buffer.h"

/* FN_data_strcmp: Compare Data one and two, the value should be a single char and
 * the result alphabetical order as per the ASCII char numbering system */
int FN_data_strcmp(void *v1, void *v2);

/* frequency_list_compression: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue */
unsigned frequency_list_compression(
						HC_HuffmanNode **list,
						F_Buf *buf,
						const int state);

/* decompress_priority_queue: Retrieve the frequency mapping from the
 * beginning of a compressed file and make it into a list, sort the list into a
 * priority queue */
unsigned decompress_priority_queue(
						HC_HuffmanNode **list,
						F_Buf *buf,
						int state);

/* print_frequency: Output the frequency map */
void print_frequency(HC_HuffmanNode *list);

