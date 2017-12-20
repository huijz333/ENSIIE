# include "list.h"

/** merge the two lists represented by the given nodes to the result node */
static void list_merge(t_node * n1, t_node * n2, t_node ** result) {
	if (n1 == NULL) {
		*result = n2;
		return;
	}
	if (n2 == NULL) {
		*result = n1;
		return;
	}

	if (n1->v <= n2->v) {
		*result = n1;
	} else {
		*result = n2;
	}

	t_node * current1 = n1;
	t_node * current2 = n2;
	t_node * previous;
	while (current1 != NULL && current2 != NULL) {
		if (current1->v <= current2->v) {
			previous = current1;
			current1 = current1->next;
			if (current1 == NULL || current1->v > current2->v) {
				previous->next = current2;
			}
		} else {
			previous = current2;
			current2 = current2->next;
			if (current2 == NULL || current2->v >= current1->v) {
				previous->next = current1;
			}
		}
	}
}

/* split the head node into two sub nodes (begin and middle of the list */
static void list_split(t_node * head, t_node ** n1, t_node ** n2) {
	if (head == NULL || head->next == NULL) {
		*n1 = head;
		*n2 = NULL;
		return ;
	}

	t_node * mid = head;
	t_node * end = head->next;
	while (end != NULL) {
		end = end->next;
		if (end != NULL) {
			mid = mid->next;
			end = end->next;
		}
	}

	*n1 = head;
	*n2 = mid->next;
	mid->next = NULL;
}

/** merge sort the given list */
static void list_mergesort(t_node ** headRef) {
	t_node * head = *headRef;

	/* list of size 0 or 1 */
	if (head == NULL || head->next == NULL) {
		return ;
	}

	t_node * n1;
	t_node * n2;
	list_split(head, &n1, &n2);
	list_mergesort(&n1);
	list_mergesort(&n2);
	list_merge(n1, n2, headRef);
}

/** merge sort the list */
void list_sort(t_list * lst) {
	list_mergesort(&(lst->head));
}
