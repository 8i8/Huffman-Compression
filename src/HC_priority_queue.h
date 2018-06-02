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
HC_HuffmanNode *HC_priority_queue_pop(HC_HuffmanNode*list);

/* HC_HuffmanTree_walk: Walk tree and perform (*func) on every node */
Data *HC_HuffmanTree_walk(HC_HuffmanNode **tree, void* store, int(*func)(void*, void*));

/* create_priority_cue: Call the ListSort function and set the sort parameter */
HC_HuffmanNode **create_priority_cue(HC_HuffmanNode **list, FILE *fp);
