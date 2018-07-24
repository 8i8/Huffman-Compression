#include "data_structures/DS_huffman_node.h"
#include "general/GE_file_buffer.h"

/* create_priority_queue: Compile a frequency list for all characters in the
 * document, sort that list into a priority queue */
HC_HuffmanNode **create_priority_queue(
						HC_HuffmanNode **list,
						F_Buf **buf,
						const unsigned state);

/* build_priority_queue_from_file: Retrieve the frequency mapping from the
 * beginning of a compressed file and make it into a list, sort the list into a
 * priority queue */
HC_HuffmanNode **build_priority_queue_from_file(HC_HuffmanNode **list, F_Buf **io);

/* print_frequency: Output the frequency map */
void print_frequency(HC_HuffmanNode *list);

