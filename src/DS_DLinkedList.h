/*
 * DS_DLinkedList
 * TODO Can this be abstracted one level further using pointers to pointers?
 * Avoiding some of the issues of and whilst economising access by, holding the
 * last accessed node in an encapsulating struct. Do compilers make this
 * optimisation automagicaly?
 */
#include <stdio.h>
#include "../tests/DS_Struct.h"

#ifndef _ds_dlinkedlist_
#define _ds_dlinkedlist_
typedef struct _ds_dlinkedlist {
	Data data;
	struct _ds_dlinkedlist *prev;
	struct _ds_dlinkedlist *next;
} DS_DLinkedList;
#endif

/* DS_DLinkedList_new_node: Internal function for creating new list nodes */
DS_DLinkedList *DS_DLinkedList_new_node(Data data);

/* DS_DLinkedList_add: Create the next node in the list and add data */
DS_DLinkedList **DS_DLinkedList_add(DS_DLinkedList **list, Data data);

/* DS_DLinkedList_get: Returns the node at the given index, if that index
 * exists */
DS_DLinkedList **DS_DLinkedList_get(DS_DLinkedList **list, size_t index);

/* DS_DLinkedList_do: Itterate over the intire list, perform given function
 * on each node */
int DS_DLinkedList_do(DS_DLinkedList **list, void *var, int(*func)(void*, void*));

/* DS_DLinkedList_insert: Insert a new node at tyhe current location */
DS_DLinkedList **DS_DLinkedList_insert(DS_DLinkedList **list, Data data);

/* DS_DLinkedList_insert_at: Insert a new node before node n */
DS_DLinkedList **DS_DLinkedList_insert_at(DS_DLinkedList **list, size_t num, Data data);

/* DS_DLinkedList_remove: Remove and free the node at the given index */
DS_DLinkedList **DS_DLinkedList_remove(DS_DLinkedList **list, size_t num);

/* DS_DLinkedList_fwd: Fastforward n nodes, if node is null return an error */
DS_DLinkedList **DS_DLinkedList_fwd(DS_DLinkedList **list, size_t num);

/* DS_DLinkedList_rwd: Rewind n nodes, if node is the head return an error */
DS_DLinkedList **DS_DLinkedList_rwd(DS_DLinkedList **list, size_t num);

/*  DS_DLinkedList_set: Set the data at index n to be the given data */
DS_DLinkedList **DS_DLinkedList_set(DS_DLinkedList **list, size_t index, Data data);

/* DS_DLinkedList_size: Returns the quantity of nodes in a linked list */
size_t DS_DLinkedList_size(DS_DLinkedList **list);

/* DS_DLinkedList_clear: Destroy all nodes in the list */
int DS_DLinkedList_clear(DS_DLinkedList **list);

