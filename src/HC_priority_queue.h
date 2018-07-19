#include "HC_struct.h"

/* HC_priority_queue_new_node: Internal function for creating new list nodes */
HC_HuffmanNode *HC_priority_queue_new_node(Data data);

/* HC_priority_queue_add: Create the next node in the list and add data */
HC_HuffmanNode **HC_priority_queue_add(HC_HuffmanNode **list, Data data);

/* HC_priority_queue_insert: Insert a new node at tyhe current location */
HC_HuffmanNode **HC_priority_queue_insert(HC_HuffmanNode **list, Data data);

/* HC_priority_queue_insert_ordered: Insert a new node conditionaly */
HC_HuffmanNode **HC_priority_queue_insert_ordered(HC_HuffmanNode **list, HC_HuffmanNode*newList,
						int(*func)(void*, void*));

/* HC_priority_queue_pop: Pop the first node and return a pointer to the new list
 * address. */
HC_HuffmanNode *HC_priority_queue_pop(HC_HuffmanNode **list);

/* HC_HuffmanTree_walk: Walk tree and perform (*func) on every node */
Data *HC_HuffmanTree_walk(HC_HuffmanNode **tree, void* store, int(*func)(void*, void*));

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

