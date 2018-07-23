#include "huffman/HC_huffman_node.h"
#include "general/GE_file_buffer.h"

/* DS_linkedlist_new_node: Internal function for creating new list nodes */
HC_HuffmanNode *DS_linkedlist_new_node(Data data);

/* DS_linkedlist_add: Create the next node in the list and add data */
HC_HuffmanNode **DS_linkedlist_add(HC_HuffmanNode **list, Data data);

/* DS_linkedlist_insert: Insert a new node at tyhe current location */
HC_HuffmanNode **DS_linkedlist_insert(HC_HuffmanNode **list, Data data);

/* DS_linkedlist_insert_ordered: Insert a new node conditionaly */
HC_HuffmanNode **DS_linkedlist_insert_ordered(HC_HuffmanNode **list, HC_HuffmanNode*newList,
						int(*func)(void*, void*));

/* DS_linkedlist_pop: Pop the first node and return a pointer to the new list
 * address. */
HC_HuffmanNode *DS_linkedlist_pop(HC_HuffmanNode **list);

