#include "data_structures/DS_huffman_node.h"
#include "general/GE_file_buffer.h"

/* priority_queue_compression: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue */
unsigned priority_queue_compression(
						HC_HuffmanNode **list,
						F_Buf **io,
						const unsigned state);

/* priority_queue_decompression: Retrieve the frequency mapping from the
 * beginning of a compressed file and make it into a list, sort the list into a
 * priority queue */
unsigned priority_queue_decompression(
						HC_HuffmanNode **list,
						F_Buf *buf,
						unsigned state);

/* print_frequency: Output the frequency map */
void print_frequency(HC_HuffmanNode *list);

