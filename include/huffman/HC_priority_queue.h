#include "data_structures/DS_huffman_node.h"
#include "general/GE_file_buffer.h"

/* compression_metadata: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue */
unsigned compression_metadata(
						HC_HuffmanNode **list,
						F_Buf **io,
						const int state);

/* decompression_priority_queue: Retrieve the frequency mapping from the
 * beginning of a compressed file and make it into a list, sort the list into a
 * priority queue */
unsigned decompression_priority_queue(
						HC_HuffmanNode **list,
						F_Buf *buf,
						int state);

/* print_frequency: Output the frequency map */
void print_frequency(HC_HuffmanNode *list);

