#include "DS_DLinkedList.h"
#include "../general/DS_Error.h"
#include <stdlib.h>
#include <string.h>

/*
 * DS_DLinkedList_new_node: Internal function for creating new list nodes.
 */
DS_DLinkedList *DS_DLinkedList_new_node(Data data)
{
	DS_DLinkedList *new_node = NULL;
	if ((new_node = malloc(sizeof(DS_DLinkedList))) == NULL) {
		DS_Error_set("%s: memory allocation failed.", __func__);
		return NULL;
	}
	new_node->next = NULL;
	new_node->prev = NULL;
	new_node->data = data;

	return new_node;
}

/*
 * DS_DLinkedList_add: Create the next node in the list and add data.
 */
DS_DLinkedList **DS_DLinkedList_add(DS_DLinkedList **list, Data data)
{
	if (*list == NULL)
		*list = DS_DLinkedList_new_node(data);
	else {
		while ((*list)->next != NULL)
			list = &(*list)->next;

		if (((*list)->next = DS_DLinkedList_new_node(data)) == NULL) {
			DS_Error_append("%s: ", __func__);
			return NULL;
		}

		(*list)->next->prev = (*list);
		list = &(*list)->next;
		(*list)->data = data;
		(*list)->next = NULL;
	}

	return list;
}

/*
 * DS_DLinkedList_get: Returns the requested node if it exists, NULL if iit does
 * not.
 */
DS_DLinkedList **DS_DLinkedList_get(DS_DLinkedList **list, size_t num)
{
	size_t i;

	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	}

	if (--num == SIZE_MAX) {
		DS_Error_set("%s: list 0 does not exist, count starts at 1.", __func__);
		return NULL;
	}

	for (i = 0; i < num && (*list)->next != NULL; i++)
		list = &(*list)->next;

	if (i < num && (*list)->next == NULL) {
		DS_Error_set("%s: Out of bounds.", __func__);
		return NULL;
	}

	return list;
}

/*
 * DS_DLinkedList_do: Iterate over the entire list, performing the given
 * function upon each node.
 */
int DS_DLinkedList_do(DS_DLinkedList **list, void *var, int(*func)(void*, void*))
{
	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: NULL pointer.", __func__);
		return -1;
	}

	while (*list != NULL) {
		if ((*func)((void*)list, (void*)var))
			return -1;
		list = &(*list)->next;
	}

	return 0;
}

/*
 * DS_DLinkedList_insert: Insert a new node at current location.
 */
DS_DLinkedList **DS_DLinkedList_insert(DS_DLinkedList **list, Data data)
{
	DS_DLinkedList *new;

	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	}
	if ((new = DS_DLinkedList_new_node(data)) == NULL) {
		DS_Error_append("%s: ", __func__);
		return NULL;
	}

	new->next = *list;
	*list = new;

	return list;
}

/*
 * DS_DLinkedList_insert_at: Insert a new node before node n.
 */
DS_DLinkedList **DS_DLinkedList_insert_at(DS_DLinkedList **list, size_t num, Data data)
{
	DS_DLinkedList *new;

	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	} else if (num == 0) {
		DS_Error_set("%s: Out of bounds.", __func__);
		return NULL;
		/* Get node, but not number one; You are already there */
	} else if ((num != 1) && (list = DS_DLinkedList_get(list, num)) == NULL) {
		DS_Error_insert("%s: ", __func__);
		return NULL;
	}

	/* We should now be at node n - 1, insert a node here. */
	if ((new = DS_DLinkedList_new_node(data)) == NULL) {
		DS_Error_append("%s: ", __func__);
		return NULL;
	}

	new->next = *list;
	new->prev = (*list)->prev;
	*list = new;

	return list;
}

/*
 * DS_DLinkedList_remove: Remove and free the node at the given index.
 */
DS_DLinkedList **DS_DLinkedList_remove(DS_DLinkedList **list, size_t num)
{
	DS_DLinkedList *old;

	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: Null pointer.", __func__);
		return NULL;
	} else if (num == 0) {
		DS_Error_set("%s: Out of bounds.", __func__);
		return NULL;
		/* Get node, not if num is 1 it is the current node; Deal with the
		 * case of removing the first node  */
	} else if ((num != 1) && (list = DS_DLinkedList_get(list, num)) == NULL) {
		DS_Error_insert("%s: ", __func__);
		return NULL;
	}

	/* Remove the node */
	old = *list;
	*list = (*list)->next;
	free(old);

	return list;
}

/*
 * DS_DLinkedList_fwd: Fastforward n nodes, if node is null return an error.
 */
DS_DLinkedList **DS_DLinkedList_fwd(DS_DLinkedList **list, size_t num)
{
	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: Null pointer.", __func__);
		return NULL;
	}

	while (*list != NULL && num)
		list = &(*list)->next, num--;

	if (*list == NULL) {
		DS_Error_set("%s: Out of bounds, you have reached the end of the list.", __func__);
		return NULL;
	}

	return list;
}

/*
 * DS_DLinkedList_rwd: Rewind n nodes, if node is the head return an error.
 */
DS_DLinkedList **DS_DLinkedList_rwd(DS_DLinkedList **list, size_t num)
{
	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: Null pointer.", __func__);
		return NULL;
	}

	while (*list != NULL && num)
		list = &(*list)->prev, num--;

	if (*list == NULL) {
		DS_Error_set("%s: Out of bounds, you have reached the first node.", __func__);
		return NULL;
	}

	return list;
}

/*
 *  DS_DLinkedList_set: Set the data at index n to be the given data.
 */
DS_DLinkedList **DS_DLinkedList_set(DS_DLinkedList **list, size_t num, Data data)
{
	if (list == NULL || *list == NULL) { 
		DS_Error_set("%s: NULL pointer.", __func__);
		return NULL;
	} else if ((list = DS_DLinkedList_get(list, num)) == NULL) {
		DS_Error_set("%s: ", __func__);
		return NULL;
	}

	(*list)->data = data;

	return list;
}

/*
 * DS_DLinkedList_size: Returns the quantity of nodes in a linked list.
 */
size_t DS_DLinkedList_size(DS_DLinkedList **list)
{
	size_t i = 0;

	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: NULL pointer.", __func__);
		return 0;
	}

	while(*list != NULL)
		list = &(*list)->next, ++i;

	return i;
}

/*
 * DS_DLinkedList_clear: Destroy all nodes in the list.
 */
int DS_DLinkedList_clear(DS_DLinkedList **list)
{
	DS_DLinkedList *old;

	if (list == NULL || *list == NULL) {
		DS_Error_set("%s: NULL pointer.", __func__);
		return -1;
	}

	while (*list != NULL) {
		old = *list;
		*list = (*list)->next;
		free(old);
	}
	free(*list);

	return 0;
}

