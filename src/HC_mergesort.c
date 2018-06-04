#include "HC_mergesort.h"

HC_HuffmanNode* _advance_and_cut(HC_HuffmanNode *list, size_t len)
{
	HC_HuffmanNode *fin;

	for (; len && list; len--) {
		if (len == 1)
			fin = list;
		list = list->next;
	}

	if (list)
		fin->next = NULL;

	return list;
}

/*
 * _merge: Merge together two linked lists whist sorting them by the given
 * function.
 */
HC_HuffmanNode* _merge(
				HC_HuffmanNode **list,
				HC_HuffmanNode **left, HC_HuffmanNode **right,
				HC_HuffmanNode **end,  HC_HuffmanNode **tail,
				int(*comp)(void*, void*))
{
	HC_HuffmanNode *new = NULL;

	while (*left || *right)
	{
		/* Two list to compare */
		if (*left && *right)
			if ((comp(*left, *right)) > 0)
				new = *right, *right = (*right)->next;
			else
				new = *left, *left = (*left)->next;

		/* Merge the remaining nodes */
		else if (*left)
			new = *left, *left = (*left)->next;

		else if (*right)
			new = *right, *right = (*right)->next;

		/* If there is not yet a merged list, make one; Else
		 * add the new node to the new lists tail */
		if (*tail)
			(*tail)->next = new;
		else
			*list = new;

		*tail = new;
	}

	/* Merge done, move the left list to the start of the remaining
	 * original list */
	*left = *end;

	return *left;
}

/*
 * _mergesort_cut: Merge sort for linked lists, this version does not use a
 * count to signal the end of a segmented list, the lists are cut and the NULL
 * terminator is used to signal the end of a list.
 */
HC_HuffmanNode **_mergesort_cut(HC_HuffmanNode **list, size_t m_len, int(*comp)(void*, void*))
{
	size_t count;
	HC_HuffmanNode *left, *right, *tail, *end;

	/* Set the merge count to 0, this value will effect the recursion of
	 * this function, when only one merge occurs it was the final merge. */
	count = 0;

	/* Set the left list to the head of the current list and then the
	 * current list to NULL; The merged list will have a different head. */
	left = *list;
	tail = *list = NULL;
	
	/* Whilst there is still a list, keep working the list; Right to left */
	while (left)
	{
		count++;
		right = left;

		right = _advance_and_cut(left, m_len);
		end = _advance_and_cut(right, m_len);

		/* Whilst there remains any length of either the right or the
		 * left list; compare the two and sort in order, else merge the
		 * remainder, there should never be more than one extra */
		left = _merge(list, &left, &right, &end, &tail, comp);
	}

	/* Clip the end of the list to stop an infinite loop, if the last node
	 * came from the middle of the list, it will have still the following
	 * node attached. */
	tail->next = NULL;

	/* If more than one merge has been made, continue. If only one merge or
	 * fewer have been counted; The sort is finished */
	if (count > 1)
		list = _mergesort_cut(list, m_len * 2, comp);

	return list;
}

/*
 * HC_mergesort: Merge sort for linked lists, long function using count to keep
 * track of lists.
 */
HC_HuffmanNode **_mergesort(HC_HuffmanNode **list, size_t m_len, int(*comp)(void*, void*))
{
	size_t i, count, l_len, r_len;
	HC_HuffmanNode *left, *right, *new, *tail;

	/* Set the merge count to 0, this value will effect the recursion of
	 * this function */
	count = 0;

	/* Set the left list to the head of the current list and then the
	 * current list to NULL, the merged list will have a different head */
	left = *list;
	new = tail = *list = NULL;
	
	while (left)
	{
		count++;
		right = left;

		/* Advance the right list head and count the number of nodes
		 * for the length of the left. */
		l_len = 0;
		for (i = 0; i < m_len; i++) {
			l_len++;
			right = right->next;
			if (!right)
				break;
		}

		r_len = m_len;

		/* Whilst there remains any length of either the right or the
		 * left lists, compare the two else merge the remainder */
		while (l_len > 0 || (r_len > 0 && right))
		{
			/* Two list to compare */
			if (l_len && r_len && right)
				if ((comp(left, right)) > 0)
					new = right, right = right->next, r_len--;
				else
					new = left, left = left->next, l_len--;

			/* Merge the remaining node */
			else if (!r_len || !right)
				new = left, left = left->next, l_len--;

			else if (!l_len)
				new = right, right = right->next, r_len--;

			/* If there is not yet a merged list, make one; Else
			 * add the new node to the new lists tail */
			if (tail)
				tail->next = new;
			else
				*list = new;

			tail = new;
		}

		/* Merge done, move the left list forwards to the end of the
		 * right list, ready for the next sort, or the end */
		left = right;
	}

	/* Clip the end of the list to stop an infinite loop, if the last node
	 * came from the middle of the list, it will have an address in next */
	tail->next = NULL;

	/* If more than one merge has been made, continue. If only one merge or
	 * fewer have been counted, then the sort is finished */
	if (count > 1)
		list = _mergesort(list, m_len * 2, comp);

	return list;
}

HC_HuffmanNode **HC_mergesort(HC_HuffmanNode **list, int(*comp)(void*, void*))
{
	/* Doh! Nothing to see here, please move along. */
	if (list == NULL || *list == NULL || (*list)->next == NULL)
		return NULL;

	/* Set the ball rolling with a merge length of one node */
	return _mergesort(list, 1, comp);
}

HC_HuffmanNode **HC_mergesort_cut(HC_HuffmanNode **list, int(*comp)(void*, void*))
{
	/* Doh! Nothing to see here, please move along. */
	if (list == NULL || *list == NULL || (*list)->next == NULL)
		return NULL;

	/* Set the ball rolling with a merge length of one node */
	return _mergesort_cut(list, 1, comp);
}

